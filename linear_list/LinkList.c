#include <stdio.h>
#include <stdlib.h>
typedef int DataType;
typedef struct Klist {
  DataType data;
  struct Klist* next;
} Klist;

void initKlist(Klist** L) {
  *L = (Klist*)malloc(sizeof(Klist));
  if (!(*L)) {
    puts("malloc failed");
    exit(1);
  }
  (*L)->data = 9999;
  (*L)->next = NULL;
}

void destroyKlist(Klist** L) {
  Klist* p = *L;
  while (p->next != NULL) {
    Klist* temp = p->next;
    p->next = p->next->next;
    printf("deleted: %d\n", temp->data);
    free(temp);
    temp = NULL;
  }
  /* printf("deleted(head): %d\n", p->data); */
  *L = NULL;
}

int insertElemKlist(Klist* L, int pos, DataType elem) {
  int cur = 1; /* head Node is 0, start insert on first pos */
  while (L->next != NULL && cur != pos) {
    L = L->next;
    cur++;
  }
  if (cur != pos) {
    puts("WHAT? IMPOSSIBLE POSTION!");
    return -1;
  }
  Klist* new_node = (Klist*)malloc(sizeof(Klist));
  new_node->data = elem;
  new_node->next = L->next;
  L->next = new_node;
  return cur;
}

int deleteElemKlist(Klist* L, int pos, DataType* deleted) {
  int cur = 0; /* actual position start from 0 */
  while (L->next != NULL && cur != pos) {
    L = L->next;
    cur++;
  }
  if (cur != pos) {
    puts("WHAT? IMPOSSIBLE POSTION!");
    return -1;
  }
  Klist* temp = L->next;
  L->next = L->next->next;
  printf("deleted: %d\n", temp->data);
  *deleted = temp->data;
  free(temp);
  temp = NULL;
  return cur;
}

void visit(DataType cur) {
  printf("(%d)", cur);
}

void traverseKlist(Klist* L, void (*visit)(DataType cur)) {
  visit(L->data);
  while (L->next != NULL) {
    L = L->next;
    visit(L->data);
  }
  putchar(10);
}

int isEmptyKlist(Klist* L) {
  return L->next == NULL;
}

int lengthOfKlist(Klist* L) {
  int len = 0; /* exclude head node */
  while (L->next != NULL) {
    L = L->next;
    len++;
  }
  return len;
}

int compare(DataType a, DataType b) {
  return a == b;
}

int locateElemKlist(Klist* L, DataType elem, int (*cmp)(DataType, DataType)) {
  int cur = 0;
  while (L->next != NULL && !cmp(L->data, elem)) {
    L = L->next;
    cur++;
  }
  return cur;
}


int getElemKlist(Klist* L, int pos, DataType* elem) {
  int cur = 0;
  while (L->next != NULL && cur != pos) {
    L = L->next;
    cur++;
  }
  if (cur == pos) {
    *elem = L->data;
  }
  return cur;
}

Klist* revertKlist(Klist* L) {
  Klist* old = L->next; /* pick 2nd node */
  L->next = NULL;       /* break head node */
  while (old != NULL) {
    Klist* pre = old;
    old = old->next;
    pre->next = L->next; /* jump one head node */
    L->next = pre;
  }
  return L;
}

int main(int argc, char *argv[]) {
  Klist* link_list = NULL;
  initKlist(&link_list);
  for (int i = 1; i <= 5; ++i) {
    insertElemKlist(link_list, i, 1000 + i);
  }
  /* printf("%d\n", lengthOfKlist(link_list)); */
  /* DataType del; */
  /* deleteElemKlist(link_list, 1, &del); */
  /* printf("%d\n", locateElemKlist(link_list, 1002, compare)); */
  /* DataType elem; */
  /* getElemKlist(link_list, 1, &elem); */
  /* printf("%d\n", elem); */
  traverseKlist(link_list, visit);

  Klist* link_list_r = revertKlist(link_list);
  traverseKlist(link_list_r, visit);

  destroyKlist(&link_list);
  /* destroyKlist(&link_list_r); */
  return 0;
}
