#ifndef PROCESS_H
#define PROCESS_H
#include "jobs.h"

void child_handler(back* proc, int p);
void fore_process (char** command, int* rtime);
void back_process (char** command, int* pid);
void handle (char** arg, char* comm, char* token);

#endif

