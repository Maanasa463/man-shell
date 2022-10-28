#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pinfo.h"
#include <unistd.h>
#include <fcntl.h>

// Open /proc/[pid]/stat file which contains status info of 
// process with process id [pid]
void pinfo(char *pid, char *home)
{   
    char* pid2 = (char*) malloc(sizeof(char) * 100);
    strcpy(pid2, pid);
    int original = dup(STDOUT_FILENO);
    
    if (strcmp(pid, "\0") != 0)
    {
        char* token;
        char delim[] = " \t";

        int out = 0;
        int flag = 0;
        char* path = (char*) malloc(sizeof(char) * 100);

        strcpy(pid2, "\0");

        while(token = strtok_r(pid, delim, &pid))
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

            if (out == 0 || flag == 1)
            {
                strcpy(pid2, token);
            }
            
            if (out != 0 && flag == 0)
            {
                strcpy(path, token);
                flag = 1;
            }

        }

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
    }


    // pinfo of the currently running shell
    if (strcmp(pid2, "\0") == 0)
    {
        int p = getpid();

        char proc_id[100];
        sprintf(proc_id, "%d", p);

        char proc[100];
        strcpy(proc, "/proc/");
        strcat(proc, proc_id);
        strcat(proc, "/stat");

        FILE *f = fopen(proc, "r");
        char read[10000];

        char proc_pid[100];
        char state[100];
        char mem[100];
        char path[100];
        char pgrp[100];
        char tpgid[100];

        int n = 0;

        while (fscanf(f, "%s", read) != EOF)
        {
            if (n == 0)
                strcpy(proc_pid, read);

            if (n == 2)
                strcpy(state, read);

            if (n == 4)
                strcpy(pgrp, read);

            if (n == 7)
                strcpy(tpgid, read);

            if (n == 22)
                strcpy(mem, read);

            n++;
        }

        char process[100];
        strcpy(process, "/proc/");
        strcat(process, proc_id);
        strcat(process, "/exe");

        readlink(process, path, 100);

        printf("pid : %s\n", proc_pid);
        printf("Process Status : %s+\n", state);
        printf("Memory : %s {Virtual Memory}\n", mem);
        printf("Executable Path : %s\n", path);
    }

    // pinfo of a given process id
    else
    {
        char proc[100];
        strcpy(proc, "/proc/");
        strcat(proc, pid2);
        strcat(proc, "/stat");

        FILE *f = fopen(proc, "r");

        if (f == NULL)
        {
            perror("Process");
            return;
        }

        char read[10000];

        char proc_pid[100];
        char state[100];
        char mem[100];
        char path[100];
        char pgrp[10000];
        char tpgid[10000];

        int n = 0;

        while (fscanf(f, "%s", read) != EOF)
        {
            if (n == 0)
                strcpy(proc_pid, read);

            if (n == 2)
                strcpy(state, read);

            if (n == 4)
                strcpy(pgrp, read);

            if (n == 7)
                strcpy(tpgid, read);

            if (n == 22)
                strcpy(mem, read);

            n++;
        }

        char process[100];
        strcpy(process, "/proc/");
        strcat(process, pid2);
        strcat(process, "/exe");

        readlink(process, path, 100);

        if (strcmp(pgrp, tpgid) == 0)
        {
            printf("pid : %s\n", proc_pid);
            printf("Process Status : %s+\n", state);
            printf("Memory : %s {Virtual Memory}\n", mem);
            printf("Executable Path : %s\n", path);
        }

        else
        {
            printf("pid : %s\n", proc_pid);
            printf("Process Status : %s\n", state);
            printf("Memory : %s {Virtual Memory}\n", mem);
            printf("Executable Path : %s\n", path);

        }
    }

    dup2(original, STDOUT_FILENO);
    return;
}
