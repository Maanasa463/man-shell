#ifndef _LS_H
#define _LS_H

int ls(char* path, char* comm, char* home);
int ls_a(char* path, int out);
int ls_l(char* path, int out);
int ls_la(char* path, int out);
int ls_noflag(char* path, int out);

#endif