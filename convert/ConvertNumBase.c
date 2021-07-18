/*
 * convert 2~36 base to 2~36 base, support mantissa
 */
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BASE 36
char result[81];               // contains final convert result.
char integer_part_result[40];  // contains result's integer part
char mantissa_part_result[40]; // contains result's mantissa part
char base_table[MAX_BASE] = "0123456789abcdefghijklmnopqrstuvwxyz";

// find char's num, base on it's base
int baseNum(char ch) {
  int i;
  for (i = 0; i < MAX_BASE; i++)
    if (base_table[i] == ch)
      break;
  return (i == MAX_BASE) ? -1 : i;
}

// find the '.' index of num str
int splitPointPos(char num[]) {
  int i = 0;
  while (num[i] != '\0' && num[i] != '.')
    ++i;
  return (i == strlen(num)) ? -1 : i;
}

///////////////////////////////////////////////////// BaseN to Base10

//
int integerPartBaseNto10(char int_num[], int base) {
  int i, n, exp = strlen(int_num) - 1;
  n = 0;
  for (i = 0; int_num[i] != '\0'; ++i) {
    n += pow(base, exp - i) * baseNum(int_num[i]);
  }
  return n;
}

double mantissaPartBaseNto10(char int_num[], int base) {
  int i;
  double n = 0.0;
  for (i = 0; int_num[i] != '\0'; ++i) {
    n += pow(base, (i + 1) * (-1)) * baseNum(int_num[i]);
  }
  return n;
}

char *convertBaseNto10(char num[], int base_n) {
  if (base_n == 10) {
    strcpy(result, num);
    return result;
  }
  // using expansion method
  double n = 0.0;
  int p = splitPointPos(num);
  if (p == -1) {
    n = integerPartBaseNto10(num, base_n);
  } else {
    n += mantissaPartBaseNto10(&num[p + 1], base_n);
    num[p] = '\0';
    n += integerPartBaseNto10(num, base_n);
  }
  sprintf(result, "%g", n);
  return result;
}

///////////////////////////////////////////////////// Base10 to BaseX

//
void putCharAfterStr(char str[], int str_caps, char ch) {
  int str_len = strlen(str);
  if (str_len == str_caps) {
    printf("reach max str capacity!\n");
    return;
  }
  str[str_len] = ch;
  str[str_len + 1] = '\0';
}

void putCharBeforeStr(char str[], int str_caps, char ch) {
  int str_len = strlen(str);
  if (str_len == str_caps) {
    printf("reach max str capacity!\n");
    return;
  }
  int i;
  for (i = str_len; i >= 0; --i) {
    str[i + 1] = str[i];
  }
  str[0] = ch;
}

char *integerPartBase10toX(char int_num[], int base_x) {
  integer_part_result[0] = '\0';
  int n = atoi(int_num);
  while (n) {
    putCharBeforeStr(integer_part_result, 40, base_table[n % base_x]);
    n /= base_x;
  }
  return integer_part_result;
}

char *mantissaPartBase10toX(char int_num[], int base_x) {
  mantissa_part_result[0] = '\0';
  char temp[50];
  strcpy(temp, int_num);
  putCharBeforeStr(temp, 50, '.');
  putCharBeforeStr(temp, 50, '0');
  double n = atof(temp);
  int n_int_part = (int)n;
  while (n != (double)n_int_part) {
    n *= base_x;
    n_int_part = (int)n;
    putCharAfterStr(mantissa_part_result, 40, base_table[n_int_part]);
    n -= n_int_part;
  }
  return mantissa_part_result;
}

char *convertBase10toX(char num[], int base_x) {
  // using division method
  int p = splitPointPos(num);
  if (p == -1) {
    integerPartBase10toX(num, base_x);
    strcpy(result, integer_part_result);
  } else {
    mantissaPartBase10toX(&num[p + 1], base_x);
    num[p] = '\0';
    integerPartBase10toX(num, base_x);
    strcpy(result, integer_part_result);
    strcat(result, ".");
    strcat(result, mantissa_part_result);
  }
  return result;
}

///////////////////////////////////////////////////// BaseN to BaseX

char *convertBaseNtoX(char num[], int base_n, int base_x) {
  return convertBase10toX(convertBaseNto10(num, base_n), base_x);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s [num] [num_base(2~36)] [to_base(2~36)]\n", argv[0]);
    return 0;
  }
  int n = atoi(argv[2]);
  int x = atoi(argv[3]);
  if (n > 1 && n < 37 && x > 1 && x < 37) {
    char numstr_buffer[81];
    strcpy(numstr_buffer, argv[1]);
    int i = 0;
    while (numstr_buffer[i]) {
      numstr_buffer[i] = tolower(numstr_buffer[i]);
      ++i;
    }
    printf("%s\n", convertBaseNtoX(numstr_buffer, n, x));
  } else {
    printf("something went wrong in your parameters, please check it.\n");
  }
  return 0;
}
