#ifndef HWSH_GETCMD_H
#define HWSH_GETCMD_H
#include <stdio.h>

ssize_t getcmd(char ** buf, size_t *n, const char *delim);
int enable_raw();
int disable_raw();
#endif // HWSH_GETCMD_H
