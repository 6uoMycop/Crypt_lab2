#include "Header.h"

int readText(uint8_t** pBuf, const char* pFileName)
{
    int iLen = 0;

    FILE* F = fopen(pFileName, "rb");
    if (F == NULL)
    {
        return -1;
    }

    fseek(F, 0L, SEEK_END);
    iLen = ftell(F);
    fseek(F, 0L, SEEK_SET);
    
    *pBuf = (uint8_t*)calloc(iLen, iLen * sizeof(uint8_t));

    fread(*pBuf, sizeof(uint8_t), iLen, F);

    fclose(F);

    return (iLen);
}

void S_blocks(uint8_t* B, const uint8_t* S)
{
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        uint8_t rez[2] = { 0 };
        uint8_t ind = 0;

        ind = B[i] >> 4;
        rez[0] = S[ind];

        ind = B[i] << 4;
        ind >>= 4;
        rez[1] = S[ind];

        B[i] = (rez[0] << 4) + rez[1];
    }
}

int P_func(int num)
{
    return ((29 * num + 31) % 32);
}

// x = (-31 + y) 29^-1 =
//   =   (1 + y) 21 (mod 32)
int P_func_reverse(int num)
{
    return (((num + 1) * 21) % 32);
}

void P_block(uint8_t* B, int (*func) (int))
{
    uint32_t tmpB = 0;
    for (int i = 0; i < 8 * BLOCK_SIZE; i++)
    {
        int newPos = func(i);
        uint32_t rezBit = (*((uint32_t*)B) << newPos);
        rezBit >>= 8 * BLOCK_SIZE - 1;
        rezBit <<= 8 * BLOCK_SIZE - i - 1;

        tmpB += rezBit;
    }

    *((uint32_t*)B) = tmpB;
}

void encryptBlock(uint8_t* Y, const uint8_t* X, const uint8_t* key)
{
    memcpy(Y, X, BLOCK_SIZE);

    for (int j = 0; j < d; j++)
    {
        // 1. X xor k
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            Y[i] ^= key[i];
        }
    
        // 2. S-blocks
        S_blocks(Y, S);
    
        // 3. Permutation
        P_block(Y, P_func);
    }
    // 4. whitening
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        Y[i] ^= key[i];
    }

}

void decryptBlock(const uint8_t* Y, uint8_t* X, const uint8_t* key)
{
    memcpy(X, Y, BLOCK_SIZE);

    // 0. whitening
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        X[i] ^= key[i];
    }

    for (int j = 0; j < d; j++)
    {
        // 1. Permutation^-1
        P_block(X, P_func_reverse);
    
        // 2. S-blocks^-1
        S_blocks(X, S_reverse);
    
        // 3. Y xor k
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            X[i] ^= key[i];
        }
    }
}

void encryptRemainder(uint32_t* Y_prev, const uint32_t* X_remainder, uint8_t* Y_remainder, const uint32_t* key, int l)
{
    uint32_t tmpBlock = 0;

    memcpy(&tmpBlock,                 X_remainder,              l);
    memcpy((uint8_t*)(&tmpBlock) + l, &(((uint8_t*)Y_prev)[l]), BLOCK_SIZE - l);
    memcpy(Y_remainder,               Y_prev,                   l);

    encryptBlock((uint8_t*)Y_prev, (uint8_t*)(&tmpBlock), (uint8_t*)key);
}

void decryptRemainder(uint32_t* X_prev, const uint32_t* Y_remainder, uint8_t* X_remainder, const uint32_t* key, int l)
{
    uint32_t tmpBlock = 0;

    memcpy(&tmpBlock,                 Y_remainder,              l);
    memcpy((uint8_t*)(&tmpBlock) + l, &(((uint8_t*)X_prev)[l]), BLOCK_SIZE - l);
    memcpy(X_remainder,               X_prev,                   l);

    decryptBlock((uint8_t*)(&tmpBlock), (uint8_t*)X_prev, (uint8_t*)key);
}

void E(uint8_t* cyphertext, const uint8_t* plaintext, const uint8_t* key, int len)
{
    int i = 0;
    int l = len % BLOCK_SIZE;

    for (i = 0; i < len / BLOCK_SIZE; i++)
    {
        encryptBlock(&(cyphertext[i * BLOCK_SIZE]), &(plaintext[i * BLOCK_SIZE]), key);
    }
    if (l != 0)
    {
        encryptRemainder(
            (uint32_t*)       &(cyphertext[(i - 1) * BLOCK_SIZE]),
            (uint32_t*)       &(plaintext[i * BLOCK_SIZE]),
                              &(cyphertext[i * BLOCK_SIZE]),
            (const uint32_t*) key,
                              l
        );
    }
}

void D(const uint8_t* cyphertext, uint8_t* plaintext, const uint8_t* key, int len)
{
    int i = 0;
    int l = len % BLOCK_SIZE;

    for (i = 0; i < len / BLOCK_SIZE; i++)
    {
        decryptBlock(&(cyphertext[i * BLOCK_SIZE]), &(plaintext[i * BLOCK_SIZE]), key);
    }
    if (l != 0)
    {
        decryptRemainder(
            (uint32_t*) & (plaintext[(i - 1) * BLOCK_SIZE]),
            (uint32_t*) & (cyphertext[i * BLOCK_SIZE]),
            &(plaintext[i * BLOCK_SIZE]),
            (const uint32_t*)key,
            l
        );
    }


}

#ifndef KEY_SEARCH
int main(int argc, char* argv[])
{
    uint8_t* plaintext = NULL;
    uint8_t* key = NULL;
    uint8_t* cyphertext = NULL;
    uint8_t* plaintextDecrypted = NULL;
    int      iPlainLen = 0;
    int      iKeyLen = 0;
    FILE* rezFile = NULL;

    iPlainLen = readText(&plaintext, argv[1]);
    if (iPlainLen < BLOCK_SIZE)
    {
        printf("error\n");
        return -1;
    }
    iKeyLen = readText(&key, "key");
    if (iKeyLen == -1 && iKeyLen != BLOCK_SIZE)
    {
        free(plaintext);
        printf("error\n");
        return -1;
    }
    cyphertext = (uint8_t*)calloc(iPlainLen, sizeof(uint8_t));
    plaintextDecrypted = (uint8_t*)calloc(iPlainLen, sizeof(uint8_t)); // проверить длину

    ///
    E(cyphertext, plaintext, key, iPlainLen);
    //D(cyphertext, plaintextDecrypted, key, iPlainLen);

    uint32_t keyWrong = 0x12345678;
    D(cyphertext, plaintextDecrypted, (uint8_t*)&keyWrong, iPlainLen);



    ///

    rezFile = fopen("encrypted", "wb");
    fwrite(cyphertext, 1, iPlainLen, rezFile); // проверить длину файла
    fclose(rezFile);

    rezFile = fopen("decrypted", "wb");
    fwrite(plaintextDecrypted, 1, iPlainLen, rezFile); // проверить длину файла
    fclose(rezFile);

    free(plaintext);
    free(key);
    free(cyphertext);
    free(plaintextDecrypted);

    system("pause");
    return 0;
}
#endif // !KEY_SEARCH
