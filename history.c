#include <stdio.h>
#include "history.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Stores the given commands in history file
void store_in_file(char *command, char *hispath)
{
    char buff[100];
    char store[20];

    FILE *f = fopen(hispath, "a+");

    if (f == NULL)
        perror("History");

    int count = 0;

    // Counts number of lines in file currently present
    while (fgets(buff, 100, f) != NULL)
    {
        count++;
    }

    fclose(f);

    f = fopen(hispath, "a+");

    // Retrieve the last command present in the file
    while (fgets(buff, 100, f) != NULL)
    {
        strcpy(store, buff);
    }

    fclose(f);

    int i = 0;
    int flag = 0;

    int len1 = strlen(store);
    int len2 = strlen(command);

    // Comparison to check if the preceding command is same as the current command
    if (len1 != len2 + 1)
    {
        flag = 1;
    }

    if (flag == 0)
    {
        while (store[i] != '\n')
        {
            if (command[i] != store[i])
            {

                flag = 1;
                break;
            }

            i++;
        }
    }

    if (flag != 0)
    {
        // Number of commands in file < 20
        if (count < 20)
        {
            FILE *f = fopen(hispath, "a");
            fprintf(f, "%s\n", command);
            fclose(f);
        }

        // Copy commands into another file, leaving the first command
        // copy commands back into history file and append the last command
        // to maintain 20 commands in the file at once
        else
        {
            FILE *temp = fopen("temp.txt", "w");
            FILE *f = fopen(hispath, "a+");

            fgets(buff, 100, f);

            while (fgets(buff, 100, f) != NULL)
            {
                fprintf(temp, "%s", buff);
            }

            fclose(f);
            fclose(temp);

            temp = fopen("temp.txt", "a+");
            f = fopen(hispath, "w");

            while (fgets(buff, 100, temp) != NULL)
            {
                fprintf(f, "%s", buff);
            }

            fclose(f);
            fclose(temp);

            f = fopen(hispath, "a+");
            fprintf(f, "%s\n", command);
            fclose(f);

            remove("temp.txt");
        }
    }
    else
    {
        FILE *f = fopen(hispath, "a");
        fclose(f);
    }

    return;
}

void history(char *hispath, char *comm)
{
    int out = 0;
    int original = dup(STDOUT_FILENO);

    if (strcmp(comm, "\0") != 0)
    {
        char *token;
        char *path = (char*) malloc(sizeof(char) * 100);
        char delim[] = " \t";

        while (token = strtok_r(comm, delim, &comm))
        {
            if (strcmp(token, ">") == 0)
                out = 1;
            else if (strcmp(token, ">>") == 0)
                out = 2;
            else
            {
                strcpy(path, token);
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

    FILE *f = fopen(hispath, "r");

    if (f == NULL)
        perror("History ");

    int count = 0;
    char buff[100];

    while (fgets(buff, 100, f) != NULL)
    {
        count++;
    }

    fclose(f);

    f = fopen(hispath, "r");

    if (count <= 10)
    {
        while (fgets(buff, 100, f) != NULL)
        {
            printf("%s", buff);
        }
    }
    else
    {
        int i = 0;

        while (i < count - 10)
        {
            fgets(buff, 100, f);
            i++;
        }

        while (fgets(buff, 100, f) != NULL)
        {
            printf("%s", buff);
        }
    }

    fclose(f);
    
    dup2(original, STDOUT_FILENO);

    return;
}
