/*
 * URL encode and decode
 *   ref: https://www.cnblogs.com/quliuliu2013/p/9915288.html
 *
 *   NOTE: for chinese character representations, please set file encoding
 *   to cp936(gb2312), use UTF-8 will cause `urlDecode` malfunction!
 *
 *   Last File Encoding: UTF-8
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void urlEncode(char str[], char out[], size_t str_len, size_t out_len) {
  int i, j;
  for (i = 0 , j = 0; i < str_len && j < out_len; ++i, ++j) {
    if (isalnum(str[i])
        || str[i] == '-' || str[i] == '_'
        || str[i] == '~' || str[i] == '.') {
      out[j] = str[i];
    }
    else {
      // encode char
      char hex_bit[4] = "";
      sprintf(hex_bit, "%%%02X", str[i] >= 0 ? str[i] : (str[i] & 0xFF));
      strncat(out, hex_bit, out_len);
      j = strlen(out) - 1;
    }
  }
}

int hex2dec(char c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  }
  else if ('a' <= c && c <= 'f') {
    return c - 'a' + 10;
  }
  else if ('A' <= c && c <= 'F') {
    return c - 'A' + 10;
  }
  else {
    return -1;
  }
}

void urlDecode(char str[], char out[], size_t str_len, size_t out_len) {
  int i, j;
  for (i = 0, j = 0; i < str_len && j < out_len; ++i, ++j) {
     if (str[i] != '%') {
       out[j] = str[i];
     }
     else {
       // decode char
       char c1 = str[++i];
       char c0 = str[++i];
       int num = 0;
       num = hex2dec(c1) * 16 + hex2dec(c0);
       printf("%x\n", num);
       out[j] = num;
     }
  }
}

int main(int argc, char *argv[]) {
  char url[100] = "s阿在e";
  char url_encode[300] = "";
  char url_decode[300] = "";
  urlEncode(url, url_encode, strlen(url), sizeof(url_encode));
  printf("%s\n", url_encode); // utf-8 => s%E9%98%BF%E5%9C%A8e
  urlDecode(url_encode, url_decode, strlen(url_encode), sizeof(url_decode));
  printf("%s\n", url_decode); // ?
  urlDecode("s%B0%a2%d4%Dae", url_decode,
      strlen(url_encode), sizeof(url_decode)); // gb2312 => s%B0%A2%D4%DAe
  printf("%s\n", url_decode);
  return 0;
}
