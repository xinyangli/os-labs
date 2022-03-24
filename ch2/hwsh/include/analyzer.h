#ifndef HWSH_ANALYZER_H
#define HWSH_ANALYZER_H

#define MAX_ARGV_SIZE 128 // 定义最大参数长度

int scanner(char *buf, int *argc, char **argv);
int parser();
#endif // HWSH_ANALYZER_H