#include "Header.h"
#include <thread>
#include <mutex>
#include <time.h>

std::mutex mutStdout;

const uint32_t key[4] = {
    0x47fad3d6,
    0xbed68245,
    0x088114c0,
    0xff7398d4
};
const uint32_t X[4] = {
    0x7fea2b9d,
    0x4351f16e,
    0xd9ad8629,
    0x35bc076a
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
