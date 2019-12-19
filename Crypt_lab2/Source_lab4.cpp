#include "Header.h"

void encryptCBC(uint32_t* cyphertext, const uint32_t* plaintext, const uint8_t* key, int len, const uint8_t* iv)
{
    int i = 0;
    int l = len % BLOCK_SIZE;
    uint32_t Yi = iv[3] + (iv[2] << 8) + (iv[1] << 16) + (iv[0] << 24);
    uint32_t XYi = plaintext[0] ^ Yi;

    encryptBlock((uint8_t*)&(cyphertext[0]), (uint8_t*)&XYi, key);

    for (i = 1; i < len / BLOCK_SIZE; i++)
    {
        XYi = plaintext[i] ^ cyphertext[i - 1];
        encryptBlock((uint8_t*)&(cyphertext[i]), (uint8_t*)&XYi, key);
    }
    if (l != 0) // remainder
    {
        encryptRemainder(
            &(cyphertext[(i - 1)]),
            &plaintext[i],
            (uint8_t*)&(cyphertext[i]),
            (const uint32_t*)key,
            l
        );
    }
}

void decryptCBC(const uint32_t* cyphertext, uint32_t* plaintext, const uint8_t* key, int len, const uint8_t* iv)
{
    int i = 0;
    int l = len % BLOCK_SIZE;
    uint32_t Yi = iv[3] + (iv[2] << 8) + (iv[1] << 16) + (iv[0] << 24);

    decryptBlock((const uint8_t*)&(cyphertext[0]), (uint8_t*)&(plaintext[0]), key);
    plaintext[0] ^= Yi;

    for (i = 1; i < len / BLOCK_SIZE - 1; i++)
    {
        decryptBlock((const uint8_t*)&(cyphertext[i]), (uint8_t*)&(plaintext[i]), key);
        plaintext[i] ^= cyphertext[i - 1];
    }
    if (l != 0)
    {
        decryptBlock((const uint8_t*)&(cyphertext[i]), (uint8_t*)&(plaintext[i]), key);
        i++;

        decryptRemainder(
            &plaintext[(i - 1)],
            &(cyphertext[i]),
            (uint8_t*)&(plaintext[i]),
            (const uint32_t*)key,
            l
        );
        plaintext[i - 1] ^= cyphertext[i - 2];
    }
    else
    {
        decryptBlock((const uint8_t*)&(cyphertext[i]), (uint8_t*)&(plaintext[i]), key);
        plaintext[i] ^= cyphertext[i - 1];
    }
}

int main(int argc, char* argv[])
{
    uint8_t* plaintext = NULL;
    uint8_t* key = NULL;
    uint8_t* iv = NULL;
    uint8_t* cyphertext = NULL;
    uint8_t* plaintextDecrypted = NULL;
    int      iPlainLen = 0;
    int      iKeyLen = 0;
    int      iivLen = 0;
    FILE*    rezFile = NULL;

    iPlainLen = readText(&plaintext, argv[1]);
    if (iPlainLen < BLOCK_SIZE)
    {
        printf("error\n");
        system("pause");
        return -1;
    }
    iKeyLen = readText(&key, "key");
    if (iKeyLen == -1 && iKeyLen != BLOCK_SIZE)
    {
        free(plaintext);
        printf("error\n");
        system("pause");
        return -1;
    }
    iivLen = readText(&iv, "iv");
    if (iivLen == -1 && iivLen != BLOCK_SIZE)
    {
        free(plaintext);
        free(key);
        printf("error\n");
        system("pause");
        return -1;
    }
    cyphertext = (uint8_t*)calloc(iPlainLen + sizeof(uint32_t), sizeof(uint8_t));
    plaintextDecrypted = (uint8_t*)calloc(iPlainLen + sizeof(uint32_t), sizeof(uint8_t));

    ///
    encryptCBC((uint32_t*)cyphertext, (const uint32_t*)plaintext, key, iPlainLen, iv);
    //D(cyphertext, plaintextDecrypted, key, iPlainLen);

    uint8_t keyWrong[4] = { 0x12,0x34,0x56,0x78 };
    uint8_t ivWrong[4]  = { 0x78,0x56,0x34,0x12 };
    decryptCBC((const uint32_t*)cyphertext, (uint32_t*)plaintextDecrypted, key, iPlainLen, iv);
    //decryptCBC((const uint32_t*)cyphertext, (uint32_t*)plaintextDecrypted, key, iPlainLen, ivWrong);



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
    free(iv);

    system("pause");
    return 0;
}