#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sig.h"
#include <signal.h>

void sig (char * comm, int p, back* process)
{
    // Error handling for number of command line arguments
    if (strcmp(comm, "\0") == 0)
    {
        printf("Too few arguments than required\n");
        return;
    }

    char *token;
    char *job_id = "\0";
    char *sig_num = "\0";

    job_id = strtok_r(comm, " \t", &comm);

    if (strcmp(comm, "\0") == 0)
    {
        printf("Too few arguments than required\n");
        return;
    }

    sig_num = strtok_r(comm, " \t", &comm);

    if (strcmp(comm, "\0") != 0)
    {
        printf("Too many arguments than required\n");
        return;
    }

    int job;
    job = atoi(job_id);

    int sig;
    sig = atoi(sig_num);

    if (job > p)
    {
        printf("No job with job number %d found\n", job);
        return;
    }

    for (int i = 0; i < p; i++)
    {
        // Send signal specified by sig to background process specified by job_id
        if (process[i].job_num == job && process[i].run != 1)
        {
            int ret = kill(process[i].pid, sig);

            if (ret < 0)
            {
                perror("Kill");
            }

            process[i].run = -1;

            return;
        }
    }

    return;
}