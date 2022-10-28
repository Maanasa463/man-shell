#ifndef _DISCOVERY_H
#define _DISCOVERY_H

int discover_noflags(char *path, char *print, char* file);
int discover(char* path, char* comm, char* home);
int discover_d(char *path, char *file, char *print);
int discover_f(char *path, char *file, char *print);
int discover_df(char *path, char *file, char *print);

#endif