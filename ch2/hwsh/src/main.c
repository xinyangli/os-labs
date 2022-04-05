#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "exec.h"
#include "analyzer.h"
#include "getcmd.h"
#include "hist.h"

void prompt();

int main() {
    /* TODO: manage memory for argv */
    char **argv = NULL;
    int argc = 0;
    char *buf = NULL;
    size_t len = 0;
    if(enable_raw()) {
      perror("Failed to initialize terminal!\n");
      return -1;
    }
    if(hist_init()) {
      perror("Failed to initialize history buffer, not recording history\n");
    }
    while (1) {
      ssize_t ret;
      size_t task_cnt;
      Task *t;
      prompt();
      ret = getcmd(&buf, &len, "\n");
      signal(SIGINT, SIG_IGN);
      disable_raw();
      if(ret == EOF)
        break;
      else if(ret == -2) {/* Received Ctrl + C */
        write(STDOUT_FILENO, "^C\n", 3);
        continue;
      }
      t = parser(buf, &task_cnt);
      hist_save(buf);
      exec(t, task_cnt);
      if(enable_raw()) {
        perror("Failed to initialize terminal!\n");
        return -1;
      }
      signal(SIGINT, SIG_DFL);
      buf = NULL; len = 0;
    }
    if(buf != NULL)
      free(buf);
    disable_raw();
    hist_free();
    
    return 0;
}


void prompt(){
    char cwd[PATH_MAX + 5];
    int len;
//    printf("%s$ ", getcwd(cwd, PATH_MAX));
    getcwd(cwd, PATH_MAX);
    len = strlen(cwd);
    for(int i = len - 1; i >= 0; i--){
      if(cwd[i] == '/'){
        i++;
        for(int j = 0; i <= len; i++,j++){
          cwd[j] = cwd[i];
        }
        break;
      }
    }
    strcat(cwd, " $ ");
    write(STDOUT_FILENO, cwd, strlen(cwd));
}