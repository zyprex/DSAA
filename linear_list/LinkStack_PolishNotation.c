#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <limits.h> */

typedef char DataType[12];
typedef struct Kstack {
  DataType elem;
  struct Kstack* next;
} Kstack;

void initKstack(Kstack** S) {
  (*S) = (Kstack*)malloc(sizeof(Kstack));
  if (!(*S)) {
    puts("init malloc failed!");
    exit(1);
  }
  memset((*S)->elem, 0, sizeof(DataType));
  (*S)->next = NULL;

}

void destroyKstack(Kstack** S) {
  Kstack* top = *S;
  while (top->next != NULL) {
    Kstack* temp = top->next;
    top->next = top->next->next;
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
  strcpy(new_top->elem, elem);
  new_top->next = S->next;
  S->next = new_top;
  return 0;
}

int popKstack(Kstack* S, DataType deleted) {
  if (S->next == NULL) {
    puts("pop empty stack!");
    return -1;
  }
  Kstack* del = S->next;
  S->next = S->next->next;
  strcpy(deleted, del->elem);
  free(del);
  del = NULL;
  return 0;
}

void visit(DataType elem) {
  printf("%s|", elem);
}

void traverseKstack(Kstack* S, void (*visit)(DataType elem)) {
  while (S->next != NULL) {
    S = S->next;
    visit(S->elem);
  }
  putchar(10);
}

int isEmptyKstack(Kstack* S) {
  return S->next == NULL;
}

char* getTopKstack(Kstack* S) {
  if (S->next == NULL) {
    return "";
  }
  return S->next->elem;
}

int isOperator(char ch) {
  switch (ch) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '(':
    case ')':
      return 1;
    default:
      return 0;
  }
}

int operatorPriority(char* op) {
  switch (op[0]) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
    case '^':
      return 3;
    default:
      return 0;
  }
}

int opPriorCmp(char* op1, char* op2) {
  return operatorPriority(op1) - operatorPriority(op2);
}

int isNumber(char ch) {
  return ch >= '0' && ch <= '9';
}

int splitExprRTL(char* expr, int exprlen, int start, char* number, char* operator) {
  int i;
  strcpy(number, "");
  strcpy(operator, "");
  for (i = start; i >= 0; --i) {
    if (isOperator(expr[i])) {
      if (start - i > 0 && i < exprlen - 1) {
        sprintf(number, "%.*s", start - i, expr + i + 1);
        printf("%.*s\n", start - i, expr + i + 1);
      }
      sprintf(operator, "%c", expr[i]);
      printf("%c\n", expr[i]);
      return i;
    }
    /* first number */
    if (i == 0 && expr[0] != '(') {
      int j;
      for (j = 0; !isOperator(expr[j]); ++j) {}
      sprintf(number, "%.*s", j, expr);
      printf("%.*s\n", j, expr);
      return 0;
    }
  }
  return -1;
}

int splitExprLTR(char* expr, int exprlen, int start, char* number, char* operator) {
  int i;
  strcpy(number, "");
  strcpy(operator, "");
  for (i = start; i < exprlen; ++i) {
    if (isOperator(expr[i])) {
      if (i - start > 0 && i > 0) {
        sprintf(number, "%.*s", i - start, expr + start);
        printf("%.*s\n", i - start, expr + start);
      }
      sprintf(operator, "%c", expr[i]);
      printf("%c\n", expr[i]);
      return i;
    }
    /* last number */
    if (i == exprlen - 1 && expr[exprlen - 1] != ')') {
      int j;
      for (j = exprlen - 1; !isOperator(expr[j]); --j) {}
      sprintf(number, "%s", expr + j + 1);
      printf("%s\n", expr + j + 1);
      return exprlen;
    }
  }
  return -1;
}

void infix2prefix(char* expr) {
  int start, ret;
  char number[12], operator[2];
  int exprlen = strlen(expr);
  start = exprlen;

  Kstack* OP; // operator stack
  Kstack* CA; // calculate stack
  DataType del; // popped elem
  initKstack(&OP);
  initKstack(&CA);

  do {
    ret  = splitExprRTL(expr, exprlen, start, number, operator);
    start = ret - 1;

    if (*number != 0) {
      pushKstack(CA, number);
    }
    if (*operator != 0) {
      if (*operator == '(') {
        while (strcmp(getTopKstack(OP), ")") != 0 && !popKstack(OP, del)) {
          pushKstack(CA, del);
        }
        popKstack(OP, del);
      }
      else if (*operator == ')') {
        pushKstack(OP, operator);
      }
      else if (isEmptyKstack(OP)) {
        pushKstack(OP, operator);
      }
      else if (opPriorCmp(operator, getTopKstack(OP)) >=  0) {
        pushKstack(OP, operator);
      }
      else {
        while (strcmp(getTopKstack(OP), ")") != 0 && !popKstack(OP, del)) {
          pushKstack(CA, del);
        }
        pushKstack(OP, operator);
      }
    }
  } while (ret != -1);

  while (!popKstack(OP, del)) {
    pushKstack(CA, del);
  }

  traverseKstack(OP, visit);
  traverseKstack(CA, visit);
  destroyKstack(&OP);
  destroyKstack(&CA);
}

void infix2postfix(char* expr) {
  int start = 0, ret;
  char number[12], operator[2];
  int exprlen = strlen(expr);

  Kstack* OP; // operator stack
  Kstack* CA; // calculate stack
  DataType del; // popped elem
  initKstack(&OP);
  initKstack(&CA);

  do {
    ret = splitExprLTR(expr, exprlen, start, number, operator);
    start = ret + 1;

    if (*number != 0) {
      pushKstack(CA, number);
    }
    if (*operator != 0) {
      if (*operator == ')') {
        while (strcmp(getTopKstack(OP), "(") != 0 && !popKstack(OP, del)) {
          pushKstack(CA, del);
        }
        popKstack(OP, del);
      }
      else if (*operator == '(') {
        pushKstack(OP, operator);
      }
      else if (isEmptyKstack(OP)) {
        pushKstack(OP, operator);
      }
      else if (opPriorCmp(operator, getTopKstack(OP)) > 0) {
        pushKstack(OP, operator);
      }
      else {
        while (strcmp(getTopKstack(OP), "(") != 0 && !popKstack(OP, del)) {
          pushKstack(CA, del);
        }
        pushKstack(OP, operator);
      }
    }
  } while (ret != -1);

  while (!popKstack(OP, del)) {
    pushKstack(CA, del);
  }

  traverseKstack(OP, visit);
  traverseKstack(CA, visit);
  destroyKstack(&OP);
  destroyKstack(&CA);
}

int main(int argc, char *argv[]) {
  /* infix2prefix("1+9*2"); */
  /* infix2prefix("12.22+((2.121+3)*4)-5"); */
  infix2prefix("(12+3)*(7.8-115^2)+176.2/89.1+((1.2-45)+78)");
  /* infix2postfix("1+9*2"); */
  /* infix2postfix("1+((2+3)*4)-5"); */
  /* infix2postfix("(12+3)*(7.8-115^2)+176.2/89.1+((1.2-45)+78)"); */
  return 0;
}

