#include "Header.h"
#include <time.h>
#include <mutex>
#include <thread>

std::mutex mutStdout;

const uint32_t realKey = 0x1000ffff;
const uint32_t X       = 0x12345678;
uint32_t       Y;

void worker(uint32_t threadNumber, clock_t start)
{
    uint32_t threadConst = threadNumber << 30;
    uint32_t Y_tmp;
    uint32_t key;

    for (uint32_t i = 0; i < 0x40000000; i++)
    {
        Y_tmp = 0;
        key = i | threadConst;

        E((uint8_t*)&Y_tmp, (const uint8_t*)&X, (const uint8_t*)&key, BLOCK_SIZE);

        if (Y == Y_tmp)
        {
            mutStdout.lock();
            printf("0x%08x\n", key);
            if (realKey == key)
            {
                clock_t end = clock();
                printf(" Time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
            }
            mutStdout.unlock();
        }
    }
}

int main()
{
    std::thread* pThreads[4];

    printf("Real key: 0x%08x\n\n", realKey);

    E((uint8_t*)&Y, (const uint8_t*)&X, (const uint8_t*)&realKey, BLOCK_SIZE);

    clock_t start = clock();
    for (uint32_t i = 0; i < 4; i++)
    {
        pThreads[i] = new std::thread(worker, i, start);
    }
    for (int i = 0; i < 4; i++)
    {
        pThreads[i]->join();
    }
    clock_t end = clock();

    printf("Time: %f seconds\n", (double) (end - start) / CLOCKS_PER_SEC);

    system("pause");
    return 0;
}
