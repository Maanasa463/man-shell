#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include "process.h"
#include <time.h>
#include "jobs.h"
#include <fcntl.h>

void child_handler(back *proc, int p)
{
    // Checks the status of currently running background processes
    int status;
    int child = 1;

    while (child > 0)
    {
        child = waitpid(-1, &status, WNOHANG | WUNTRACED);

        for (int k = 0; k < p; k++)
        {
            if (child == proc[k].pid)
            {
                if (WIFEXITED(status))
                {
                    printf("%s with pid %d exitted normally \n", proc[k].name, proc[k].pid);
                    proc[k].run = 1;
                }

                else
                {
                    printf("%s with pid %d exitted abnormally \n", proc[k].name, proc[k].pid);
                    if (proc[k].run != -1)
                        proc[k].run = 2;
                }
            }
        }
    }
}

void handle(char **arg, char *comm, char *token)
{
    char *temp;
    char *to;
    char *input = (char *)malloc(sizeof(char) * 100);
    char *output = (char *)malloc(sizeof(char) * 100);

    int in = 0, out = 0;
    int inflag = 0, outflag = 0;
    char *operation = (char *)malloc(sizeof(char) * 100);
    strcpy(operation, "");

    if (comm != NULL)
    {
        while (to = strtok_r(comm, " \t", &comm))
        {
            if (strcmp(to, "<") == 0)
                in = 1;

            else if (in == 1 && inflag == 0)
            {
                strcpy(input, to);
                inflag = 1;
            }

            else if (strcmp(to, ">") == 0)
                out = 1;

            else if (strcmp(to, ">>") == 0)
                out = 2;

            else if (out != 0 && outflag == 0)
            {
                strcpy(output, to);
                outflag = 1;
            }

            else
            {
                strcat(operation, to);
                strcat(operation, " ");
            }

            temp = comm;
        }
    }

    arg[0] = token;

    int i = 1;

    if (in != 0)
    {
        if (strcmp(operation, "\0") != 0)
        {
            while (to = strtok_r(operation, " \t", &operation))
            {
                arg[i] = to;
                i++;
            }
        }

        arg[i] = input;
        i++;
    }

    else
    {
        if (strcmp(operation, "\0") != 0)
        {
            while (to = strtok_r(operation, " \t", &operation))
            {
                arg[i] = to;
                i++;
            }
        }
    }

    arg[i + 1] = NULL;

    if (out == 1)
    {
        int fp = open(output, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        dup2(fp, STDOUT_FILENO);
        close(fp);
    }

    if (out == 2)
    {
        int fp = open(output, O_CREAT | O_WRONLY | O_APPEND, 0644);
        dup2(fp, STDOUT_FILENO);
        close(fp);
    }
}

// If the process is a foreground process
void fore_process(char **command, int *rtime)
{
    int *status;
    int pid = fork();

    // Start time of child execution
    time_t start = time(NULL);

    if (pid != 0)
    {
        fg_pid = pid;
    }

    if (pid == 0)
    {
        setpgid(0, 0);
        int stat = execvp(*command, command);

        if (stat == -1)
            printf("Error: No such command found.\n");

        return;
    }

    // Parent waits for child to finish
    else
        pid = wait(status);

    // End time of child execution
    time_t end = time(NULL);

    // Runtime
    *rtime = end - start;

    return;
}

// Background process
void back_process(char **command, int *pids)
{
    int *status;
    int pid = fork();

    if (pid != 0)
    {
        *pids = pid;
    }

    if (pid == 0)
    {
        setpgid(0, 0);
        int stat = execvp(*command, command);

        if (stat == -1)
            printf("Error: No such command found.\n");

        return;
    }

    // Parent does not wait

    return;
}