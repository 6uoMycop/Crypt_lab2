#include "Header.h"
#include <thread>
#include <mutex>
#include <time.h>

std::mutex mutStdout;

const uint32_t key[4] = {
    0x43438a5b,
    0x4cdc1e74,
    0xb27fdd43,
    0xce8aa851
};
const uint32_t X[4] = {
    0xcae25f86,
    0xd5f7c38e,
    0x61f937ea,
    0x62000cd2
};

void worker(uint32_t threadNumber, clock_t start)
{
    uint32_t Y = 0;
    uint32_t X_cur = X[threadNumber];

    long long i = 0;
    while (1)
    {
        E((uint8_t*)&Y, (const uint8_t*)&X_cur, (const uint8_t*)&key[threadNumber], BLOCK_SIZE);
        X_cur = Y;

        if (Y == X[threadNumber])
        {
            mutStdout.lock();
            clock_t end = clock();
            printf("Thread %i: key 0x%08x; result %llu; time %f seconds\n", threadNumber, key[threadNumber], i, (double)(end - start) / CLOCKS_PER_SEC);
            mutStdout.unlock();
            break;
        }
        i++;
    }
}

int main()
{
    std::thread* pThreads[4];

    clock_t start = clock();
    for (uint32_t i = 0; i < 4; i++)
    {
        pThreads[i] = new std::thread(worker, i, start);
    }
    for (int i = 0; i < 4; i++)
    {
        pThreads[i]->join();
    }
    
    system("pause");
    return 0;
}
