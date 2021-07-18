/**
 * \file          BF.c
 * \brief         Brute Force string match algorithm
 */
#include <stdio.h>
#include <string.h>
int matchBF(const char* str, const char* pat) {
  int str_len = strlen(str);
  int pat_len = strlen(pat);
  int i, j;
  for (i = 0; i < str_len - pat_len; ++i) {
    for (j = 0; j < pat_len && str[i + j] == pat[j]; ++j) {}
    if (j == pat_len) {
      return i;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  printf("%d\n", matchBF("abcaabbcc", "aab"));
  return 0;
}
