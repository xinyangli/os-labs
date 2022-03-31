#ifndef HWSH_ANALYZER_H
#define HWSH_ANALYZER_H
#include "exec.h"

#define MAX_TASK_IN_LINE 32

int scanner(char *buf, size_t *argc, char **argv);
Task* create_task(char **argv, size_t argc);
int delete_task(Task* task);
Task** parser(const char buf[], int* tasknum);

#endif // HWSH_ANALYZER_H