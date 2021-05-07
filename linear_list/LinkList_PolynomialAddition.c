#include <stdio.h>
#include <stdlib.h>
#define EPS 1e-9
#define DOUBLE_ISZERO(x) ((x) > -EPS && (x) < EPS)

typedef struct Term {
  double coef;
  int expn;
} DataType;

typedef struct Klist {
  DataType term;
  struct Klist* next;
} Klist;

void initKlist(Klist** L) {
  (*L) = (Klist*)malloc(sizeof(Klist));
  if (!(*L)) {
    puts("malloc failed");
    exit(1);
  }
  (*L)->term.coef = 0;
  (*L)->term.expn = 0;
  (*L)->next = NULL;
}

void destroyKlist(Klist** L) {
  Klist* head_ptr = (*L)->next;
  while (head_ptr != NULL) {
    Klist* temp = head_ptr;
    head_ptr = temp->next;
    printf("DEL coef:%g, expn:%d\n", temp->term.coef, temp->term.expn );
    free(temp);
    temp = NULL;
  }
  (*L) = NULL;
}


int insertElemKlist(Klist* L, int pos, DataType term) {
  int cur = 1;
  while (L->next != NULL && cur != pos) {
    L = L->next;
    cur++;
  }
  if (pos != cur) {
    puts("impossible position!");
    return -1;
  }
  Klist* new_node = (Klist*)malloc(sizeof(Klist));
  new_node->term = term;
  new_node->next = L->next;
  L->next = new_node;
  return cur;
}

int deleteElemKlist(Klist* L, int pos, DataType* deleted) {
  int cur = 0;
  while (L->next != NULL && cur != pos) {
    L = L->next;
    cur++;
  }
  if (pos != cur) {
    puts("impossible position!");
    return -1;
  }
  Klist* temp = L->next;
  L->next = temp->next;
  *deleted = temp->term;
  free(temp);
  temp = NULL;
  return cur;
}

void visit(DataType term) {
  printf("+ %gP^%d ", term.coef, term.expn);
}

void traverseKlist(Klist* L, void (*visit)(DataType term)) {
  while (L->next != NULL) {
    L = L->next;
    visit(L->term);
  }
  putchar(10);
}

int isEmptyKlist(Klist* L) {
  return L->next == NULL;
}

int compare(DataType term1, DataType term2) {
  return term1.expn == term2.expn;
}

int locateElemKlist(Klist* L, DataType term, int (*cmp)(DataType term1, DataType term2)) {
  int cur = 0;
  while (L->next != NULL && !cmp(term, L->term)) {
    L = L->next;
    cur++;
  }
  return cur;
}

int getElemKlist(Klist* L, int pos, DataType* term) {
  int cur = 0;
  while (L->next != NULL && cur != pos) {
    L = L->next;
    cur++;
  }
  if (cur == pos) {
    *term = L->term;
  }
  return cur;
}

int initKlistFromArray(Klist** L, DataType term[], int len) {
  initKlist(L);
  for (int i = 0; i < len; ++i) {
    insertElemKlist(*L, i + 1, term[i]);
  }
  return 0;
}

void polynomialAddition(Klist* poly1, Klist* poly2, Klist* result) {
  Klist* p1 = poly1->next;
  Klist* p2 = poly2->next;
  int index = 1;
  while (p1 != NULL) {
    insertElemKlist(result, index,p1->term);
    index++;
    p1 = p1->next;
  }
  while (p2 != NULL) {
    int pos = 1;
    Klist* head_ptr = result->next;
    while (head_ptr != NULL) {
      /* traverse result, merge or insert 'p2->term' */
      if (head_ptr->term.expn == p2->term.expn) {
        /* when current result exponent is equal to the term's exponent
         * add coefficient
         * */
        head_ptr->term.coef += p2->term.coef;
        if (DOUBLE_ISZERO(head_ptr->term.coef)) {
          DataType del;
          deleteElemKlist(result, pos - 1, &del);
        }
        break;
      }
      else if (head_ptr->term.expn > p2->term.expn) {
        /* when result exponent larger than the term we want inserted
         * insert before it
         * */
        insertElemKlist(result, pos, p2->term);
        break;
      }
      else if (head_ptr->next == NULL && head_ptr->term.expn < p2->term.expn) {
        /* when LAST result exponent still smaller than the term we want inserted
         * insert to the end of result
         * */
        insertElemKlist(result, pos + 1, p2->term);
        break;
      }
      pos++;
      head_ptr = head_ptr->next;
    }
    p2 = p2->next;
  }
}


int main(int argc, char *argv[]) {
  Klist* poly1; /* 3 0, 2 2 , 3 3 */
  Klist* poly2; /* 1 0, 2 1 , -2 2, 3 3 */
  Klist* poly3;
  DataType term1[3] = {{3, 0}, {2, 2}, {3, 3}};
  DataType term2[5] = {{1, 0}, {2, 1}, {-2, 2}, {3, 3}, {4.3, 4}};
  initKlistFromArray(&poly1, term1, 3);
  initKlistFromArray(&poly2, term2, 5);
  traverseKlist(poly1, visit);
  traverseKlist(poly2, visit);
  initKlist(&poly3);
  polynomialAddition(poly2, poly1, poly3);
  traverseKlist(poly3, visit);
  destroyKlist(&poly1);
  destroyKlist(&poly2);
  destroyKlist(&poly3);
  return 0;
}

