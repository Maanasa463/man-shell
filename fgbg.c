#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "fgbg.h"
#include <errno.h>

void fg(char *comm, int p, back *process)
{
    // Error handling for number of arguments
    if (strcmp(comm, "\0") == 0)
    {
        printf("Too less arguments in function call.\n");
        return;
    }

    char *num;
    num = strtok_r(comm, " \t", &comm);

    if (strcmp(comm, "\0") != 0)
    {
        printf("Too many arguments in function call.\n");
        return;
    }

    int job_id;
    job_id = atoi(num);

    if (job_id > p)
    {
        printf("No job with job number %d found\n", job_id);
        return;
    }

    int gpid = getpid();

    for (int i = 0; i < p; i++)
    {
        if (process[i].job_num == job_id && process[i].run != 1)
        {
            // error handling for terminal signal handlers
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            // Setting the foreground process group to specified group id
            int pg_set = tcsetpgrp(0, getpgid(process[i].pid));

            if (pg_set < 0)
            {
                perror("tcsetpgrp");
            }

            // Ask the child to resume execution
            int ret = kill(process[i].pid, SIGCONT);

            if (ret < 0)
            {
                perror("Kill");
            }

            // Parent waits for child to finish execution
            int status;
            waitpid(process[i].pid, &status, WUNTRACED);

            // Return Foreground process group to that of the shell
            signal(SIGTTOU, SIG_IGN);
            pg_set = tcsetpgrp(0, getpgid(0));

            if (pg_set < 0)
            {
                perror("tcsetpgrp");
            }

            // error handlin for terminal signal handlers
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if (WIFSTOPPED(status))
                process[i].run = 2;
            else
                process[i].run = -1;

            return;
        }
    }

    return;
}

void bg(char *comm, int p, back *process)
{
    // Error handling for number of arguments
    if (strcmp(comm, "\0") == 0)
    {
        printf("Too less arguments in function call.\n");
        return;
    }

    char *num;
    num = strtok_r(comm, " \t", &comm);

    if (strcmp(comm, "\0") != 0)
    {
        printf("Too many arguments in function call.\n");
        return;
    }

    int job_id;
    job_id = atoi(num);

    if (job_id > p)
    {
        printf("No job with job number %d found\n", job_id);
        return;
    }

    for (int i = 0; i < p; i++)
    {
        // Process already in the background
        if (process[i].run == 0)
        {
            return;
        }

        if (process[i].job_num == job_id && (process[i].run == 2 || process[i].run == -1))
        {
            // Restart the process in the background
            kill(process[i].pid, SIGCONT);

            process[i].run = 0;
            return;
        }
    }

    return;
}