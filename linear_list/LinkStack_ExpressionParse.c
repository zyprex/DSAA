/*
 * Ref_1: https://www.programmingoneonone.com/2020/05/polish-notation.html
 * Ref_2: https://www.cnblogs.com/chensongxian/p/7059802.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* #define DEBUG */
#ifdef DEBUG
#  define D(x) x
#else
#  define D(x)
#endif

typedef char DataType[64]; /* increase it when you meets segment fault! */
typedef struct Kstack {
  DataType elem;
  struct Kstack* next;
} Kstack; /* Link Stack */

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
    D(puts("pop empty stack!"));
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

int splitExprRTL(char* expr, int exprlen, int start, char* number, char* operator) {
  int i;
  strcpy(number, "");
  strcpy(operator, "");
  for (i = start; i >= 0; --i) {
    if (isOperator(expr[i])) {
      if (start - i > 0 && i < exprlen - 1) {
        sprintf(number, "%.*s", start - i, expr + i + 1);
        D(printf("%.*s\n", start - i, expr + i + 1));
      }
      sprintf(operator, "%c", expr[i]);
      D(printf("%c\n", expr[i]));
      return i;
    }
    /* first number */
    if (i == 0 && expr[0] != '(') {
      int j;
      for (j = 0; !isOperator(expr[j]); ++j) {}
      sprintf(number, "%.*s", j, expr);
      D(printf("%.*s\n", j, expr));
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
        D(printf("%.*s\n", i - start, expr + start));
      }
      sprintf(operator, "%c", expr[i]);
      D(printf("%c\n", expr[i]));
      return i;
    }
    /* last number */
    if (i == exprlen - 1 && expr[exprlen - 1] != ')') {
      int j;
      for (j = exprlen - 1; !isOperator(expr[j]); --j) {}
      sprintf(number, "%s", expr + j + 1);
      D(printf("%s\n", expr + j + 1));
      return exprlen;
    }
  }
  return -1;
}

Kstack* infix2prefix(char* expr) {
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

  D(traverseKstack(OP, visit));
  D(traverseKstack(CA, visit));
  destroyKstack(&OP);
  /* destroyKstack(&CA); */
  return CA;
}

Kstack* infix2postfix(char* expr) {
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

  D(traverseKstack(OP, visit));
  D(traverseKstack(CA, visit));
  destroyKstack(&OP);
  /* destroyKstack(&CA); */
  return CA;
}

/* convert float number to ascii string */
char* ftoa(double num) {
  char* str_num = (char*)malloc(sizeof(DataType));
  sprintf(str_num, "%lf", num);
  return str_num;
}

/* evaluate string number expression */
char* evalStrNum(char* left, char* right, char* op) {
  double num_left = atof(left);
  double num_right = atof(right);
  double result;
  switch (op[0]) {
    case '+':
      result = num_left + num_right;
      break;
    case '-':
      result = num_left - num_right;
      break;
    case '*':
      result = num_left * num_right;
      break;
    case '/':
      result = num_left / num_right;
      break;
    case '^':
      result = pow(num_left, num_right);
      break;
    default:
      break;
  }
  return ftoa(result);
}

/* evaluate string expression to number result (use prefix) */
double exprParsePrefix(char* expr) {
  double result;
  DataType left, right, op;
  Kstack* CA = infix2prefix(expr);
  Kstack* OP;
  DataType del;
  initKstack(&OP);
  while (1) {
    if (isEmptyKstack(CA)) {
      puts("empty expression!");
      destroyKstack(&OP);
      destroyKstack(&CA);
      return 0;
    }
    if (CA->next->next == NULL) {
      result = atof(CA->next->elem);
      destroyKstack(&OP);
      destroyKstack(&CA);
      return result;
    }
    if (CA->next->next->next == NULL && isEmptyKstack(OP)) {
      puts("invalid expression!");
      destroyKstack(&OP);
      destroyKstack(&CA);
      return 0;
    }
    if (CA->next->next->next == NULL && !isEmptyKstack(OP)) {
      popKstack(OP, del);
      pushKstack(CA, del);
    }
    char ch1 = getTopKstack(CA)[0];
    char ch2 = CA->next->next->elem[0];
    char ch3 = CA->next->next->next->elem[0];
    if (isOperator(ch1)) {
      if (!isOperator(ch2) && !isOperator(ch3)) {
        popKstack(CA, del);
        strcpy(op, del);
        popKstack(CA, del);
        strcpy(left, del);
        popKstack(CA, del);
        strcpy(right, del);
        pushKstack(CA, evalStrNum(left, right, op));
        if (!popKstack(OP, del)) {
          pushKstack(CA, del);
        }
      }
      else {
        popKstack(CA, del);
        pushKstack(OP, del);
      }
    }
    else {
      popKstack(CA, del);
      pushKstack(OP, del);
    }
  }
  destroyKstack(&OP);
  destroyKstack(&CA);
  return 0;
}

/* evaluate string expression to number result (use postfix) */
double exprParsePostfix(char* expr) {
  double result;
  DataType left, right, op;
  Kstack* CA = infix2postfix(expr);
  Kstack* OP;
  DataType del;
  initKstack(&OP);
  // TODO:...
      /* traverseKstack(CA, visit); */
      /* destroyKstack(&OP); */
      /* destroyKstack(&CA); */
      /* return 0; */
  while (1) {
    if (isEmptyKstack(CA)) {
      puts("empty expression!");
      destroyKstack(&OP);
      destroyKstack(&CA);
      return 0;
    }
    if (CA->next->next == NULL) {
      result = atof(CA->next->elem);
      destroyKstack(&OP);
      destroyKstack(&CA);
      return result;
    }
    if (CA->next->next->next == NULL && isEmptyKstack(OP)) {
      puts("invalid expression!");
      destroyKstack(&OP);
      destroyKstack(&CA);
      return 0;
    }
    if (CA->next->next->next == NULL && !isEmptyKstack(OP)) {
      popKstack(OP, del);
      pushKstack(CA, del);
    }
    char ch1 = getTopKstack(CA)[0];
    char ch2 = CA->next->next->elem[0];
    char ch3 = CA->next->next->next->elem[0];
    if (isOperator(ch1)) {
      if (!isOperator(ch2) && !isOperator(ch3)) {
        popKstack(CA, del);
        strcpy(op, del);
        popKstack(CA, del);
        strcpy(right, del);
        popKstack(CA, del);
        strcpy(left, del);
        pushKstack(CA, evalStrNum(left, right, op));
        if (!popKstack(OP, del)) {
          pushKstack(CA, del);
        }
      }
      else {
        popKstack(CA, del);
        pushKstack(OP, del);
      }
    }
    else {
      popKstack(CA, del);
      pushKstack(OP, del);
    }
  }
  destroyKstack(&OP);
  destroyKstack(&CA);
  return 0;
}

int main(int argc, char *argv[]) {
  /* double double_number = 1.233456; */
  /* char* n = ftoa(double_number); */
  /* printf("%f\n", double_number); */
  /* printf("%s\n", n); */
  /* free(n); */
  /* printf("%f\n", exprParsePrefix("7-10")); */
  if (argc > 1) {
    /* NOTICE: when use '^', quote expression with '"' */
    printf("%g", exprParsePrefix(argv[1]));
    /* printf("%g", exprParsePostfix(argv[1])); */
  }
  /* if (argc > 2) { */
    /* printf("::%s", (argv[2])); */
  /* } */
  /* printf("%lf\n", exprParsePrefix("4^0.5")); */
  /* printf("%lf\n", exprParsePrefix("(1+)^(1/2)")); */
  /* printf("%lf\n", exprParsePrefix("4.578*123456/4")); */
  /* printf("%lf\n", pow(4,0.5)); */
  /* printf("%lf\n", pow(0.5, 4)); */
  return 0;
}
