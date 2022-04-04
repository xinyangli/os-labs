#ifndef HWSH_HIST_H
#define HWSH_HIST_H

int hist_init(void);
int hist_save(char *buf);
char *hist_prev(void);
char *hist_last(void);
char *hist_next(void);
int hist_free(void);
#endif  // HWSH_HIST_H