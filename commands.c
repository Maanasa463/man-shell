#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "commands.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Fetches absolute path (from root of system) of current working directory
int pwd(char *path)
{
    char dir[1000];
    getcwd(dir, 1000);
    int out = 0;

    char *token;
    char delim[] = " \t";

    if (strcmp(path, "\0") != 0)
    {
        token = strtok_r(path, delim, &path);

        if (strcmp(token, ">") == 0)
            out = 1;
        if (strcmp(token, ">>") == 0)
            out = 2;
    }

    if (out == 1)
    {
        int fp = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);

        int original;
        dup2(STDOUT_FILENO, original);

        dup2(fp, STDOUT_FILENO);
        close(fp);

        printf("%s\n", dir);

        dup2(original, STDOUT_FILENO);
    }

    else if (out == 2)
    {
        int fp = open(path, O_CREAT | O_APPEND | O_WRONLY, 0644);

        int original;
        dup2(STDOUT_FILENO, original);

        dup2(fp, STDOUT_FILENO);
        close(fp);

        printf("%s\n", dir);

        dup2(original, STDOUT_FILENO);
    }

    else
    {
        printf("%s\n", dir);
    }

    return 0;
}

// Echoes the command passed as argument
int echo(char *comm)
{
    char *token;
    char delim[] = " \t";

    char *path = (char *)malloc(sizeof(char) * 50);
    char *print = (char *)malloc(sizeof(char) * 50);
    strcpy(print, "");
    strcpy(path, "");

    int out = 0;

    while (token = strtok_r(comm, delim, &comm))
    {
        if (strcmp(token, ">") == 0)
            out = 1;
        else if (strcmp(token, ">>") == 0)
            out = 2;
        else if (out == 0)
        {
            strcat(print, token);
            strcat(print, " ");
        }
        else
        {
            strcat(path, token);
        }
    }

    if (out == 1)
    {
        int fp = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);

        int original = dup(STDOUT_FILENO);
        dup2(fp, STDOUT_FILENO);
        close(fp);

        printf("%s\n", print);

        dup2(original, STDOUT_FILENO);
    }

    else if (out == 2)
    {
        int fp = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);

        int original = dup(STDOUT_FILENO);
        dup2(fp, STDOUT_FILENO);
        close(fp);

        printf("%s\n", print);

        dup2(original, STDOUT_FILENO);
    }

    else
    {
        printf("%s\n", print);
    }

    return 0;
}

// Changes directory according to given argument
int cd(char **comm, char *save, char **prev, char **prev2)
{
    char *token;
    char *parsed;
    char *temp = *comm;
    char delim[] = " \t";

    while (token = strtok_r(temp, delim, &temp))
    {
        parsed = token;
    }

    *comm = parsed;

    if (strcmp(*comm, ".") == 0)
    {
        getcwd(*prev, 100);
        chdir(".");
    }
    else if (strcmp(*comm, "..") == 0)
    {
        getcwd(*prev, 100);
        chdir("..");
    }
    else if (strcmp(*comm, "\0") == 0 || strcmp(*comm, "~") == 0)
    {
        getcwd(*prev, 100);
        chdir(save);
    }
    else if (strcmp(*comm, "-") == 0)
    {
        getcwd(*prev2, 100);
        printf("%s\n", *prev);
        chdir(*prev);
        strcpy(*prev, *prev2);
    }
    else
    {
        if (*comm[0] == '~')
        {
            token = strtok_r(*comm, "~/", &*comm);
            *comm = token;
        }

        int dire = open(*comm, O_DIRECTORY, O_RDONLY);

        if (dire >= 0)
        {
            getcwd(*prev, 100);
            chdir(*comm);
        }
        else
        {
            perror("Directory");
            return -1;
        }
    }

    return 0;
}