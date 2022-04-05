#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyzer.h"
#include "exec.h"
#include "hist.h"

static Task current_task;

size_t cmdlen(const char* str){
  size_t len = 0;
  while(str[len] != '\0') len++;
  return len;
}

char* subcmd(const char* str, int start, int end){
  char* substr = (char*)malloc(sizeof(char) * (end - start + 2));
  for(int i = start; i <= end; ++i){
    substr[i - start] = str[i];
  }
  substr[end - start + 1] = '\0';
  return substr;
}

int delete_cmd(char* str){
  free(str);
  return 0;
}

int delete_argv(char** argv, size_t argc){
  for(size_t i = 0; i < argc; ++i){
    free(argv[i]);
  }
  free(argv);
  return 0;
}

int scanner(char buf[], size_t *argc, char **argv){
  /*
  将传入的字符串按照空格分割
  参数解释:
    buf: 待分割字符串
    argc: 分割后参数个数, 以地址的形式传入, 指针的形式返回
    argv: 分割后的字符串数组, 每个一级指针指向一个字符串， 每个二级指针指向一个字符
  参数argv的只需要分配第一级指针，字符串分割后的指针的内存在函数内分配
  返回值：
    正常退出时, 返回值为0
    在其他非正常情况下返回-1
  */
  size_t num = 0;
  size_t len = cmdlen(buf);
  if(buf == NULL || len == 0)
    return -1;
  int l = 0, r = len - 1;
  while(buf[l] == ' ')  ++l;
  while(buf[r] == ' ' || buf[r] == '\n' || buf[r] == '\0')  --r;
  for(int p = l; p <= r; ++p){
    if(buf[p] == ' '){
      continue;
    }
    if(buf[p] == '!') {
      if(p + 1 <= r && buf[p + 1] == '!') {
        char *last_cmd = hist_last();

        char * substr = NULL;
        if(last_cmd != NULL)
          substr = subcmd(last_cmd, 0, strlen(last_cmd) - 2);
        (*argv) = substr;
        argv++;
        num++;
        p += 2;
        continue;
      }
    }
    if(buf[p] != '\"' && (buf[p + 1] == ' ' || buf[p + 1] == '\0' || buf[p + 1] == '\n')){
    	char* substr = subcmd(buf, p, p);
    	(*argv) = substr;
    	argv++;
    	num++;
    	continue;
    }
    size_t k = p + 1;
    if(buf[p] == '\"'){
      while(k <= r && buf[k] != '\"') ++k;
      if(k > r) return -1;
    }else{
      while(k <= r){
      	if(buf[k + 1] == ' ' || buf[k + 1] == '\0' || buf[k + 1] == '\n')	break;
      	++k;
      }
    }
    char* substr = subcmd(buf, p, k);
    (*argv) = substr;
    argv++;
    num++;
    p = k;
  }
  *argc = num;
  return 0;
}

int create_task(Task* task, char **argv, size_t argc){
  /*
  传入分割好的字符串数组和参数个数，创建并返回一个Task的指针型任务
  参数解释:
    argv: 分割后的字符串数组, 每个一级指针指向一个字符串， 每个二级指针指向一个字符
    argc: argv中的字符串个数
  函数创建了一个Task指针, 并分配了Task指针内的argv的内存，传入的参数不会被销毁内存
  返回值:
    一个Task指针, 其两个参数分别为传入的两个参数
  */
//  Task* task = (Task*) malloc(sizeof(Task));
  task->argc = argc;
  char** temp_argv = (char**) malloc(sizeof(char *) * argc);
  for(size_t i = 0; i < argc; ++i){
    temp_argv[i] = (char*) malloc(sizeof(char) * (cmdlen(argv[i]) + 1));
    strcpy(temp_argv[i], argv[i]);
  }
  task->argv = temp_argv;
  return 0;
}

int delete_task(Task* task){
  /*
  传入一个任务Task的指针，将该指针指向的内存释放
  参数解释：
    task：传入的任务指针
  函数涉及销毁task指针的内存
  返回值：
    在正常情况下，返回值均为0,表示task的内存空间已被成功释放。
  */
  for(size_t i = 0; i < task->argc; ++i){
    free(task->argv[i]);
  }
  free(task->argv);
  free(task);
  return 0;
}

Task* parser(const char buf[], size_t* tasknum) {
  size_t pipe_loc[MAX_TASK_IN_LINE];
  size_t num = 1;
  size_t cmd_len = cmdlen(buf);
  for(size_t i = 0; i < cmdlen(buf); ++i){
    if(buf[i] == '|'){
      pipe_loc[num] = i;
      num++;
    }
  }
  pipe_loc[num] = cmd_len;
  Task* task_list = (Task*)malloc(sizeof(Task) * num);
  size_t task_ptr = 0;
  for(size_t i = 1; i <= num; ++i){
    char* task_str;
    size_t argc;
    if(i == 1){
      task_str = subcmd(buf, 0, pipe_loc[i] - 1);
    }else if(i == num){
      task_str = subcmd(buf, pipe_loc[i - 1] + 1, cmdlen(buf) - 1);
    }else{
      task_str = subcmd(buf, pipe_loc[i - 1] + 1, pipe_loc[i] - 1);
    }
    char** argv = (char**)malloc(sizeof(char*) * MAX_ARGV_IN_CMD);
    scanner(task_str, &argc, argv);
    create_task(&(task_list[task_ptr]), argv, argc);
    task_ptr++;
    free(task_str);
    delete_argv(argv, argc);
  }
  *tasknum = num;
  return task_list;
}
