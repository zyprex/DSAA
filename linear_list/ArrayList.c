#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ALIST_INIT_SIZE  4
#define ALIST_INCR_SIZE  1
#define ALIST_ERRNO_OVERFLOW 1
typedef int DataType;
typedef struct {
  DataType* elem; /* base address */
  int len;        /* length of list*/
  int capacity;   /* sizeof list  */
} Alist;          /* array list */

void initAlist(Alist* L) {
  L->elem = (DataType*)malloc(ALIST_INIT_SIZE * sizeof(DataType));
  if (!L->elem) {
    exit(ALIST_ERRNO_OVERFLOW);
  }
  L->len = 0;
  L->capacity = ALIST_INIT_SIZE;
}

void destroyAList(Alist* L) {
  /* !!!
   * Critical error detected c0000374
   * this error upcoming when I free memory twice
   */
  /* printf("Destroy Alist!\n"); */
  /* free(L); */
}

void clearAlist(Alist* L) {
  memset(L, 0, L->len * sizeof(DataType));
  L->len = 0;
}

int isEmptyAlist(Alist L) {
  return (L.len == 0);
}

int lengthOfAlist(Alist L) {
  return L.len;
}

int getElemAlist(Alist L, int pos, DataType* elem) {
  /* illegal postion */
  if (pos < 1 || pos > L.len + 1) {
    return 1;
  }
  *elem = L.elem[pos];
  return 0;
}

int compare(DataType a, DataType b) {
  return (a == b);
}

int locateElemAlist(Alist L, DataType elem, int (*compare)(DataType a, DataType b)) {
  int pos = 1;
  DataType* place = L.elem;
  while (pos <= L.len && !(*compare)(*(place++), elem)) {
    pos++;
  }
  if (pos <= L.len) {
    return pos;
  }
  return 0;
}

int insertElemAlist(Alist* L, int pos, DataType ins_elem) {
  /* illegal postion */
  if (pos < 1 || pos > L->len + 1) {
    return 1;
  }
  /* out of current capacity */
  if (L->len >= L->capacity) {
    DataType* newbase = (DataType *)realloc(L->elem,
                                            ((L->capacity + ALIST_INCR_SIZE) * sizeof(DataType)));
    if (!newbase) {
      exit(1);
    }
    L->elem = newbase;
    L->capacity += ALIST_INCR_SIZE;
  }
  /* postion place */
  DataType* place = &(L->elem[pos-1]);
  /* shift element to make new place */
  for (DataType* cur = &(L->elem[L->len - 1]); cur >= place; --cur) {
    *(cur+1) = *cur;
  }
  *place = ins_elem; /* insert ins_elem */
  L->len += 1; /* len++ */
  return 0;
}

int deleteElemAlist(Alist* L, int pos, DataType* deleted_elem) {
  /* illegal postion */
  if (pos < 1 || pos > L->len + 1) {
    return 1;
  }
  /* postion place */
  DataType* place = &(L->elem[pos-1]);
  *deleted_elem = *place;
  DataType* last = L->elem + L->len - 1;
  /* shift element to replace prev place */
  for (++place; place <= last; ++place) {
    *(place - 1)  = *place;
  }
  L->len -= 1; /* len-- */
  /* smaller than current capacity */
  if (L->capacity > ALIST_INIT_SIZE && L->len <= L->capacity - ALIST_INCR_SIZE) {
    DataType* newbase = (DataType *)realloc(L->elem,
                                            ((L->capacity - ALIST_INCR_SIZE) * sizeof(DataType)));
    if (!newbase) {
      exit(1);
    }
    L->elem = newbase;
    L->capacity -= ALIST_INCR_SIZE;
  }
  return 0;
}

void visit(DataType cur) {
  printf("(%d)", cur);
}

void traverseAlist(Alist L, void (*visit)(DataType cur)) {
  for (int i = 0; i < L.len; ++i) {
     (*visit)(L.elem[i]);
  }
  printf("-> %d\n", L.capacity);
}


int main(int argc, char *argv[])
{
  Alist a_list;
  DataType temp = 0;
  initAlist(&a_list);
  insertElemAlist(&a_list, 1, 1001);
  insertElemAlist(&a_list, 2, 1002);
  insertElemAlist(&a_list, 3, 1003);
  insertElemAlist(&a_list, 4, 1004);
  insertElemAlist(&a_list, 5, 1005);
  /* printf("%d\n", locateElemAlist(a_list, 1004, compare)); */
  traverseAlist(a_list, visit);
  deleteElemAlist(&a_list, 3, &temp);
  traverseAlist(a_list, visit);
  deleteElemAlist(&a_list, 3, &temp);
  /* printf("%d\n", temp); */
  traverseAlist(a_list, visit);
  /* destroyAList(&a_list); */
  return 0;
}

