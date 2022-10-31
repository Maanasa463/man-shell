# Assignment 3 -  2021101101 


## Maanasa Kovuru

> Ubuntu 22.04
*  
    * Terminal Commands :
    
        * > `make `
        * > `./shell` 
        * The above code creates a shell that can execute basic shell commands.

        * shell.c is the main file. 
    
    * commands.c

        * Contains the code required to execute cd,pwd and echo commands.
        * cd returns an error if the directory is not found.
        * echo prints a new line if no succeeding argument is given.
        * pwd prints the absolute path of the current working directory from the root of the system.
        
    * ls.c
        * Executes the ls function.
        * Contains different functions to execute the different flags.
        * Handles multiple files and directories at once.
        * Returns error if file/ directory not found.

    * process.c
        * Executes foreground and background processes via fork() and exec().

    * pinfo.c
        * Gives information about the process with given pid.
        * If no pid is given, it gives information about the currently running process which is the shell.

    * discover.c
        * Executes the discover function.
        * Returns error if target directory does not exist. Assume only one target directory. 
        * Return path of file from target directory if file is found, else nothing is printed.
        * Assume only one "file/directory" is to be searched for.
    
    * history.c
        * Executes the history function.
        * Contains two functions: one stores the given commands in a file, the other fetches the history when history is called from shell.
        * Only 20 commands in history file at once.
        * Last 10 commands printed on calling history.

    * sig.c
        * Executes the sig command.
        * Gives an error on giving too many, or too little arguments.
        * Uses kill to execute signal.
    
    * fgbg.c
        * Used to execute the commands fg and bg.
        * Gives an error on giving too few or too many commands.
        * fg calls running/ stopped background processes to the background.
        * bg changes state of stopped backgorund process to running in the background.

    * jobs.c
        * Executes the jobs commands.
        * Can execute jobs with -r and -s flags.
        * Prints the list of background jobs (runnning or stopped or both depending on flags) on the terminal.

    * signals.c
        * Executes ctrl+C and ctrl+Z using signal handlers.

* Invalid commands give an error.
