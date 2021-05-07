#include <stdio.h>
#include <stdlib.h>
typedef int DataType;
typedef struct Kstack {
  DataType data;
  struct Kstack* next;
} Kstack;

void initKstack(Kstack** S) {
  (*S) = (Kstack*)malloc(sizeof(Kstack));
  if (!(*S)) {
    puts("init malloc failed");
    exit(1);
  }
  (*S)->data = 9999;
  (*S)->next = NULL;
}

void destroyKstack(Kstack** S) {
  Kstack* top = *S;
  while (top->next != NULL) {
    Kstack* temp = top->next;
    top->next = top->next->next;
    /* printf("free: %d\n", temp->data); */
    free(temp);
  }
  *S = NULL;
}

int pushKstack(Kstack* S, DataType elem) {
  Kstack* new_top = (Kstack*)malloc(sizeof(Kstack));
  if (!new_top) {
    puts("push malloc failed!");
    return -1;
  }
  new_top->data = elem;
  new_top->next = S->next;
  S->next = new_top;
  return 0;
}

int popKstack(Kstack* S, DataType* deleted) {
  if (S->next == NULL) {
    puts("pop empty stack!");
    return -1;
  }
  Kstack* del = S->next;
  S->next = S->next->next;
  *deleted = del->data;
  free(del);
  del = NULL;
  return 0;
}

void visit(DataType elem) {
  printf("%d|", elem);
}

void visitChar(DataType elem) {
  printf("%c|", elem);
}

void traverseKstack(Kstack* S, void (*visit)(DataType elem)) {
  while (S->next != NULL) {
    S = S->next;
    visit(S->data);
  }
  putchar(10);
}

int lengthOfKstack(Kstack* S) {
  int len = 0;
  while (S->next != NULL) {
    S = S->next;
    len++;
  }
  return len;
}

void printBase10asBaseN(int num, int n) {
  Kstack head = {.data = 9999, .next = NULL};
  Kstack* stack = &head;
  DataType pop_elem;
  initKstack(&stack);
  while (num) {
    pushKstack(stack, num % n);
    num /= n;
  }
  while (stack->next != NULL) {
    popKstack(stack, &pop_elem);
    printf("%d", pop_elem);
  }
  destroyKstack(&stack);
}

int testValidBracket(char* str) {
  Kstack head = {.data = 9999, .next = NULL};
  Kstack* stack = &head;
  DataType pop_elem;
  initKstack(&stack);
  while (*str) {
    if ((*str == ']' && stack->next->data == '[')
        || (*str == '}' && stack->next->data == '{')
        || (*str == ')' && stack->next->data == '(')) {
      popKstack(stack, &pop_elem);
    }
    else {
      pushKstack(stack, *str);
    }
    /* traverseKstack(stack, visitChar); */
    str++;
  }
  return lengthOfKstack(stack);
}

void localTest() {
  Kstack head = {.data = 9999, .next = NULL};
  Kstack* stack = &head;
  DataType del;
  initKstack(&stack);
  for (int i = 100; i < 200; i += 10) {
    traverseKstack(stack, visit);
    pushKstack(stack, i);
  }
  for (int i = 0; i < 2; ++i) {
    popKstack(stack, &del);
  }
  traverseKstack(stack, visit);
  destroyKstack(&stack);
}

int main(int argc, char *argv[]) {
  /*
  Kstack* stack;
  DataType del;
  initKstack(&stack);
  for (int i = 100; i < 200; i += 10) {
    traverseKstack(stack, visit);
    pushKstack(stack, i);
  }
  for (int i = 0; i < 2; ++i) {
    popKstack(stack, &del);
  }
  traverseKstack(stack, visit);
  destroyKstack(&stack);
  */

  /* localTest(); */

  /* printBase10asBaseN(1348, 2); */
  /* putchar(10); */

  /* printf("%d\n", testValidBracket("{[()]}[]")); */

  return 0;
}

