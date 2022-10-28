#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jobs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fg_pid = 0;
int shell = 0;
int p = 0;
char fg_name[100];
back proc[100];

// jobs || jobs -r -s || jobs -s -r
void jobs_noflag(back *process, int p)
{
    for (int i = 0; i < p; i++)
    {
        for (int j = i + 1; j < p; j++)
        {
            // Sort the background processes in alphabetical order
            if (strcasecmp(process[i].name, process[j].name) > 0)
            {
                char temp[100];
                int temp_pid;
                int temp_jobid;
                int temp_run;

                strcpy(temp, process[j].name);
                temp_pid = process[j].pid;
                temp_jobid = process[j].job_num;
                temp_run = process[j].run;

                strcpy(process[j].name, process[i].name);
                process[j].pid = process[i].pid;
                process[j].job_num = process[i].job_num;
                process[j].run = process[i].run;

                strcpy(process[i].name, temp);
                process[i].pid = temp_pid;
                process[i].job_num = temp_jobid;
                process[i].run = temp_run;
            }
        }
    }

    // Print running as well as stopped background processes
    for (int i = 0; i < p; i++)
    {
        if (proc[i].run == 1 || proc[i].run == -1)
            continue;

        char proc_id[100];
        sprintf(proc_id, "%d", process[i].pid);

        char proc[100];
        strcpy(proc, "/proc/");
        strcat(proc, proc_id);
        strcat(proc, "/stat");

        FILE *f = fopen(proc, "r");
        char read[10000];
        char state[1000];

        int n = 0;

        while (fscanf(f, "%s", read) != EOF)
        {
            if (n == 2)
            {
                strcpy(state, read);
                break;
            }
            n++;
        }

        if (strcmp(state, "S") == 0 || strcmp(state, "R") == 0)
            printf("[ %d ] Running %s [ %d ]\n", process[i].job_num, process[i].name, process[i].pid);
        if (strcmp(state, "T") == 0)
            printf("[ %d ] Stopped %s [ %d ]\n", process[i].job_num, process[i].name, process[i].pid);
    }
}

// jobs -r
void jobs_r(back *process, int p)
{
    for (int i = 0; i < p; i++)
    {
        for (int j = i + 1; j < p; j++)
        {
            // Sort the background processes in alphabetical order
            if (strcasecmp(process[i].name, process[j].name) > 0)
            {
                char temp[100];
                int temp_pid;
                int temp_jobid;
                int temp_run;

                strcpy(temp, process[j].name);
                temp_pid = process[j].pid;
                temp_jobid = process[j].job_num;
                temp_run = process[j].run;

                strcpy(process[j].name, process[i].name);
                process[j].pid = process[i].pid;
                process[j].job_num = process[i].job_num;
                process[j].run = process[i].run;

                strcpy(process[i].name, temp);
                process[i].pid = temp_pid;
                process[i].job_num = temp_jobid;
                process[i].run = temp_run;
            }
        }
    }

    // Print only Running background processes
    for (int i = 0; i < p; i++)
    {
        if (proc[i].run == 1 || proc[i].run == -1)
            continue;

        char proc_id[100];
        sprintf(proc_id, "%d", process[i].pid);

        char proc[100];
        strcpy(proc, "/proc/");
        strcat(proc, proc_id);
        strcat(proc, "/stat");

        FILE *f = fopen(proc, "r");
        char read[10000];
        char state[1000];

        int n = 0;

        while (fscanf(f, "%s", read) != EOF)
        {
            if (n == 2)
            {
                strcpy(state, read);
                break;
            }
            n++;
        }

        if (strcmp(state, "S") == 0 || strcmp(state, "R") == 0)
            printf("[ %d ] Running %s [ %d ]\n", process[i].job_num, process[i].name, process[i].pid);
    }
}

// jobs -s
void jobs_s(back *process, int p)
{
    for (int i = 0; i < p; i++)
    {
        for (int j = i + 1; j < p; j++)
        {
            // Sort the background processes in alphabetical order
            if (strcasecmp(process[i].name, process[j].name) > 0)
            {
                char temp[100];
                int temp_pid;
                int temp_jobid;
                int temp_run;

                strcpy(temp, process[j].name);
                temp_pid = process[j].pid;
                temp_jobid = process[j].job_num;
                temp_run = process[j].run;

                strcpy(process[j].name, process[i].name);
                process[j].pid = process[i].pid;
                process[j].job_num = process[i].job_num;
                process[j].run = process[i].run;

                strcpy(process[i].name, temp);
                process[i].pid = temp_pid;
                process[i].job_num = temp_jobid;
                process[i].run = temp_run;
            }
        }
    }

    // Print only Stopped background processes
    for (int i = 0; i < p; i++)
    {
        if (proc[i].run == 1 || proc[i].run == -1)
            continue;

        char proc_id[100];
        sprintf(proc_id, "%d", process[i].pid);

        char proc[100];
        strcpy(proc, "/proc/");
        strcat(proc, proc_id);
        strcat(proc, "/stat");

        FILE *f = fopen(proc, "r");
        char read[10000];
        char state[1000];

        int n = 0;

        while (fscanf(f, "%s", read) != EOF)
        {
            if (n == 2)
            {
                strcpy(state, read);
                break;
            }
            n++;
        }

        if (strcmp(state, "R") == 0)
            printf("[ %d ] Stopped %s [ %d ]\n", process[i].job_num, process[i].name, process[i].pid);
    }
}

// Caller function
void jobs(char *flag, back *process, int p)
{
    int original = dup(STDOUT_FILENO);

    if (strcmp(flag, "\0") == 0)
    {
        jobs_noflag(process, p);
    }

    if (strcmp(flag, "\0") != 0)
    {
        char *token;
        char *temp = flag;
        char delim[] = " \t";
        int flagr = 0;
        int flags = 0;

        int out = 0;
        int outflag = 0;
        char *path = (char *)malloc(sizeof(char) * 100);

        while (token = strtok_r(temp, delim, &temp))
        {
            if (strcmp(token, ">") == 0)
            {
                out = 1;
                continue;
            }

            if (strcmp(token, ">>") == 0)
            {
                out = 2;
                continue;
            }

            if ((out == 1 || out == 2) && outflag == 0)
            {
                strcpy(path, token);
                outflag = 1;
            }

            if (strcmp(token, "-r") == 0)
                flagr = 1;

            if (strcmp(token, "-s") == 0)
                flags = 1;
        }

        // printf("%d\n", out);
        // printf("%s\n", path);

        if (out == 1)
        {
            int fp = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            dup2(fp, STDOUT_FILENO);
            close(fp);
        }

        if (out == 2)
        {
            int fp = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
            dup2(fp, STDOUT_FILENO);
            close(fp);
        }

        if ((flagr == 1 && flags == 1) || (flagr == 0 && flags == 0))
        {
            jobs_noflag(process, p);
            dup2(original, STDOUT_FILENO);
            return;
        }

        if (flagr == 1)
            jobs_r(process, p);

        if (flags == 1)
            jobs_s(process, p);
    }

    dup2(original, STDOUT_FILENO);
    return;
}