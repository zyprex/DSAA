/*
 * convert Roman Digit to Arabic InDigit
 * I=1 V=5 X=10 L=50 C=100 D=500 M=1000
 */
#include <stdio.h>
#include <string.h>

const struct {
  char roman_val;
  int  num_val;
} roman_num_table[7] = {
    {'I', 1},   {'V', 5},   {'X', 10},   {'L', 50},
    {'C', 100}, {'D', 500}, {'M', 1000},
};

const struct {
  int num_val;
  char* roman_val;
} num_roman_table[13] = {
    {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
    {90, "XC"},  {50, "L"},   {40, "XL"}, {10, "X"},   {9, "IX"},
    {5, "V"},    {4, "IV"},   {1, "I"},
};

char result[80];

int romanDigitIndex(char roman_digit) {
  int i;
  for (i = 0; i < 7; ++i) {
    if (roman_digit == roman_num_table[i].roman_val) {
      return i;
    }
  }
  return -1;
}

int convertFromRomanDigit(char roman_digit[]) {
  int n = 0, i, index = -1, next_index = -1;
  for (i = 0; roman_digit[i] != '\0'; ++i) {
    index = romanDigitIndex(roman_digit[i]);
    next_index = romanDigitIndex(roman_digit[i + 1]);
    if (index >= next_index) {
      n += roman_num_table[index].num_val;
    } else {
      n -= roman_num_table[index].num_val;
    }
  }
  return n;
}

char* convertToRomanDigit(int num) {
  int i;
  result[0] = '\0';
  for (i = 0; i < 13; ++i) {
    while (num >= num_roman_table[i].num_val) {
      num -= num_roman_table[i].num_val;
      strcat(result, num_roman_table[i].roman_val);
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  /*
  printf("%d\n", convertFromRomanDigit("C"));   // 100
  printf("%d\n", convertFromRomanDigit("III"));   // 3
  printf("%d\n", convertFromRomanDigit("IV"));    // 4
  printf("%d\n", convertFromRomanDigit("IX"));    // 9
  printf("%d\n", convertFromRomanDigit("LVIII")); // 58
  printf("%d\n", convertFromRomanDigit("MCMXCIV")); // 1994
  */
  printf("%s\n", convertToRomanDigit(100));
  printf("%s\n", convertToRomanDigit(3));
  printf("%s\n", convertToRomanDigit(4));
  printf("%s\n", convertToRomanDigit(58));
  printf("%s\n", convertToRomanDigit(1994));
  return 0;
}
