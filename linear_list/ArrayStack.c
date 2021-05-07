#include <stdio.h>
#include <stdlib.h>
/* #include <string.h> */
#define ASTACK_INIT_SIZE 5
#define ASTACK_INCR_SIZE 5
typedef int DataType;
typedef struct Astack {
  DataType* bot;
  DataType* top;
  int capacity;
} Astack;

// P59
void initAstack(Astack* S) {
  S->bot = (DataType*)malloc(ASTACK_INIT_SIZE * sizeof(DataType));
  if (!S->bot) {
    puts("malloc failed!");
    exit(1);
  }
  S->top = S->bot;
  S->capacity = ASTACK_INIT_SIZE;
}

void destroyAstack(Astack* S) {
  free(S->bot);
  S->bot = NULL;
  S->top = NULL;
  S->capacity = 0;
}

int isEmptyAstack(Astack* S) {
  return S->top == S->bot;
}

int lengthOfAstack(Astack* S) {
  return S->top - S->bot;
}

DataType* getTopAstack(Astack* S) {
  return S->top;
}

int pushAstack(Astack* S, DataType elem) {
  if (S->top - S->bot >= S->capacity) {
    S->bot = (DataType*)realloc(S->bot, (S->capacity + ASTACK_INCR_SIZE) * sizeof(DataType));
    if (!S->bot) {
      puts("push realloc failed!");
      return -1;
    }
    S->top = S->bot + S->capacity;
    S->capacity += ASTACK_INCR_SIZE;
  }
  *(S->top++) = elem;
  return 0;
}

int popAstack(Astack* S, DataType* deleted) {
  if (S->top == S->bot) {
    puts("pop empty stack!");
    return -1;
  }
  *deleted = *(S->top--);
  int actual_len = S->top - S->bot;
  if (S->capacity > ASTACK_INIT_SIZE && S->capacity - actual_len > ASTACK_INCR_SIZE) {
    S->bot = (DataType*)realloc(S->bot, (S->capacity - ASTACK_INCR_SIZE) * sizeof(DataType));
    if (!S->bot) {
      puts("pop realloc failed!");
      return -1;
    }
    S->top = S->bot + actual_len;
    S->capacity -= ASTACK_INCR_SIZE;
  }
  return 0;
}

void visit(DataType elem) {
  printf("|%d", elem);
}

void traverseAstack(Astack* S, void (*visit)(DataType elem)) {
  DataType* cur = S->bot;
  printf("{%d}", S->capacity);
  while (cur != S->top) {
    visit(*cur);
    cur++;
  }
  putchar(10);
}

int main(int argc, char *argv[]) {
  Astack stack;
  DataType deleted;
  initAstack(&stack);
  for (int i = 100; i < 160; i += 10) {
    pushAstack(&stack, i);
  }
  traverseAstack(&stack, visit);
  for (int i = 0; i < 2; ++i) {
    popAstack(&stack, &deleted);
  }
  traverseAstack(&stack, visit);
  destroyAstack(&stack);
  return 0;
}
