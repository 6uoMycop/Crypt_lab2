#include "Header.h"


#ifdef KEY_SEARCH

#include <thread>
#include <deque>
#include <mutex>
#include <time.h>

std::mutex mutStdout;
//SYNCHRONIZATION_BARRIER barrier;

uint32_t g_size = 0;

uint32_t keys1[133] = 
{ 
    0x00000007,
    0x0000000a,
    0x0000000e,
    0x00000001,
    0x00000011,
    0x0000001c,
    0x0000001b,
    0x00000018,
    0x00000011,
    0x0000001e,
    0x00000001,
    0x00000011,
    0x0000000b,
    0x0000000d,
    0x0000001d,
    0x00000013,
    0x0000000f,
    0x0000000b,
    0x0000001f,
    0x00000019,
    0x0000000b,
    0x00000019,
    0x0000000c,
    0x00000012,
    0x0000000e,
    0x00000017,
    0x0000000f,
    0x0000001a,
    0x00000019,
    0x00000003,
    0x00000019,
    0x00000019,
    0x0000001f,
    0x00000018,
    0x00000016,
    0x00000011,
    0x0000001a,
    0x0000002b,
    0x0000003c,
    0x00000037,
    0x00000031,
    0x00000020,
    0x00000023,
    0x00000035,
    0x00000020,
    0x0000003a,
    0x00000036,
    0x0000003e,
    0x0000002c,
    0x00000037,
    0x00000024,
    0x0000002d,
    0x00000035,
    0x00000026,
    0x00000022,
    0x00000032,
    0x00000037,
    0x00000036,
    0x00000027,
    0x0000002d,
    0x00000035,
    0x00000023,
    0x0000004b,
    0x00000059,
    0x00000041,
    0x00000042,
    0x0000004d,
    0x0000005f,
    0x0000005c,
    0x00000056,
    0x00000056,
    0x0000005c,
    0x0000005b,
    0x00000059,
    0x00000059,
    0x00000058,
    0x00000059,
    0x00000049,
    0x00000056,
    0x0000004c,
    0x00000058,
    0x00000044,
    0x0000004c,
    0x00000049,
    0x00000041,
    0x0000004f,
    0x0000004a,
    0x00000041,
    0x00000045,
    0x00000040,
    0x00000053,
    0x00000042,
    0x00000052,
    0x0000005a,
    0x00000078,
    0x0000007e,
    0x0000006a,
    0x0000007a,
    0x00000072,
    0x00000072,
    0x00000066,
    0x00000063,
    0x0000007f,
    0x0000007e,
    0x00000067,
    0x00000074,
    0x0000006f,
    0x00000068,
    0x0000006b,
    0x0000007c,
    0x0000006e,
    0x00000061,
    0x00000064,
    0x0000006c,
    0x00000061,
    0x0000006a,
    0x00000065,
    0x00000068,
    0x0000007c,
    0x00000064,
    0x00000065,
    0x0000007d,
    0x00000069,
    0x0000007c,
    0x00000075,
    0x00000065,
    0x00000072,
    0x00000063,
    0x0000007d,
    0x00000064,
    0x0000006a,
    0x0000009d,
    0x00000088
};

uint32_t keys2[133] =
{
    0x02ba2f28,
    0x161deb92,
    0x1b01dcb6,
    0x1dbe3543,
    0x20abb4fa,
    0x20ff6827,
    0x22fa8b2a,
    0x35bc076a,
    0x3c98233c,
    0x3db7f8e0,
    0x3f8b181b,
    0x4f48744d,
    0x5715fc19,
    0x5aee55c8,
    0x5d6003c5,
    0x6d08f1b2,
    0x6ed78dd5,
    0x86b6fca3,
    0x87d582b3,
    0x90012d07,
    0x9b780604,
    0x9bba5625,
    0xa0ec8d1c,
    0xa2f07745,
    0xac37a975,
    0xac35abc0,
    0xb65bcde2,
    0xb90f64ec,
    0xba3a6a7b,
    0xbb3adf1b,
    0xc6efaafb,
    0xcdbf0779,
    0xe333007b,
    0xe3a2db5f,
    0xec4495c4,
    0xfac39247,
    0xff7398d4,
    0x06fa56ac,
    0x0cf77ce6,
    0x13528252,
    0x275fb84a,
    0x2b408a27,
    0x2fa9748d,
    0x365806fb,
    0x3ed743b1,
    0x401a6bf1,
    0x43438a5b,
    0x4cdc1e74,
    0x627d184a,
    0x7e404902,
    0x7fea2b9d,
    0x876de92f,
    0x89c892ba,
    0x8dec2e48,
    0x964d0cfb,
    0x9943499e,
    0xaf6b686a,
    0xb27fdd43,
    0xce8aa851,
    0xdcaa4fff,
    0xe78c6015,
    0xea4d348f,
    0x06eda66f,
    0x130f48bd,
    0x1d2924d9,
    0x256413cc,
    0x2763059a,
    0x34529608,
    0x389f2023,
    0x49b6838a,
    0x5b4470cd,
    0x635f7204,
    0x6b5c1563,
    0x8d3e5032,
    0x933d4b74,
    0x95c26e66,
    0xa249520a,
    0xab22f92f,
    0xabd6db6c,
    0xb0761a6a,
    0xc23c7c9e,
    0xc3caafe2,
    0xc9f0a08f,
    0xcae25f86,
    0xd5f7c38e,
    0xdb217a1b,
    0xddc6dc27,
    0xe04062c0,
    0xe02715b6,
    0xe747b522,
    0xe9861aab,
    0xf4c2f722,
    0xfbb4a8b3,
    0xffe167d6,
    0x0d09251a,
    0x115f2043,
    0x1948673e,
    0x28612219,
    0x4351f16e,
    0x47fad3d6,
    0x4b4075c8,
    0x51d8f29a,
    0x520d43b3,
    0x5d90433d,
    0x5faf2ad7,
    0x61f937ea,
    0x62000cd2,
    0x709127b8,
    0x7218a834,
    0x74ca31b4,
    0x7515c575,
    0x798f0d58,
    0x7fd203ed,
    0x88845abd,
    0x930e051f,
    0x93c8daa2,
    0x9dd2d6a9,
    0xa96d5830,
    0xb771ee21,
    0xb9d79824,
    0xbed68245,
    0xc2c4bf50,
    0xc3970fcc,
    0xc5db5faa,
    0xd9ad8629,
    0xda797ca3,
    0xdd939a62,
    0xe514166e,
    0xe7882222,
    0xef0ec4e2,
    0xf1e97ad1,
    0x088114c0,
    0x0f992c2c
};

//uint32_t* keysPtr[32]  = { NULL };
//uint32_t* Ptr[0xFFFFFFFF]     = { NULL };
//std::deque<uint32_t> Q;
//uint32_t* X_newPtr[32] = { NULL };
FILE* semi = NULL;

const uint32_t X = 0x12345678;

void worker(uint32_t threadNumber)
{
    uint32_t threadConst = threadNumber << 27;

    for (uint32_t i = 0; i < 0x7FFFFFF; i++)
    {
        uint32_t key = i | threadConst;
        uint32_t Y_0 = 0;
        uint32_t Y_1 = 0;
        uint32_t X_dec = 0;

        E((uint8_t*)&Y_0, (const uint8_t*)&X,   (const uint8_t*)&key, BLOCK_SIZE);
        E((uint8_t*)&Y_1, (const uint8_t*)&Y_0, (const uint8_t*)&key, BLOCK_SIZE); // weak

        if (Y_1 == X)
        {
            mutStdout.lock();
            printf("0x%08x\n", key);
            mutStdout.unlock();
        }
    }
}

void worker1(uint32_t threadNumber)
{
    uint32_t threadConst = threadNumber << 27;

    //for (uint32_t i = 0; i < 0x7FFFFFF; i++)
    //{
    //    uint32_t key = i | threadConst;
    //    E((uint8_t*)&Q[key], (const uint8_t*)&X, (const uint8_t*)&key, BLOCK_SIZE);
    //}

    //EnterSynchronizationBarrier(&barrier, 0);

    //mutStdout.lock();
    //printf("thread %i\n", threadNumber);
    //mutStdout.unlock();

    for (uint32_t i = threadNumber; i < 0xFFFFFFFF; i += 32)
    {
        uint32_t X_dec = 0;
        D((const uint8_t*)&X, (uint8_t*)&X_dec, (const uint8_t*)&i, BLOCK_SIZE);

        for (uint32_t k = 0; k < 0xFFFFFFFF; k++)
        {
            uint32_t Y = 0;
            E((uint8_t*)&Y, (const uint8_t*)&X, (const uint8_t*)&k, BLOCK_SIZE);
            if (X_dec == Y)
            {
                mutStdout.lock();
                fprintf(semi, "0x%08x 0x%08x\n", i, k);
                printf("0x%08x 0x%08x\n", i, k);
                mutStdout.unlock();
            }
        }

        //EnterSynchronizationBarrier(&barrier, 0);
        //if (threadNumber == 0)
        //{
        //    for (int n = 0; n < 32; n++)
        //    {
        //        Q.pop_front();
        //    }
        //}
        //EnterSynchronizationBarrier(&barrier, 0);
        //free(Ptr[i]);
        //mutStdout.lock();
        //printf("thread %i: iter 0x%08x\n", threadNumber, i);
        //mutStdout.unlock();
    }

    //for (uint32_t i = 0; i < 0x7FFFFFF; i++)
    //{
    //    //keysPtr[threadNumber][i] = i | threadConst;
    //    //E((uint8_t*)&YPtr[threadNumber][i], (const uint8_t*)&X, (const uint8_t*)&keysPtr[threadNumber][i], BLOCK_SIZE);
    //    //D((const uint8_t*)&YPtr[threadNumber][i], (uint8_t*)&X_newPtr[threadNumber][i], (const uint8_t*)&keysPtr[threadNumber][i], BLOCK_SIZE);
    //    uint32_t key = i | threadConst;
    //    E((uint8_t*)&YPtr[threadNumber][i], (const uint8_t*)&X, (const uint8_t*)&key, BLOCK_SIZE);
    //    D((const uint8_t*)&YPtr[threadNumber][i], (uint8_t*)&X_newPtr[threadNumber][i], (const uint8_t*)&key, BLOCK_SIZE);
    //}
    //
    //mutStdout.lock();
    //printf("thread %i\n", threadNumber);
    //mutStdout.unlock();
    //EnterSynchronizationBarrier(&barrier, 0);
    //
    //for (int i = 0; i < 32; i++)
    //{
    //    for (int j = threadNumber; j < 0x7FFFFFF; j += 32)
    //    {
    //        for (int k = threadNumber + 1; k < 0x7FFFFFF; k++)
    //        {
    //            if (X_newPtr[i][j] == YPtr[i][k])
    //            {
    //                //printf("0x%08x 0x%08x\n", keysPtr[i][j], keysPtr[i][k]);
    //                mutStdout.lock();
    //                printf("[%i][%i] ; [%i][%i]\n", i, j, i, k);
    //                mutStdout.unlock();
    //            }
    //        }
    //
    //        for (int k = i + 1; k < 32; k++)
    //        {
    //            for (int m = 0; m < 0x7FFFFFF; m++)
    //            {
    //                if (X_newPtr[i][j] == YPtr[k][m])
    //                {
    //                    //printf("0x%08x 0x%08x\n", keysPtr[i][j], keysPtr[k][m]);
    //                    mutStdout.lock();
    //                    printf("[%i][%i] ; [%i][%i]\n", i, j, k, m);
    //                    mutStdout.unlock();
    //                }
    //            }
    //        }
    //    }
    //}
}

void worker2(uint32_t threadNumber)
{
    bool flag;
    for (uint32_t i = threadNumber; i < 133; i += 32)
    {
        flag = true;
        for (uint32_t k = 0; k < 1000; k++)
        {
            uint32_t X_cur = rand() % 0xFFFFFFFF;
            uint32_t X_dec = 0;
            uint32_t Y = 0;
            D((const uint8_t*)&X_cur, (uint8_t*)&X_dec, (const uint8_t*)&keys1[i], BLOCK_SIZE);
            E((uint8_t*)&Y, (const uint8_t*)&X_cur, (const uint8_t*)&keys2[i], BLOCK_SIZE);

            if (X_dec != Y)
            {
                mutStdout.lock();
                printf("0x%08x 0x%08x NOT OK\n", keys1[i], keys2[i]);
                mutStdout.unlock();
                flag = false;
                break;
            }
        }
        if (flag)
        {
            mutStdout.lock();
            printf("0x%08x 0x%08x OK\n", keys1[i], keys2[i]);
            mutStdout.unlock();
        }
    }
}


const uint32_t keyErr = 0xffffffff;
void encryptBlockErr(uint8_t* Y, const uint8_t* X, const uint8_t* key, int dErr)
{
    memcpy(Y, X, BLOCK_SIZE);

    for (int j = 0; j < dErr; j++)
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
void worker3(uint32_t threadNumber, int dErr)
{
    uint32_t const1 = 1 << threadNumber;
    uint32_t const2 = 0xFFFFFFFF ^ const1;
    uint32_t b = 0;
    uint32_t x1 = 0;
    uint32_t x2 = 0;
    uint32_t y1 = 0;
    uint32_t y2 = 0;
    uint32_t x1prev = 0;

    for (uint32_t i = 0; i < 0xFFFFFFFF; i++)
    {
        x1 = i | const1;
        x2 = i & const2;

        if (x1prev == x1)
        {
            continue;
        }

        encryptBlockErr((uint8_t*)&y1, (const uint8_t*)&x1, (const uint8_t*)&keyErr, dErr);
        encryptBlockErr((uint8_t*)&y2, (const uint8_t*)&x2, (const uint8_t*)&keyErr, dErr);
        b |= y1 ^ y2;
        
        if (b == 0xFFFFFFFF)
        {
            mutStdout.lock();
            printf("OK: 0x%08x, bit %i\n", x1, threadNumber);
            mutStdout.unlock();
            return;
        }

        x1prev = x1;
    }
    mutStdout.lock();
    printf("NOT OK: bit %i, reached: 0x%08x ^ 0x%08x = 0x%08x => 0x%08x\n", threadNumber, y1, y2, y1 ^ y2, b);
    mutStdout.unlock();
}

void checkKeys()
{
    std::thread* pThreads[32];

    //FILE* fIn = fopen("semi.txt", "r");
    srand(time(NULL));
    for (uint32_t i = 0; i < 32; i++)
    {
        pThreads[i] = new std::thread(worker2, i);
    }

    for (int i = 0; i < 32; i++)
    {
        pThreads[i]->join();
    }

    //fclose(fIn);
}

void computeAllKeys()
{
    std::thread* pThreads[32];
    semi = fopen("semi.txt", "w");
    //for (int i = 0; i < 0xFFFFFFFF; i++)
    //{
    //    Q.push_back(i);
    //    //keysPtr[i] = (uint32_t*)calloc(0x7FFFFFF, sizeof(uint32_t));
    //    //if (keysPtr[i] == NULL)
    //    //{
    //    //    printf("error, no memory\n");
    //    //    exit(-1);
    //    //}
    //    //Ptr[i] = (uint32_t*)calloc(1, sizeof(uint32_t));
    //    //if (Ptr[i] == NULL)
    //    //{
    //    //    printf("error, no memory\n");
    //    //    exit(-1);
    //    //}
    //    //X_newPtr[i] = (uint32_t*)calloc(0x7FFFFFF, sizeof(uint32_t));
    //    //if (X_newPtr[i] == NULL)
    //    //{
    //    //    printf("error, no memory\n");
    //    //    exit(-1);
    //    //}
    //}

    //InitializeSynchronizationBarrier(&barrier, 32, -1);
    for (uint32_t i = 0; i < 32; i++)
    {
        //pThreads[i] = new std::thread(worker, i);
        pThreads[i] = new std::thread(worker1, i);
    }

    for (int i = 0; i < 32; i++)
    {
        pThreads[i]->join();
    }

    fclose(semi);
}

void errorPropagation(int dErr)
{
    std::thread* pThreads[32];

    for (uint32_t i = 0; i < 32; i+=4)
    {
        pThreads[i + 0] = new std::thread(worker3, i + 0, dErr);
        pThreads[i + 1] = new std::thread(worker3, i + 1, dErr);
        pThreads[i + 2] = new std::thread(worker3, i + 2, dErr);
        pThreads[i + 3] = new std::thread(worker3, i + 3, dErr);
        pThreads[i + 0]->join();
        pThreads[i + 1]->join();
        pThreads[i + 2]->join();
        pThreads[i + 3]->join();
    }

    //for (int i = 0; i < 32; i++)
    //{
    //    pThreads[i]->join();
    //}
}

int main()
{
    //Q.push_back(1);
    //Q.push_back(2);
    //Q.push_back(3);
    //Q.pop_front();
    //Q.pop_front();

    //uint32_t weakKeys[5] = {
    //    0xd0ea1f4f, 0xf619f6f2,
    //    0xf6a53be5, 0x8a14b071,
    //    0x2a6ac026
    //};

    //printf("E(X)         E(E(X))      X            key\n");
    //uint32_t X_cur = 0x7382639F;
    //for (int i = 0; i < 5; i++)
    //{
    //    uint32_t Y_0 = 0;
    //    uint32_t Y_1 = 0;
    //    uint32_t X_dec = 0;
    //
    //    E((uint8_t*)&Y_0, (const uint8_t*)&X_cur, (const uint8_t*)&weakKeys[i], BLOCK_SIZE);
    //
    //    E((uint8_t*)&Y_1, (const uint8_t*)&Y_0, (const uint8_t*)&weakKeys[i], BLOCK_SIZE); // weak
    //
    //    printf("0x%08x : 0x%08x : 0x%08x : 0x%08x\n", Y_0, Y_1, X_cur, weakKeys[i]);
    //}

    

    //computeAllKeys();
    //checkKeys();
    errorPropagation(3);

    //for (int i = 0; i < 32; i++)
    //{
    //    //free(keysPtr[i]);
    //    free(Ptr[i]);
    //    //free(X_newPtr[i]);
    //}

    system("pause");
    return 0;
}

#endif // KEY_SEARCH

