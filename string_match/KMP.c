/**
 * \file          KMP.c
 * \brief         Knuth-Morris-Pratt string match algorithm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void genNextTable(char* pat, int pat_len, int* next) {
  int i = 0, j = -1;
  next[0] = -1;
  while (i < pat_len -1) {
    if (j == -1 || pat[i] == pat[j]) {
      next[++i] = ++j;
    }
    else {
      /* mismatch, so we move j index */
      j = next[j];
    }
  }
  /* print next array */
  for (i = 0; i < pat_len; ++i) {
    printf("%3c", pat[i]);
  }
  putchar(10);
  for (i = 0; i < pat_len; ++i) {
    printf("%3d", next[i]);
  }
  putchar(10);
}

void matchKMP(char* str, char* pat) {
  int str_len = strlen(str);
  int pat_len = strlen(pat);
  int* next = (int*)malloc(pat_len * sizeof(int));
  genNextTable(pat, pat_len, next);
  int i = 0, j = 0;
  while (i < str_len && j < pat_len) {
    if (j == -1 || str[i] == pat[j]) {
      ++i;
      ++j;
    }
    else {
      j = next[j];
    }
  }
  free(next);
  if (j == pat_len) {
    printf("%d\n", i - j);
  }
  return;
}

int main(int argc, char *argv[]) {
  matchKMP("gtgtgagctggtgtgtgcfaa", "gtgtgcf");
  return 0;
}
