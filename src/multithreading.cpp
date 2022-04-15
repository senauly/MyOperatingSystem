#include <multithreading.h>

#define nullptr 0

using namespace myos;
using namespace myos::common;
void printf(char *str);

int ThreadManager::numThreads = 0;
int ThreadManager::currentThread = -1;
Thread *ThreadManager::Threads[256] = {nullptr};
GlobalDescriptorTable *ThreadManager::gdt;
common::uint8_t ThreadManager::stack[256][5012];

void Thread::init(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpustate = (CPUState *)(stack + 4096 - sizeof(CPUState));

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    cpustate->eip = (uint32_t)entrypoint;
    cpustate->eflags = 0x202;
    yieldStatus = false;
}

Thread::~Thread()
{
}

ThreadManager::ThreadManager()
{
}

ThreadManager::ThreadManager(GlobalDescriptorTable *gdt)
{
    this->gdt = gdt;
}

ThreadManager::~ThreadManager()
{
}

/**
 * @brief Add a thread to an empty place in the array
 *
 * @param Thread thread to add
 * @return true if succesfully added
 * @return false if error
 */
int ThreadManager::CreateThread(void entrypoint())
{

    if (numThreads >= 256)
        return false;

    int i = 0;
    while (i <= 256)
    {
        if (Threads[i] == nullptr)
        {
            Thread *th = (Thread *)(stack[i] + (5012 - sizeof(Thread))); // init space for thread
            th->init(gdt, entrypoint);
            th->tid = i;
            Threads[numThreads] = th;
            Threads[numThreads]->cpustate->cs = gdt->CodeSegmentSelector();
            numThreads++;
            return th->tid;
        }

        i++;
    }

    return -1;
}

/**
 * @brief Schedules the next thread to be executed by checking its yieldsStatus.
 *
 * @param cpustate state
 * @return CPUState* thread to be executed state
 */
CPUState *ThreadManager::Schedule(CPUState *cpustate)
{
    if (numThreads <= 0)
        return cpustate;

    if (currentThread >= 0 && Threads[currentThread] != nullptr)
        Threads[currentThread++]->cpustate = cpustate;

    int i = currentThread;
    while (i < 256)
    {
        if (i >= 0 && Threads[i] != nullptr)
        {

            if (Threads[i]->yieldStatus)
                Threads[i]->yieldStatus = false;
            else
            {
                currentThread = i;
                return Threads[i]->cpustate;
            }
        }

        i++;
        if (i >= 256)
            i = 0;
    }

    return cpustate;
}

/**
 * @brief Terminates a thread by removing it from an array by making its pointer nullptr
 *
 * @param tid thread id to terminate
 * @return true if sucessfully terminated
 * @return false if error
 */
bool ThreadManager::TerminateThread(int tid)
{
    if (tid < 0 || tid >= 256)
        return false;
    if (Threads[tid] == nullptr)
        return false;

    Threads[tid] = nullptr;
    numThreads--;
    return true;
}

/**
 * @brief Joins a thread by waiting to finish
 *
 * @param other thread to join
 * @return true if succesfully joined
 * @return false if error
 */
bool ThreadManager::JoinThreads(int other)
{
    if (Threads[other] == nullptr)
        return false;
    if (Threads[other]->yieldStatus)
        return false;

    while (true)
    {
        if (Threads[other] == nullptr)
            break;
    }

    return true;
}

/**
 * @brief Makes yield status true of the current thread
 *
 */
void ThreadManager::YieldThreads(int tid)
{
    Threads[tid]->yieldStatus = true;
}
