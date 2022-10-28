#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "discover.h"
#include <stdlib.h>

// No -d or -f flags given
int discover_noflags(char *path, char *print, char *file)
{
    DIR *dir;
    struct dirent *content;
    dir = opendir(path);

    // if given directory does not exist
    if (dir == NULL)
    {
        perror("Directory/File");
        return -1;
    }

    struct stat type;

    // Reading the contents of the directory
    while ((content = readdir(dir)) != NULL)
    {
        // If it's  a hidden file
        if (content->d_name[0] == '.')
            continue;

        // Generates absolute path of current file/ directory
        char temp[100];
        strcpy(temp, path);
        strcat(temp, "/");
        strcat(temp, content->d_name);

        stat(temp, &type);

        // Generates the relative path required to print on screen
        char temp2[100];
        strcpy(temp2, print);
        strcat(temp2, "/");
        strcat(temp2, content->d_name);

        // Only print if the given file is found, else recursively
        // search through directories
        if (strcmp(file, "\0") != 0)
        {
            if (strcmp(content->d_name, file) == 0)
            {
                printf("%s\n", temp2);
            }

            if (S_ISDIR(type.st_mode) != 0)
            {
                discover_noflags(temp, temp2, file);
            }
        }

        // If no file/directory needs to be searched for, recursively call
        // function and print all files and subdirectories
        else
        {
            if (S_ISDIR(type.st_mode) != 0)
            {
                printf("%s\n", temp2);
                discover_noflags(temp, temp2, file);
            }

            else
            {
                printf("%s\n", temp2);
            }
        }
    }

    closedir(dir);

    return 0;
}

// discover with -d flag
int discover_d(char *path, char *file, char *print)
{
    DIR *dir;
    struct dirent *content;
    dir = opendir(path);

    if (dir == NULL)
    {
        perror("Directory/File");
        return -1;
    }

    struct stat type;

    while ((content = readdir(dir)) != NULL)
    {
        if (content->d_name[0] == '.')
            continue;

        char temp[100];
        strcpy(temp, path);
        strcat(temp, "/");
        strcat(temp, content->d_name);

        stat(temp, &type);

        if (strcmp(file, "\0") != 0)
        {
            char temp2[100];
            strcpy(temp2, print);
            strcat(temp2, "/");
            strcat(temp2, content->d_name);

            // If the name matches with <file_name> to be searched for but
            // Only if it is a directory
            if (strcmp(content->d_name, file) == 0 && S_ISDIR(type.st_mode) != 0)
            {
                printf("%s\n", temp2);
            }

            if (S_ISDIR(type.st_mode) != 0)
            {
                discover_d(temp, file, temp2);
            }
        }

        if (strcmp(file, "\0") == 0)
        {
            if (S_ISDIR(type.st_mode) != 0)
            {
                char temp2[100];
                strcpy(temp2, print);
                strcat(temp2, "/");
                strcat(temp2, content->d_name);

                discover_d(temp, file, temp2);
                printf("%s\n", temp2);
            }
        }
    }

    closedir(dir);

    return 0;
}

// Discover with -f flag
int discover_f(char *path, char *file, char *print)
{
    DIR *dir;
    struct dirent *content;
    dir = opendir(path);

    if (dir == NULL)
    {
        perror("Directory/File");
        return -1;
    }

    struct stat type;

    while ((content = readdir(dir)) != NULL)
    {
        if (content->d_name[0] == '.')
            continue;

        char temp[100];
        strcpy(temp, path);
        strcat(temp, "/");
        strcat(temp, content->d_name);

        stat(temp, &type);

        char temp2[100];
        strcpy(temp2, print);
        strcat(temp2, "/");
        strcat(temp2, content->d_name);

        if (strcmp(file, "\0") != 0)
        {
            // <file_name> is found and printed only if it is a file and not a
            // directory
            if (strcmp(content->d_name, file) == 0 && S_ISDIR(type.st_mode) == 0)
            {
                printf("%s\n", temp2);
            }

            if (S_ISDIR(type.st_mode) != 0)
            {
                discover_f(temp, file, temp2);
            }
        }

        if (strcmp(file, "\0") == 0)
        {
            if (S_ISDIR(type.st_mode) != 0)
            {
                discover_f(temp, file, temp2);
            }

            else
            {
                printf("%s\n", temp2);
            }
        }
    }

    closedir(dir);

    return 0;
}

// Discover with -d and -f flags
// Behaves similarly to discover with no flags
int discover_df(char *path, char *file, char *print)
{
    DIR *dir;
    struct dirent *content;
    dir = opendir(path);

    if (dir == NULL)
    {
        perror("Directory/File");
        return -1;
    }

    struct stat type;

    while ((content = readdir(dir)) != NULL)
    {
        if (content->d_name[0] == '.')
            continue;

        char temp[100];
        strcpy(temp, path);
        strcat(temp, "/");
        strcat(temp, content->d_name);

        char temp2[100];
        strcpy(temp2, print);
        strcat(temp2, "/");
        strcat(temp2, content->d_name);

        if (strcmp(file, "\0") != 0)
        {

            if (strcmp(content->d_name, file) == 0)
            {
                printf("%s\n", temp2);
            }

            stat(temp, &type);

            if (S_ISDIR(type.st_mode) != 0)
            {
                discover_df(temp, file, temp2);
            }
        }

        else
        {
            stat(temp, &type);

            if (S_ISDIR(type.st_mode) != 0)
            {
                printf("%s\n", temp2);
                discover_df(temp, file, temp2);
            }

            else
            {
                printf("%s\n", temp2);
            }
        }
    }

    closedir(dir);

    return 0;
}

// Calling function
int discover(char *path, char *comm, char *home)
{
    int flagd = 0;
    int flagf = 0;
    char directory[100] = "\0";
    char file[100] = "\0";
    char print[100];
    strcpy(print, ".");

    int original = dup(STDOUT_FILENO);

    if (strcmp(comm, "\0") == 0)
    {
        printf(".\n");
        discover_noflags(path, print, file);
    }

    if (strcmp(comm, "\0") != 0)
    {
        char *token;
        char *flags = comm;
        char *temp = comm;
        char delim[] = " \t";

        char *printpath = (char *)malloc(sizeof(char) * 100);
        int out = 0;
        int flag = 0;

        // Tokenised to remove extra white spaces
        while (token = strtok_r(temp, delim, &temp))
        {
            if (strcmp(token, "-d") == 0)
                flagd = 1;

            if (strcmp(token, "-f") == 0)
                flagf = 1;

            if (token[0] == '"')
            {
                int length = strlen(token);
                token[length - 1] = '\0';
                strcpy(file, &token[1]);
                continue;
            }

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

            if (out != 0 && flag == 0)
            {
                strcpy(printpath, token);
                flag = 1;
                continue;
            }

            if (token[0] != '-')
            {
                strcpy(directory, token);
                continue;
            }

            strcpy(flags, token);
        }

        if (out == 1)
        {
            int fp = open(printpath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            dup2(fp, STDOUT_FILENO);
            close(fp);
        }

        if (out == 2)
        {
            int fp = open(printpath, O_CREAT | O_WRONLY | O_APPEND, 0644);
            dup2(fp, STDOUT_FILENO);
            close(fp);
        }

        if (strcmp(flags, "-d") == 0)
        {

            if (strcmp(directory, "\0") == 0)
            {
                if (strcmp(file, "\0") == 0)
                    printf(".\n");

                if (flagf == 1)
                {
                    discover_df(path, file, print);
                    dup2(original, STDOUT_FILENO);
                    return 0;
                }

                discover_d(path, file, print);
                dup2(original, STDOUT_FILENO);
                return 0;
            }

            else
            {

                if (strcmp(directory, "~") == 0)
                {
                    strcpy(directory, home);
                }

                else
                {
                    DIR *dir;
                    dir = opendir(directory);

                    if (dir == NULL)
                    {
                        dup2(original, STDOUT_FILENO);
                        perror("Directory/File: ");
                        closedir(dir);
                        return -1;
                    }

                    if (strcmp(file, "\0") == 0)
                        printf(".\n");

                    char buff[100];
                    getcwd(buff, 100);

                    int len = strlen(buff);

                    char temp[100];
                    strcpy(temp, directory);

                    if (strncmp(buff, directory, len) != 0)
                    {
                        strcat(buff, "/");
                        strcat(buff, directory);
                        strcpy(directory, buff);

                        // strcat(print, "/");
                        strcpy(print, temp);
                    }
                }

                strcpy(path, directory);

                if (flagf == 1)
                {
                    discover_df(path, file, print);
                    dup2(original, STDOUT_FILENO);
                    return 0;
                }

                discover_d(path, file, print);
                dup2(original, STDOUT_FILENO);
                return 0;
            }
        }

        if (strcmp(flags, "-f") == 0)
        {
            if (strcmp(directory, "\0") == 0)
            {
                if (flagd == 1)
                {
                    if (strcmp(file, "\0") == 0)
                        printf(".\n");

                    discover_df(path, file, print);
                    dup2(original, STDOUT_FILENO);
                    return 0;
                }

                discover_f(path, file, print);
                dup2(original, STDOUT_FILENO);
                return 0;
            }

            else
            {

                if (strcmp(directory, "~") == 0)
                {
                    strcpy(directory, home);
                }

                else
                {
                    char buff[100];
                    getcwd(buff, 100);

                    int len = strlen(buff);

                    char temp[100];
                    strcpy(temp, directory);

                    DIR *dir;
                    dir = opendir(directory);

                    if (dir == NULL)
                    {
                        dup2(original, STDOUT_FILENO);
                        perror("Directory/File");
                        closedir(dir);
                        return -1;
                    }

                    if (strncmp(buff, directory, len) != 0)
                    {
                        strcat(buff, "/");
                        strcat(buff, directory);
                        strcpy(directory, buff);

                        // strcat(print, "/");
                        strcpy(print, temp);
                    }
                }

                strcpy(path, directory);

                if (flagd == 1)
                {
                    if (strcmp(file, "\0") == 0)
                        printf(".\n");

                    discover_df(path, file, print);
                    dup2(original, STDOUT_FILENO);
                    return 0;
                }

                discover_f(path, file, print);
                dup2(original, STDOUT_FILENO);
                return 0;
            }
        }

        else
        {
            if (strcmp(directory, "\0") == 0)
            {
                if (strcmp(file, "\0") == 0)
                    printf(".\n");

                discover_noflags(path, print, file);
                dup2(original, STDOUT_FILENO);
                return 0;
            }

            else
            {

                if (strcmp(directory, "~") == 0)
                {
                    strcpy(directory, home);
                }

                else
                {
                    char buff[100];
                    getcwd(buff, 100);

                    int len = strlen(buff);

                    char temp[100];
                    strcpy(temp, directory);

                    DIR *dir;
                    dir = opendir(directory);

                    if (dir == NULL)
                    {
                        dup2(original, STDOUT_FILENO);
                        perror("Directory/File");
                        closedir(dir);
                        return -1;
                    }

                    if (strcmp(file, "\0") == 0)
                        printf(".\n");

                    if (strncmp(buff, directory, len) != 0)
                    {
                        strcat(buff, "/");
                        strcat(buff, directory);
                        strcpy(directory, buff);

                        strcpy(print, temp);
                    }
                }

                strcpy(path, directory);

                discover_noflags(path, print, file);
                dup2(original, STDOUT_FILENO);
                return 0;
            }
        }
    }

    dup2(original, STDOUT_FILENO);
    return 0;
}