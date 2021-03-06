#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "getcmd.h"
#include "hist.h"

static struct termios orig_termios;

enum Keys {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  PAGE_UP,
  PAGE_DOWN
};

/* TODO: call disable_raw to clean up at exit */
int disable_raw() {
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    return 1;
}

int enable_raw() {
  if(tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    return 1;
  struct termios raw = orig_termios;
//  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON);
//  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    return 1;
  return 0;
}

int read_key(){
  char ch = '\0';
  int ret = 0;
  if(( ret = read(STDIN_FILENO, &ch, 1) )<= 0) {
    return ret;
  }
  if(ch == '\x1b') {  /* Escape sequences */
    char seq[3];

    /* Expect 2 characters following ch */
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
          }
        }
      } else {
        switch (seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
        }
      }
    }
    return '\x1b';
  } else if(ch == 4) {
    return EOF;
  }
  return ch;
}

/*
 *
 */
/* TODO: realloc error handling */
ssize_t getcmd(char **buf, size_t *n, const char *delim) {
  size_t block_size = 64;
  ssize_t len = 0;
  int ch = 0;
  char *ptr_buf;
  char output_buf[30];

  if(*buf == NULL) {
    *buf = malloc(block_size);
    *n = block_size;
    block_size <<= 1;
  } else {
    while(block_size < *n){
      block_size <<= 1;
    }
  }

  ptr_buf = *buf;
  while((ch = read_key()) > 0){
//    write(STDOUT_FILENO, &ch, 1);
    int in_delim = 0;
    /* Handle a seris of special keys */
    char * hist_display = NULL;
    switch (ch) {
      case ARROW_LEFT:
        if(ptr_buf > *buf) {
          ptr_buf--;
          write(STDOUT_FILENO, "\x1b[D", 3);
        }
        break;
      case ARROW_RIGHT:
        if(ptr_buf < *buf + len) {
          ptr_buf++;
          write(STDOUT_FILENO, "\x1b[C", 3);
        }
        break;

      case ARROW_UP:
      case ARROW_DOWN:
        hist_display = (ch == ARROW_UP) ? hist_prev() : hist_next();
        if(hist_display == NULL)
          break;

        if(len > 0) {
          snprintf(output_buf, 30, "\x1b[%ldD\x1b[J", len); /* Clear current line */
          write(STDOUT_FILENO, output_buf, strlen(output_buf)); /* Clear line */
        }

        len = strlen(hist_display);

        /* eliminate the last deliminator */
        for(int i = len - 1; i >= 0; i--){
          int flag = 0;
          for(int j = 0; j < strlen(delim); j++)
            if(hist_display[i] == delim[j]){
              hist_display[i] = '\0';
              len = i;
              flag = 1;
              break;
            }
          if(flag)
            break;
        }
        if(len + 1 > *n) {
          *buf = realloc(*buf, len + 1);
          *n = len + 1;
        }
        ptr_buf = *buf + len;
        strncpy(*buf, hist_display, *n);
        write(STDOUT_FILENO, *buf, len);
        break;

      /* Backspace and Delete */
      case '\x08':
      case '\x7f':
        if(ptr_buf == *buf)
          break;
        ptr_buf--;
        for(char *p = ptr_buf; p < *buf + len; p++){
          *p = *(p+1);
        }
        len--;
        write(STDOUT_FILENO, "\x1b[D\x1b[K", 6);
        write(STDOUT_FILENO, ptr_buf, len - (ptr_buf - *buf));

        if(ptr_buf < *buf + len) {
          /* Move back cursor */
          snprintf(output_buf, 30, "\x1b[%ldD", len - (ptr_buf - *buf));
          write(STDOUT_FILENO, output_buf, strlen(output_buf));
        }
        break;
      /* Ctrl + C */
      case '\x03':
        return -2;
      default:
        /* check if this character is in delim */
        for(int i = 0; i < strlen(delim); i++) {
          if(ch == delim[i])
            in_delim = 1;
        }
        write(STDOUT_FILENO, &ch, 1);
        for(char *p = *buf + len; p > ptr_buf; p--) {
          *p = *(p-1);
        }
        *ptr_buf = ch;
        len++; ptr_buf++;
        if(in_delim){
          if(*n > len + 1){
            *buf = realloc(*buf, len + 1); // one more for \0
            *n = len + 1;
          }
          (*buf)[len] = '\0';
          return len;
        }

        /* Prepare for next read */
        if(len + 1 + 1 > *n){ // one more for null at the end
          size_t offset = ptr_buf - *buf;
          *buf = realloc(*buf, block_size);
          *n = block_size;
          block_size <<= 1;
          ptr_buf = *buf + offset;
        }
        break;
    }


  }
  return EOF;
}