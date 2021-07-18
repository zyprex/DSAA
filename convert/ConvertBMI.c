/*
 * WHO: Body Mass Index
 *   Severely underweight  <= 16.4
 *   Underweight  16.5 ~ 18.4
 *   Normal  18.5 ~ 24.9
 *   Overweight  25.0 ~ 29.9
 *   Obese class I  30.0 ~ 34.9
 *   Obese class I  35.0 ~ 39.0
 *   Obese class III  >= 40.0
 */
#include <stdio.h>
/* #include <stdlib.h> */
int main(int argc, char *argv[]) {
  double weight, height, bmi;
  printf("---Calculate Your BMI (WHO standard)---\n");
  printf("Input Your Weight(kg):");
  scanf("%lf", &weight);
  printf("Input Your Height(m):");
  scanf("%lf", &height);
  bmi = weight / (height * height) * 10;
  switch ((int)bmi) {
  case   0 ... 164: printf("Severely underweight\n"); break;
  case 165 ... 184: printf("Underweight\n"); break;
  case 185 ... 249: printf("Normal\n"); break;
  case 250 ... 299: printf("Overweight\n"); break;
  case 300 ... 349: printf("Obese class I\n"); break;
  case 350 ... 390: printf("Obese class II\n"); break;
  default: printf("Obese class III\n"); break;
  }
  /* system("pause"); */
  return 0;
}
