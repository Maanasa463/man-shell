#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>

// no flags
int ls_noflag(char *path, int out)
{
    DIR *dir;
    struct dirent *content;
    dir = opendir(path);

    if (dir == NULL)
    {
        perror("Directory/File ");
        return -1;
    }

    struct stat type;

    char sorted[100][100];

    int n = 0;

    while ((content = readdir(dir)) != NULL)
    {
        if (content->d_name[0] == '.')
            continue;

        strcpy(sorted[n], content->d_name);
        n++;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (strcasecmp(sorted[i], sorted[j]) > 0)
            {
                char temp[100];
                strcpy(temp, sorted[j]);
                strcpy(sorted[j], sorted[i]);
                strcpy(sorted[i], temp);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        char temp[100];
        strcpy(temp, path);
        strcat(temp, "/");
        strcat(temp, sorted[i]);

        stat(temp, &type);

        if (S_ISDIR(type.st_mode) != 0)
        {
            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;34m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }

        else if (type.st_mode & S_IEXEC)
        {
            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;32m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }

        else
        {
            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;37m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }
    }

    closedir(dir);

    return 0;
}

// ls -a
int ls_a(char *path, int out)
{
    DIR *dir;
    struct dirent *content;
    dir = opendir(path);

    if (dir == NULL)
    {
        perror("Directory/File ");
        return -1;
    }

    struct stat type;

    char sorted[100][100];

    int n = 0;

    while ((content = readdir(dir)) != NULL)
    {
        strcpy(sorted[n], content->d_name);
        n++;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (strcasecmp(sorted[i], sorted[j]) > 0)
            {
                char temp[100];
                strcpy(temp, sorted[j]);
                strcpy(sorted[j], sorted[i]);
                strcpy(sorted[i], temp);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        char temp[100];
        strcpy(temp, path);
        strcat(temp, "/");
        strcat(temp, sorted[i]);

        stat(temp, &type);

        if (S_ISDIR(type.st_mode) != 0)
        {
            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;34m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }

        else if (type.st_mode & S_IEXEC)
        {
            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;32m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }

        else
        {
            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;37m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }
    }

    closedir(dir);

    return 0;
}

// ls -l
int ls_l(char *path, int out)
{
    DIR *dir;
    struct dirent *content;
    dir = opendir(path);

    if (dir == NULL)
    {
        perror("Directory/File ");
        return -1;
    }

    struct stat type;

    char sorted[100][100];

    int n = 0;

    while ((content = readdir(dir)) != NULL)
    {
        if (content->d_name[0] == '.')
            continue;
        strcpy(sorted[n], content->d_name);
        n++;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (strcasecmp(sorted[i], sorted[j]) > 0)
            {
                char temp[100];
                strcpy(temp, sorted[j]);
                strcpy(sorted[j], sorted[i]);
                strcpy(sorted[i], temp);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        char temp[100];
        strcpy(temp, path);
        strcat(temp, "/");
        strcat(temp, sorted[i]);

        stat(temp, &type);

        if (S_ISDIR(type.st_mode))
        {
            printf("d");
            // read for user
            if (type.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");

            // For group
            // Read
            if (type.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");

            // For others
            // Read
            if (type.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXOTH)
                printf("x ");
            else
                printf("- ");

            printf("%ld ", type.st_nlink);

            struct passwd *name = getpwuid(type.st_uid);
            printf("%s ", name->pw_name);

            struct group *group = getgrgid(type.st_gid);
            printf("%s ", group->gr_name);

            printf("%6ld ", type.st_size);

            char *modtime = ctime(&type.st_mtime);
            modtime[strlen(modtime) - 9] = '\0';
            printf("%s ", &modtime[4]);

            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;34m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }

        else if (type.st_mode & S_IEXEC)
        {
            printf("-");
            // read for user
            if (type.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");

            // For group
            // Read
            if (type.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");

            // For others
            // Read
            if (type.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXOTH)
                printf("x ");
            else
                printf("- ");

            printf("%ld ", type.st_nlink);

            struct passwd *name = getpwuid(type.st_uid);
            printf("%s ", name->pw_name);

            struct group *group = getgrgid(type.st_gid);
            printf("%s ", group->gr_name);

            printf("%6ld ", type.st_size);

            char *modtime = ctime(&type.st_mtime);
            modtime[strlen(modtime) - 9] = '\0';
            printf("%s ", &modtime[4]);

            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;32m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }

        else
        {
            printf("-");
            // read for user
            if (type.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");

            // For group
            // Read
            if (type.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");

            // For others
            // Read
            if (type.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXOTH)
                printf("x ");
            else
                printf("- ");

            printf("%ld ", type.st_nlink);

            struct passwd *name = getpwuid(type.st_uid);
            printf("%s ", name->pw_name);

            struct group *group = getgrgid(type.st_gid);
            printf("%s ", group->gr_name);

            printf("%6ld ", type.st_size);

            char *modtime = ctime(&type.st_mtime);
            modtime[strlen(modtime) - 9] = '\0';
            printf("%s ", &modtime[4]);

            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;37m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }
    }

    closedir(dir);

    return 0;
}

// ls -la / ls -al / ls -a -l / ls -l -a
int ls_la(char *path, int out)
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

    char sorted[100][100];

    int n = 0;

    while ((content = readdir(dir)) != NULL)
    {
        strcpy(sorted[n], content->d_name);
        n++;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (strcasecmp(sorted[i], sorted[j]) > 0)
            {
                char temp[100];
                strcpy(temp, sorted[j]);
                strcpy(sorted[j], sorted[i]);
                strcpy(sorted[i], temp);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        char temp[100];
        strcpy(temp, path);
        strcat(temp, "/");
        strcat(temp, sorted[i]);

        stat(temp, &type);

        if (S_ISDIR(type.st_mode) != 0)
        {
            printf("d");
            // read for user
            if (type.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");

            // For group
            // Read
            if (type.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");

            // For others
            // Read
            if (type.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXOTH)
                printf("x ");
            else
                printf("- ");

            printf("%ld ", type.st_nlink);

            struct passwd *name = getpwuid(type.st_uid);
            printf("%s ", name->pw_name);

            struct group *group = getgrgid(type.st_gid);
            printf("%s ", group->gr_name);

            printf("%6ld ", type.st_size);

            char *modtime = ctime(&type.st_mtime);
            modtime[strlen(modtime) - 9] = '\0';
            printf("%s ", &modtime[4]);

            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;34m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }

        else if (type.st_mode & S_IEXEC)
        {
            printf("-");
            // read for user
            if (type.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");

            // For group
            // Read
            if (type.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");

            // For others
            // Read
            if (type.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXOTH)
                printf("x ");
            else
                printf("- ");

            printf("%ld ", type.st_nlink);

            struct passwd *name = getpwuid(type.st_uid);
            printf("%s ", name->pw_name);

            struct group *group = getgrgid(type.st_gid);
            printf("%s ", group->gr_name);

            printf("%6ld ", type.st_size);

            char *modtime = ctime(&type.st_mtime);
            modtime[strlen(modtime) - 9] = '\0';
            printf("%s ", &modtime[4]);

            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }

            else
            {
                printf("\033[0;32m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }

        else
        {
            printf("-");
            // read for user
            if (type.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");

            // For group
            // Read
            if (type.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");

            // For others
            // Read
            if (type.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");

            // Write
            if (type.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");

            // Execute
            if (type.st_mode & S_IXOTH)
                printf("x ");
            else
                printf("- ");

            printf("%ld ", type.st_nlink);

            struct passwd *name = getpwuid(type.st_uid);
            printf("%s ", name->pw_name);

            struct group *group = getgrgid(type.st_gid);
            printf("%s ", group->gr_name);

            printf("%6ld ", type.st_size);

            char *modtime = ctime(&type.st_mtime);
            modtime[strlen(modtime) - 9] = '\0';
            printf("%s ", &modtime[4]);

            if (out != 0)
            {
                printf("%s\n", sorted[i]);
            }
            else
            {
                printf("\033[0;37m");
                printf("%s\n", sorted[i]);
                printf("\033[1;37m");
            }
        }
    }

    closedir(dir);

    return 0;
}

// caller function
int ls(char *path, char *comm, char *home)
{
    int flaga = 0;
    int flagl = 0;
    char *print_into = (char *)malloc(sizeof(char) * 100);
    char directory[100][100];
    int original = dup(STDOUT_FILENO);
    int out = 0;

    if (strcmp(comm, "\0") == 0)
    {
        ls_noflag(path, out);
    }

    if (strcmp(comm, "\0") != 0)
    {
        char *token;
        char *flags = comm;
        char *temp = comm;
        char delim[] = " \t";
        int n = 0;
        int in = 0;
        int flag = 0;

        while (token = strtok_r(temp, delim, &temp))
        {
            if (strcmp(token, ">") == 0)
            {
                out = 1;
                continue;
            }
            else if (strcmp(token, ">>") == 0)
            {
                out = 2;
                continue;
            }

            if (strcmp(token, "<") == 0)
            {
                in = 1;
                continue;
            }

            else if (in == 1)
            {
                in = 0;
                continue;
            }

            if ((out == 1 || out == 2) && flag == 0)
            {
                strcpy(print_into, token);
                flag = 1;
                continue;
            }

            if (strcmp(token, "-a") == 0)
                flaga = 1;

            if (strcmp(token, "-l") == 0)
                flagl = 1;

            if (token[0] != '-')
            {
                strcpy(directory[n], token);
                n++;
                continue;
            }

            strcpy(flags, token);
        }

        int i = 0;
        // printf("out = %d\n", out);

        if (out == 1)
        {
            int fp = open(print_into, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            dup2(fp, STDOUT_FILENO);
            close(fp);
        }

        if (out == 2)
        {
            int fp = open(print_into, O_CREAT | O_WRONLY | O_APPEND, 0644);
            dup2(fp, STDOUT_FILENO);
            close(fp);
        }

        if (n == 0)
        {
            if (strcmp(flags, "-a") == 0)
            {
                if (flagl == 1)
                {
                    ls_la(path, out);
                    dup2(original, STDOUT_FILENO);
                    return 0;
                }

                ls_a(path, out);
                dup2(original, STDOUT_FILENO);
                return 0;
            }

            if (strcmp(flags, "-l") == 0)
            {

                if (flaga == 1)
                {
                    ls_la(path, out);
                    dup2(original, STDOUT_FILENO);
                    return 0;
                }

                ls_l(path, out);
                dup2(original, STDOUT_FILENO);
                return 0;
            }

            if (strcmp(flags, "-la") == 0 || strcmp(flags, "-al") == 0)
            {

                ls_la(path, out);
                dup2(original, STDOUT_FILENO);
                return 0;
            }

            else
            {
                ls_noflag(path, out);
            }
        }

        while (i < n)
        {
            char temp[100];

            if (strcmp(directory[i], "~") == 0)
            {
                strcpy(directory[i], home);
                strcpy(temp, directory[i]);
            }

            else
            {
                char buff[100];
                getcwd(buff, 100);
                strcpy(temp, directory[i]);

                int len = strlen(buff);

                if (strncmp(buff, directory[i], len) != 0)
                {
                    strcat(buff, "/");
                    strcat(buff, directory[i]);
                    strcpy(directory[i], buff);
                }
            }

            struct stat check;

            stat(directory[i], &check);

            if (S_ISREG(check.st_mode))
            {
                if (fopen(directory[i], "r") != NULL)
                    printf("%s\n", temp);
                else
                    perror("File");
                i++;
                continue;
            }

            if (strcmp(flags, "-a") == 0)
            {
                if (strcmp(directory[i], "\0") != 0)
                {
                    if (n != 1)
                        printf("%s:\n", temp);

                    if (strcmp(directory[i], "~") == 0)
                    {
                        if (flagl == 1)
                        {
                            ls_la(home, out);
                            i++;
                            continue;
                        }

                        ls_a(home, out);
                        i++;
                        continue;
                    }

                    strcpy(path, directory[i]);

                    if (flagl == 1)
                    {
                        ls_la(path, out);
                        i++;
                        continue;
                    }

                    ls_a(path, out);
                    i++;
                    continue;
                }
            }

            if (strcmp(flags, "-l") == 0)
            {
                if (strcmp(directory[i], "\0") != 0)
                {
                    if (n != 1)
                        printf("%s:\n", temp);

                    if (strcmp(directory[i], "~") == 0)
                    {
                        if (flaga == 1)
                        {
                            ls_la(home, out);
                            i++;
                            continue;
                        }

                        ls_l(home, out);
                        i++;
                        continue;
                    }

                    strcpy(path, directory[i]);

                    if (flaga == 1)
                    {
                        ls_la(path, out);
                        i++;
                        continue;
                    }

                    ls_l(path, out);
                    i++;
                    continue;
                }
            }

            if (strcmp(flags, "-la") == 0 || strcmp(flags, "-al") == 0)
            {
                if (strcmp(directory[i], "\0") != 0)
                {
                    if (n != 1)
                        printf("%s:\n", temp);

                    if (strcmp(directory[i], "~") == 0)
                    {
                        ls_la(home, out);
                        i++;
                        continue;
                    }

                    strcpy(path, directory[i]);
                    ls_la(path, out);
                    i++;
                    continue;
                }
            }

            else
            {
                if (n != 1)
                    printf("%s:\n", temp);

                ls_noflag(directory[i], out);
                i++;
                continue;
            }
        }
    }

    dup2(original, STDOUT_FILENO);
    return 0;
}
