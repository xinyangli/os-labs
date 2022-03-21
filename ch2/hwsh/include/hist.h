#ifndef HWSH_HIST_H
#define HWSH_HIST_H
typedef struct HistLine {
    char **argv;
    int argc;
} HistLine;

typedef struct CurrentLine {
    char **argv;
    int argc;
    int *str_len;   /* length of every argument */
    int argv_size;  /* malloced size of the pointer array */
    int *str_size;  /* malloced size of every string in argv */
} CurrentLine;

/* History stack */
typedef struct Hist {
    HistLine *lines;
    int size, cur;
} Hist;

#include "global.h"
#endif  // HWSH_HIST_H