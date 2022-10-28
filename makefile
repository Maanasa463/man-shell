CC = gcc 

shell: shell.c commands.c discover.c history.c pinfo.c process.c ls.c jobs.c fgbg.c sig.c signals.c
	$(CC) shell.c commands.c discover.c history.c pinfo.c process.c ls.c jobs.c fgbg.c sig.c signals.c -o shell