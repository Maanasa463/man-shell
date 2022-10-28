#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "jobs.h"

// extern int fg_pid;

void INT_handler(int sig)
{
    if (fg_pid != 0)
    {
        int *status;
        int ret = waitpid(fg_pid, status, WNOHANG);

        if (ret < 0)
        {
            fg_pid = 0;
        }

        else
        {
            kill(fg_pid, SIGINT);
            printf("\n");
            fg_pid = 0;
            return;
        }
    }

    printf("\n");
    return;
}

void STP_handler(int sig)
{
    // signal(SIGTSTP, SIG_IGN);

    if (fg_pid != 0)
    {
        kill(fg_pid, SIGTSTP);

        proc[p].name = (char *)malloc(sizeof(char) * 100);
        strcpy(proc[p].name, fg_name);

        proc[p].pid = fg_pid;
        proc[p].job_num = p + 1;
        proc[p].run = 0;

        p++;

        printf("\n");

        fg_pid = 0;
        return;
    }

    fflush(stdout);

    return;
}
