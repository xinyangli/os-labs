#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
      prompt();
      ret = getcmd(&buf, &len, "\t\n");
      if(ret == EOF)
        break;
      else if(ret == -2) {/* Received Ctrl + C */
        write(STDOUT_FILENO, "\n", 1);
        continue;
      }

      hist_save(buf);
      buf = NULL; len = 0;
      exec(argv, argc);
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