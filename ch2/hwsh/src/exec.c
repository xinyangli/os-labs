#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "exec.h"

// Execute a given series of Tasks
typedef struct node{
    int val;
    struct node *next;
}List;
void print_error(Task t)
{
    switch(errno)
    {
        case E2BIG:fprintf(stderr,"Argument list too long!\n");break;
        case ENOENT:fprintf(stderr,"No such file or directory: %s\n", t.argv[0]);break;
        case ENOMEM:fprintf(stderr,"Not enough space/cannot allocate memory!\n");break;
        case EEXIST:fprintf(stderr,"File exists!\n");break;
        case EINVAL:fprintf(stderr,"Invalid argument!\n");break;
        default:fprintf(stderr,"Unknown error!\n");break;
    }
}
void destroy_list(List *p)
{
    List *tmp=p;
    List *tt;
    while(tmp){
        tt=tmp;
        tmp=tmp->next;
        free(tt);
    }
}

int exec(Task *task, size_t len) {

    // head链表保存所有子进程号
    List *head;
    head=(List *)malloc(sizeof(List));
    head->next=NULL;
#ifdef DEBUG
    printf("len:%lu\n", len);
    for(int i = 0; i < len; i++) {
      printf("task[i]: %p\n", task + i);
      for(int k = 0; k < task[i].argc; k++) {
        printf("%d: %s\n", i, task[i].argv[k]);
      }
    }
#endif
  // 当有>=2条指令时，需要重定向
    int old_pipefd[2], new_pipefd[2];
    for (int i=0;i<len;i++)
    {
        if(task[i].argc == 0)
          continue;
        if(strlen(task[i].argv[0]) >= 2 && !strncmp(task[i].argv[0], "cd", 2)) {
          if(task[i].argc != 2) {
            fprintf(stderr, "Require exactly 1 arguments, got %lu\n", task[i].argc - 1);
          }
          if(chdir(task[i].argv[1])) {
            switch(errno) {
              case EACCES:
                fprintf(stderr, "Permission denied: %s\n", task[i].argv[1]);
                break;
              case ENOTDIR:
                fprintf(stderr, "%s is not a directory\n", task[i].argv[1]);
                break;
              case ENOENT:
                fprintf(stderr, "No such file or directory: %s\n", task[i].argv[1]);
            }
          }
          continue;
        }
        if(strlen(task[i].argv[0]) >= 4 && !strncmp(task[i].argv[0], "exit", 4)) {
          exit(EXIT_SUCCESS);
        }
        if(i != len-1) {
          if(pipe(new_pipefd) == -1) //创建管道，若失败则报错退出
          {
            fprintf(stderr, "pipe error");
            exit(EXIT_FAILURE);
          }
        }
        pid_t pid;
        pid=fork();
        if(pid==0){
          signal(SIGINT, SIG_DFL);
            //child process
          // 从第二条指令开始需要重定向读端口
          if(i > 0){
            close(old_pipefd[1]);
            dup2(old_pipefd[0],STDIN_FILENO);
            close(old_pipefd[0]);
          }
          if(i!=len-1)
            {
                close(new_pipefd[0]);
                dup2(new_pipefd[1],STDOUT_FILENO); //将输出定义到写端
                close(new_pipefd[1]);
            }
            execvp(task[i].argv[0],task[i].argv);
            print_error(task[i]);
            exit(EXIT_FAILURE);
        } else {
            //parent process
            List *tmp;
            tmp=(List *)malloc(sizeof(List));
            tmp->val=pid;
            tmp->next=head->next;
            head->next=tmp;
            if(i > 0) {
              close(old_pipefd[0]);
              close(old_pipefd[1]);
            }
            old_pipefd[0] = new_pipefd[0];
            old_pipefd[1] = new_pipefd[1];
        }
    }
    int status;
    List *p=head->next;
    while(p){
        waitpid(p->val,&status,0);
        p=p->next;
    }
    destroy_list(head);
    return 0;
}