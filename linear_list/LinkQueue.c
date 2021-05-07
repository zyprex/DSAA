#include <stdio.h>
#include <stdlib.h>
typedef int DataType;
typedef struct Klist {
  DataType elem;
  struct Klist* next;
} Klist;

typedef struct Kqueue {
  Klist* front;
  Klist* rear;
} Kqueue;

void initKqueue(Kqueue** Q) {
  *Q = (Kqueue*)malloc(sizeof(Kqueue));
  (*Q)->front = (Klist*)malloc(sizeof(Klist));
  if (!(*Q)->front) {
    puts("malloc failed");
    exit(1);
  }
  (*Q)->rear = (*Q)->front;
  (*Q)->front->next = NULL;
}

void destroyKqueue(Kqueue** Q) {
  Klist* queue_head = (*Q)->front->next;
  while (queue_head != NULL) {
    Klist* temp = queue_head;
    queue_head = temp->next;
    printf("deleted: %d\n", temp->elem);
    free(temp);
  }
  (*Q)->front = NULL;
  (*Q)->rear = NULL;
}

int enqueueKqueue(Kqueue* Q, DataType elem) {
  Klist* new_node = (Klist*)malloc(sizeof(Klist));
  if (!new_node) {
    puts("malloc failed on new node");
    return 1;
  }
  new_node->elem = elem;
  new_node->next = NULL;
  Q->rear->next = new_node;
  Q->rear = new_node;
  return 0;
}

int dequeueKqueue(Kqueue* Q, DataType* deleted) {
  if (Q->rear == Q->front) {
    puts("dequeue empty queue!");
    return 1;
  }
  Klist* temp = Q->front->next;
  Q->front->next = temp->next;
  *deleted = temp->elem;
  if (temp == Q->rear) {
    Q->rear = Q->front;
  }
  free(temp);
  return 0;
}

void visit(DataType elem) {
  printf("%d<", elem);
}

void traverseKqueue(Kqueue* Q, void (*visit)(DataType elem)) {
  Klist* qhead = Q->front;
  while (qhead->next != NULL) {
    qhead = qhead->next;
    visit(qhead->elem);
  }
  putchar(10);
}

int main(int argc, char *argv[]) {
  Kqueue* queue;
  DataType del;
  initKqueue(&queue);
  enqueueKqueue(queue, 1);
  enqueueKqueue(queue, 2);
  enqueueKqueue(queue, 3);
  enqueueKqueue(queue, 4);
  enqueueKqueue(queue, 5);
  dequeueKqueue(queue, &del);
  traverseKqueue(queue, visit);
  destroyKqueue(&queue);
  return 0;
}

