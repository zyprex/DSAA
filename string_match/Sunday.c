/**
 * \file          Sunday.c
 * \brief         Sunday string match algorithm
 */
#include <stdio.h>
#include <string.h>

int matchSunday(char* str, char* pat) {
  int str_len = strlen(str);
  int pat_len = strlen(pat);
  int i, j;
  for (i = 0; i <= str_len - pat_len; ) {
    for (j = 0; j < pat_len && str[j + i] == pat[j]; ++j) {}
    if (j == pat_len) {
      return i;
    }
    for (j = pat_len - 1; j >= 0; --j) {
      if (pat[j] == str[i + pat_len]) {
        break;
      }
    }
    i += pat_len - j;
  }
  return -1;
}

int main(int argc, char *argv[]) {
  printf("%d\n", matchSunday("abcaabbcc", "aab"));
  return 0;
}
