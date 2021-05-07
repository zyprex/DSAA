#include <stdio.h>
#include <stdlib.h>
#define AQUEUE_MAX_SIZE 6
typedef int DataType;
typedef struct Aqueue {
  DataType* elem;
  int front;
  int rear;
} Aqueue;

void initAqueue(Aqueue* Q) {
  Q->elem = (DataType*)malloc(AQUEUE_MAX_SIZE * sizeof(DataType));
  if (!(Q->elem)) {
    puts("malloc failed");
    exit(1);
  }
  Q->front = 0;
  Q->rear = 0;
}

void destroyAqueue(Aqueue* Q) {
  free(Q->elem);
  Q->elem = NULL;
}

int lengthOfAqueue(Aqueue* Q) {
  if (Q->front < Q->rear) {
    return Q->rear - Q->front;
  }
  else if (Q->front > Q->rear) {
    return AQUEUE_MAX_SIZE - Q->front + Q->rear;
  }
  else {
    return 0;
  }
}

int enqueueAqueue(Aqueue* Q, DataType elem) {
  if ((Q->rear + 1) % AQUEUE_MAX_SIZE == Q->front) {
    puts("cannot enqueue full queue!");
    return 1;
  }
  Q->elem[Q->rear] = elem;
  Q->rear = (Q->rear + 1) % AQUEUE_MAX_SIZE;
  return 0;
}

int dequeueAqueue(Aqueue* Q, DataType* deleted) {
  if (Q->front == Q->rear) {
    puts("cannot dequeue empty queue");
    return 1;
  }
  *deleted = Q->elem[Q->front];
  Q->front = (Q->front + 1) % AQUEUE_MAX_SIZE;
  return 0;
}

void visit(DataType elem) {
  printf("%d<", elem);
}

void traverseAqueue(Aqueue* Q, void (*visit)(DataType elem)) {
  for (int i = 0; i < lengthOfAqueue(Q); ++i) {
    visit(Q->elem[(Q->front + i) % AQUEUE_MAX_SIZE]);
  }
  putchar(10);
}

int main(int argc, char *argv[]) {
  Aqueue queue;
  DataType del;
  initAqueue(&queue);
  /* printf("%d\n", queue.front); */
  int i;
  for (i = 0; i < 8; ++i) {
    enqueueAqueue(&queue, i + 1);
    traverseAqueue(&queue, visit);
  }
  /* enqueueAqueue(&queue, 1); */
  /* enqueueAqueue(&queue, 2); */
  /* enqueueAqueue(&queue, 3); */
  for (i = 0; i < 4; ++i) {
    dequeueAqueue(&queue, &del);
    traverseAqueue(&queue, visit);
  }
  destroyAqueue(&queue);
  return 0;
}
