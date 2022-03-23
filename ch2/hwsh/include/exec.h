#ifndef HWSH_EXEC_H
#define HWSH_EXEC_H
typedef struct Task {
  char **argv;
  char *argc;
} Task;
int exec(Task *task, size_t len);

#endif // HWSH_EXEC_H