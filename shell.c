#include "header.h"

int run = 1;

void D_handler()
{
    run = 0;
}

int main()
{
    fg_pid = getpgid(getpid());
    shell = getpid();

    char *u = (char *)malloc(sizeof(char) * 1000);
    char dir[1000];
    char host1[1000];
    char host[1000];

    // Job number
    p = 0;

    char *path = (char *)malloc(sizeof(char) * 100);

    // Gets path of home directory
    getcwd(path, 100);

    // Generates absolute path of file which stores history
    char *hispath = (char *)malloc(sizeof(char) * 100);
    strcpy(hispath, path);
    strcat(hispath, "/history.txt");

    // Gets name of user
    getlogin_r(u, sizeof(u));

    // Gets name of host system
    gethostname(host1, sizeof(host));
    char *save = (char *)malloc(sizeof(char) * 100);
    strcat(u, "@");
    strcat(u, host1);
    strcat(host, ":~");

    // Saves the current prompt
    strcpy(save, host);

    char host_save[1000];
    strcpy(host_save, host);

    // Gives time taken to execute a foreground process as prompt if time taken > 1
    char prompt[1000];
    int fflag = 0;

    char ch_dir[1000];

    // Saves the previous working directory
    char *prevwd = (char *)malloc(sizeof(char) * 100);
    getcwd(prevwd, 100);

    // Saves the previous working directory
    char *prevw = (char *)malloc(sizeof(char) * 100);
    getcwd(prevw, 100);

    // Saves the previous prompt
    char *save2 = (char *)malloc(sizeof(char) * 100);

    // Saves the id of stdout
    int original = dup(STDOUT_FILENO);
    int input = dup(STDIN_FILENO);

    signal(SIGINT, INT_handler);

    struct sigaction new_action, old_action;

    signal(SIGTSTP, STP_handler);
    new_action.sa_handler = STP_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGTSTP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGTSTP, &new_action, NULL);

    // signal(SIGTTIN, SIG_DFL);
    // signal(SIGTTOU, SIG_DFL);

    while (run)
    {
        printf("\033[1;32m<%s", u);

        // If foreground process took > 1s to execute
        if (fflag == 1)
        {
            printf("\033[1;34m%s>\033[1;37m", prompt);
            fflag = 0;
        }

        else
        {
            printf("\033[1;34m%s>\033[1;37m", host);
        }

        // Scan the user command
        char command[1000];

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            D_handler();

            for (int i = 0; i < p; i++)
            {
                kill(proc[i].pid, SIGKILL);
            }

            printf("\n");
            exit(0);
        }

        // Null terminate the command removing new line
        command[strlen(command) - 1] = '\0';

        // Tokenise according to "|"
        char *store = (char *)malloc(sizeof(char) * 1000);
        strcpy(store, command);

        int pipes = 0;
        char *count;
        char commands[100][1000];

        //  Count the number of pipes
        while (count = strtok_r(store, "|", &store))
        {
            strcpy(commands[pipes], count);
            pipes++;
        }

        char delim_1[] = ";&";
        char *tok = (char *)malloc(sizeof(char) * 100);

        // stores the command in the history file
        store_in_file(command, hispath);

        // Function to check if background processes terminate
        child_handler(proc, p);

        int channel[pipes - 1][2];

        for (int i = 0; i < pipes; i++)
        {
            char *com = commands[i];
            char copy[10000];
            strcpy(copy, commands[i]);

            if (i != pipes - 1 && pipes != 1)
            {
                pipe(channel[i]);
                dup2(channel[i][1], STDOUT_FILENO);
            }
            else
            {
                dup2(original, STDOUT_FILENO);
            }

            if (i != 0)
            {
                dup2(channel[i - 1][0], STDIN_FILENO);
            }

            // Tokenises according to the delimiters & and ; to separate commands
            // input output redirection
            while (tok = strtok_r(com, delim_1, &com))
            {

                char delim[] = " \t";
                char copy_tok[10000];
                strcpy(copy_tok, tok);
                char *comm = tok;
                char *token;

                // Tokenises according to " " and tab space "\t"
                token = strtok_r(comm, delim, &comm);

                // If given command is cd
                if (strcmp(token, "cd") == 0)
                {
                    if (strcmp(comm, "\0") == 0 || strcmp(comm, "~") == 0)
                    {
                        strcpy(save, host);
                        strcpy(host, host_save);
                        chdir(path);
                    }

                    else
                    {
                        int flag = cd(&comm, path, &prevwd, &prevw);

                        if (strcmp(comm, ".") == 0)
                        {
                            strcpy(save, host);
                            strcpy(host, host);
                        }

                        else
                        {
                            if (strcmp(comm, "-") == 0)
                            {
                                strcpy(save2, host);
                                strcpy(host, save);
                                strcpy(save, save2);
                            }

                            else if (strcmp(comm, "..") == 0)
                            {
                                char *wd = (char *)malloc(sizeof(char) * 100);
                                getcwd(wd, 100);

                                char *temp = (char *)malloc(sizeof(char) * 100);

                                strcpy(save, host);

                                if (strcmp(path, wd) > 0)
                                {
                                    strcpy(temp, host_save);
                                    strcat(temp, wd);
                                    strcpy(host, temp);
                                }

                                else
                                {
                                    temp = strrchr(host, '/');
                                    *temp = '\0';
                                }
                            }

                            else
                            {
                                if (flag >= 0)
                                {
                                    strcpy(save, host);
                                    strcat(host, "/");
                                    strcat(host, comm);
                                }
                            }
                        }
                    }
                }

                // If given command is pwd
                else if (strcmp(token, "pwd") == 0)
                {
                    // printf("%s\n", comm);
                    pwd(comm);
                }

                // If given command is echo
                else if (strcmp(token, "echo") == 0)
                {
                    echo(comm);
                }

                // If given command is history
                else if (strcmp(token, "history") == 0)
                {
                    history(hispath, comm);
                }

                // If given command is ls
                else if (strcmp(token, "ls") == 0)
                {
                    char *curr = (char *)malloc(sizeof(char) * 100);
                    getcwd(curr, 100);

                    ls(curr, comm, path);
                }

                // If given command is discover
                else if (strcmp(token, "discover") == 0)
                {
                    char *curr = (char *)malloc(sizeof(char) * 100);
                    getcwd(curr, 100);

                    discover(curr, comm, path);
                }

                // If given command is pinfo
                else if (strcmp(token, "pinfo") == 0)
                {
                    pinfo(comm, path);
                }

                else if (strcmp(token, "jobs") == 0)
                {
                    jobs(comm, proc, p);
                }

                else if (strcmp(token, "fg") == 0)
                {
                    fg(comm, p, proc);
                }

                else if (strcmp(token, "bg") == 0)
                {
                    bg(comm, p, proc);
                }

                else if (strcmp(token, "sig") == 0)
                {
                    sig(comm, p, proc);
                }

                else if (strcmp(token, "quit") == 0)
                {
                    for (int i = 0; i < p; i++)
                    {
                        kill(proc[i].pid, SIGKILL);
                    }

                    exit(0);
                }

                // Any other command is given
                else
                {
                    char *arg[100];
                    memset(arg, '\0', 100);

                    // checks for I/O redirection
                    handle(arg, comm, token);

                    char del;

                    // Delimiter following the command obtained;
                    del = copy[com - commands[i] - 1];

                    int back = 0;

                    // If process is to run in background
                    if (del == '&')
                        back = 1;

                    // For foreground
                    if (back == 0)
                    {
                        int time;
                        strcpy(fg_name, copy_tok);
                        fore_process(arg, &time);

                        // If time taken to finish > 1s
                        if (time > 1)
                        {
                            fflag = 1;

                            char t[100];

                            sprintf(t, "%d", time);
                            strcpy(prompt, ":~took ");
                            strcat(prompt, t);
                            strcat(prompt, "s");
                        }
                    }

                    else
                    {
                        int pid;
                        proc[p].name = (char *)malloc(sizeof(char) * 100);
                        // Stores bg command info into struct
                        strcpy(proc[p].name, copy_tok);

                        back_process(arg, &pid);

                        proc[p].pid = pid;
                        proc[p].job_num = p + 1;
                        proc[p].run = 0;

                        printf("[ %d ]  %d\n", proc[p].job_num, proc[p].pid);
                        p++;
                    }

                    if (pipes == 1)
                    {
                        dup2(original, STDOUT_FILENO);
                    }
                    else
                    {
                        dup2(channel[i][1], STDOUT_FILENO);
                    }
                }
            }

            close(channel[i - 1][0]);
            close(channel[i][1]);
        }

        // close(channel[pipes - 1][0]);

        dup2(input, STDIN_FILENO);
        dup2(original, STDOUT_FILENO);
    }

    return 0;
}