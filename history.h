#ifndef _HISTORY_H
#define _HISTORY_H

extern int num_commands;

void store_in_file(char* command, char* hispath);
void history(char * hispath, char* comm);

#endif