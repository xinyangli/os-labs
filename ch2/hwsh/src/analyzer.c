#include <stdio.h>
#include <stdlib.h>
#include "exec.h"

static Task current_task;
int scanner(char *buf, int *argc, char **argv) {
  /*
  将传入的字符串按照空格分割
  参数解释: 
    buf: 待分割字符串
    argc: 分割后参数个数, 以地址的形式传入, 指针的形式返回
    argv: 分割后的字符串数组, 每个一级指针指向一个字符串， 每个二级指针指向一个字符
  需要先给各个参数分配内存再进行调用
  返回值：
    正常退出时, 返回值为0
    在其他非正常情况下返回-1
  */
  int num = 0;
  if(buf == NULL || strlen(buf) == 0)
    return -1;
  char* str = strtok(buf, " ");
  while(str != NULL){
    *argv ++ = str;
    ++num;
    str = strtok(NULL, " ");
  }
  *argc = num;
  return 0;
}

int parser() {
  /*
  根据scanner的分割结果，分析命令是否输入完成
  */

  return 0;
}
