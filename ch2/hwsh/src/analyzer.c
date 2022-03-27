#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec.h"

static Task current_task;
int scanner(char buf[], size_t *argc, char **argv) {
  /*
  将传入的字符串按照空格分割
  参数解释: 
    buf: 待分割字符串
    argc: 分割后参数个数, 以地址的形式传入, 指针的形式返回
    argv: 分割后的字符串数组, 每个一级指针指向一个字符串， 每个二级指针指向一个字符
  函数没有涉及内存的分配和销毁，需要先给各个参数分配内存再进行调用
  返回值：
    正常退出时, 返回值为0
    在其他非正常情况下返回-1
  */
  size_t num = 0;
  if(buf == NULL || strlen(buf) == 0)
    return -1;
  char *str, *cur = buf;
  while(str = strsep(&cur, " ")){
    *argv ++ = str;
    num++;
  }
  *argc = num;
  return 0;
}

Task* create_task(char **argv, size_t argc){
  /*
  传入分割好的字符串数组和参数个数，创建并返回一个Task的指针型任务
  参数解释:
    argv: 分割后的字符串数组, 每个一级指针指向一个字符串， 每个二级指针指向一个字符
    argc: argv中的字符串个数
  函数创建了一个Task指针, 并分配了Task指针内的argv的内存，传入的参数不会被销毁内存
  返回值:
    一个Task指针, 其两个参数分别为传入的两个参数
  */
  Task* task = (Task*) malloc(sizeof(Task));
  task->argc = argc;
  char** temp_argv = (char**) malloc(sizeof(char *) * argc);
  for(int i = 0; i < argc; ++i){
    temp_argv[i] = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
    strcpy(temp_argv[i], argv[i]);
  }
  task->argv = temp_argv;
  return task;
}

int parser() {
  /*
  根据scanner的分割结果，分析命令是否输入完成
  */

  return 0;
}
