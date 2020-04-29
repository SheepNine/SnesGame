#pragma once

#include "SDL.h"

typedef struct Perf* hPerf;

hPerf creat_Perf();
void destr_Perf(hPerf perf);

void logInterval_Perf(hPerf perf, char* label);
void logTarget_Perf(hPerf perf, char* label, Uint64 targetNs);
