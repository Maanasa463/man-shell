#ifndef _JOBS_H
#define _JOBS_H

// Struct which stores the name and pid of the background processes
typedef struct background back;

struct background
{
    char* name;
    int pid;
    int job_num;
    int run;
};

extern int fg_pid;
extern int shell;
extern back proc[100];
extern int p;
extern char fg_name[100];

void jobs(char* flag, back* process, int p);
void jobs_r(back* process, int p);
void jobs_s(back* process, int p);
void jobs_noflags(back* process, int p);

#endif