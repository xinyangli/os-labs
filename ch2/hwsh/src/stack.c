#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


int stack_init(Stack *s, size_t size) {
  s->data = malloc(size * sizeof(stack_data_t));
  s->top = s->bottom = 0;
  s->size = size;
  if (s->data == NULL) {
    perror("Malloc history buffer failed!");
    return 1;
  }
  return 0;
}

int stack_full(Stack *s) {
  if(s->bottom == (s->top + 1) % s->size)
    return 1;
  return 0;
}

int stack_empty(Stack *s) {
  if(s->top == s->bottom)
    return 1;
  return 0;
}

int stack_pop(Stack *s, stack_data_t *dest) {
  if (stack_empty(s))
    return 1;
  if (dest != NULL)
    *dest = s->data[s->top];
  s->top = (s->top - 1 + s->size) % s->size;
  return 0;
}

int stack_push(Stack *s, stack_data_t data) {
  if (stack_full(s))
    return 1;
  s->top = (s->top + 1) % s->size;
  s->data[s->top] = data;
  return 0;
}

int stack_pop_bottom(Stack *s, stack_data_t *dest) {
  if(stack_empty(s))
    return 0;
  if (dest != NULL)
    *dest = s->data[s->bottom];
  s->bottom = (s->bottom + 1) % s->size;
}
