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
void print_error()
{
    switch(errno)
    {
        case E2BIG:fprintf(stderr,"Argument list too long!\n");break;
        case ENOTDIR:fprintf(stderr,"Not a directory!\n");break;
        case ENOENT:fprintf(stderr,"No such file or directory!\n");break;
        case ENOMEM:fprintf(stderr,"Not enough space/cannot allocate memory!\n");break;
        case EEXIST:fprintf(stderr,"File exists!\n");break;     
        case EINVAL:fprintf(stderr,"Invalid argument!|n");break;     
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

    int concurrent=0;
    // 只有一条指令时，直接执行，不需要重定向
    if(len==1) 
    {
        execvp(task->argv[0],task->argv);
        print_error(errno);
        return -1;
    }
    // head链表保存所有子进程号
    List *head;
    head=(List *)malloc(sizeof(List));
    head->next=NULL;
    // 当有>=2条指令时，需要重定向
    int pipefd[2];
    for (int i=0;i<len;i++)
    {
        if(pipe(pipefd) == -1) //创建管道，若失败则报错退出
        {
            fprintf(stderr, "pipe error");
        }
        pid_t pid;
        pid=fork();
        if(pid==0){
            //child process
            if(i!=len-1)
            {
                close(pipefd[0]);
                dup2(pipefd[1],STDOUT_FILENO); //将输出定义到写端
                close(pipefd[1]);
            }
            execvp(task->argv[0],task->argv);
            print_error(errno);
        }
        else
        {
            //parent process
            List *tmp;
            tmp=(List *)malloc(sizeof(List));
            tmp->val=pid;
            tmp->next=head->next;
            head->next=tmp;
            // 从第二条指令开始需要重定向读端口
            close(pipefd[1]);
            dup2(pipefd[0],STDIN_FILENO);
            close(pipefd[0]);
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