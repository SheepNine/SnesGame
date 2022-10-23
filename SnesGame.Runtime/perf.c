#include "perf.h"

struct Perf
{
    int category;
    Uint64 freqNs;
    Uint64 initTime;
    Uint64 lastTime;
} Perf;

hPerf creat_Perf(int category)
{
    hPerf result = (hPerf)SDL_malloc(sizeof(Perf));
    result->category = category;
    result->freqNs = SDL_GetPerformanceFrequency() / (1000 * 1000);
    result->lastTime = SDL_GetPerformanceCounter();
    result->initTime = result->lastTime;
    return result;
}

void destr_Perf(hPerf perf)
{
    SDL_LogDebug(perf->category, "\r\n");
    SDL_free(perf);
}

void logInterval_Perf(hPerf perf, char* label)
{
    Uint64 now = SDL_GetPerformanceCounter();
    SDL_LogDebug(perf->category, "%s: %llu ns (%llu ns elapsed)\r\n",
        label,
        (now - perf->lastTime) / perf->freqNs,
        (now - perf->initTime) / perf->freqNs);
    perf->lastTime = now;
}

void logTarget_Perf(hPerf perf, char* label, Uint64 targetNs)
{
    Uint64 elapsedNs = (SDL_GetPerformanceCounter() - perf->initTime) / perf->freqNs;
    if (elapsedNs <= targetNs)
    {
        return;
    }
    SDL_LogWarn(perf->category, "!!! Target for %s missed: %llu ns elapsed !!!\r\n",
        label, elapsedNs);
}