#ifndef HWSH_ANALYZER_H
#define HWSH_ANALYZER_H
#include "exec.h"

#define MAX_ARGV_SIZE 64

int scanner(char *buf, size_t *argc, char **argv);
Task* create_task(char **argv, size_t argc);
int delete_task(Task* task);
int parser();

#endif // HWSH_ANALYZER_H