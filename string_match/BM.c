/**
 * \file          BM.c
 * \brief         Boyer Moore string match algorithm
 *                complete BM algorithm require another
 *                good suffix table, but in this case
 *                we only use bad char table.
 */
#include <stdio.h>
#include <string.h>
#define CHAR_MAX 0xff
#define MAX(x, y) ((x) > (y) ? (x) : (y))

void genBadCharTable(char* pat,  int pat_len, int* bad_char_table) {
  int i;
  for (i = 0; i < CHAR_MAX; ++i) {
    bad_char_table[i] = -1;
  }
  for (i = 0; i < pat_len; ++i) {
    bad_char_table[(int)pat[i]] = pat_len - i;
  }
}

void matchBM(char* str, char* pat) {
  int str_len = strlen(str);
  int pat_len = strlen(pat);
  int bad_char_table[CHAR_MAX];
  genBadCharTable(pat, pat_len, bad_char_table);
  int shift = 0;
  while (shift <= (str_len - pat_len)) {
    int match_pos = pat_len - 1;
    while (match_pos >= 0 && pat[match_pos] == str[match_pos + shift]) {
      match_pos--;
    }
    if (match_pos < 0) {
      printf("match at %d\n", shift);
      break;
    }
    shift += MAX(1, bad_char_table[(int)pat[match_pos]]);
  }
}

int main(int argc, char *argv[]) {
  matchBM("bcbdcabdca", "cabdca");
  return 0;
}
