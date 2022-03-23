#ifndef HWSH_STACK_H
#define HWSH_STACK_H
typedef char *stack_data_t;
typedef struct Stack {
  size_t top, bottom;
  size_t size;
  stack_data_t *data;
} Stack;
int stack_init(Stack *s, size_t size);
int stack_full(Stack *s);
int stack_empty(Stack *s);
int stack_pop(Stack *s, stack_data_t *dest);
int stack_push(Stack *s, stack_data_t data);
int stack_pop_bottom(Stack *s, stack_data_t *dest);

#endif // HWSH_STACK_H