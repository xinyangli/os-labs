typedef struct Task {
  char **argv;
  char *argc;
} Task;
int exec(Task *task, size_t len);
