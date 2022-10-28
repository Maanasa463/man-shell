#ifndef _COMMAND_H
#define _COMMAND_H

int cd(char** name, char* save, char** prev, char** prev2);
int echo(char* string);
int pwd(char* path);
int cd_prompt(char **save, char **host, char **host_save, char **save2, char** comm,
                char* path, char** prevwd, char** prevw);

#endif