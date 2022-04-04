#include <stdlib.h>
#include <stdio.h>
#include "hist.h"
#include "stack.h"
#define MAX_HISTORY 10000

static Stack *hist = NULL;
static size_t current_ref = 0;
int hist_init(void){
  hist = malloc(sizeof(Stack));
  if(stack_init(hist, MAX_HISTORY)){
    if(hist != NULL)
      free(hist);
    hist = NULL;
    return 1;
  }
  return 0;
}

int hist_save(char *buf) {
  if(hist == NULL){
    perror("History not initialized");
    return 1;
  }
  if(stack_full(hist)){
    char *tofree = NULL;
    stack_pop_bottom(hist, &tofree);
    free(tofree);
  }
  if(stack_push(hist, buf))
    return 1;
  current_ref = (hist->top + 1) % hist->size;
  return 0;
}

char *hist_prev(void) {
  size_t nxt = (current_ref - 1 + hist->size) % hist->size;
  if(nxt == hist->bottom)
    return NULL;
  current_ref = nxt;
  return hist->data[current_ref];
}

char *hist_last(void) {
  if(stack_empty(hist))
    return NULL;
  return hist->data[hist->top];
}

char *hist_next(void) {
  if(current_ref == (hist->top + 1) % hist->size)
    return NULL;
  if(current_ref != hist->top)
    current_ref = (current_ref + 1) % hist->size;
  return hist->data[current_ref];
}

int hist_free(void) {
  if(hist == NULL){
    perror("History not initialized");
    return 1;
  }
  char * s;
  while(stack_pop(hist, &s) == 0) {
    if(s != NULL) {
      free(s);
    }
  }
  free(hist->data);
  free(hist);
  return 0;
}