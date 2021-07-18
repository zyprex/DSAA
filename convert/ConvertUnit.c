/*
 * convert many measurement with human understandable unit.
 */
#include <stdio.h>
#include <string.h>

char line[80];
char unit_str[12];
double src_val;
#define UNIT_CONV_TABLE_LEN 20
#define HUU_TABLE_LEN 10
#define IN_GAP(x, y, m) (x > y ? (x - y < m) : (y - x < m))

/* classes of units */
typedef enum {
  TEMPERATURE,
  LENGTH,
  DATA,
} UnitClass;

typedef struct UnitDesc {
  char word_less[20];           /* string describe less number */
  char word_more[20];           /* string describe more number*/
  double gap;                   /* range of acceptable value
                                   to compare with standard datum */
} UnitDesc;

typedef struct Huu {
  UnitClass class;             /* class of unit */
  double value;                /* value of standard datum */
  char*  name[20];             /* corresponding unit names */
  char desc[128];              /* description of this datum */
} Huu;

typedef struct UnitElem {
  UnitClass class;              /* class of unit */
  char* name[20];               /* unit's names */
  void (*convFunc)();           /* convert function */
} UnitElem;

#define CONV_C_F(c) (c * 9 / 5.0 + 32)
#define CONV_F_C(f) ((f - 32) * 5.0 / 9)
#define CONV_INCH_CM(in) (in * 2.54)
#define CONV_FOOT_M(ft) (ft * 0.3048)
#define CONV_YARD_M(yd) (yd * 0.9144)
#define CONV_MILE_KM(mi) (mi * 1.609)

#define CONV_CM_INCH(cm) (cm * 0.3937)
#define CONV_CM_M(cm) (cm / 100)

#define CONV_M_FOOT(m) (m * 3.28084)
#define CONV_M_YARD(m) (m * 1.09361)
#define CONV_M_CM(m) (m * 100)

#define CONV_KM_MILE(km) (km * 0.62137)
#define CONV_KM_M(km) (km * 1000)


#define PRINT_CONV_RESULT(unit1, unit2, fn)                                    \
  printf("%.2lf " unit1 " = %.2lf " unit2 "\n", src_val, fn(src_val))

void convCtoF() { PRINT_CONV_RESULT("Celsius", "Fahreneit", CONV_C_F); }
void convFtoC() { PRINT_CONV_RESULT("Fahreneit", "Celsius", CONV_F_C); }
void convInchToCM() { PRINT_CONV_RESULT("inch", "cm", CONV_INCH_CM); }
void convFootToM() { PRINT_CONV_RESULT("foot", "m", CONV_FOOT_M); }
void convYardToM() { PRINT_CONV_RESULT("yard", "m", CONV_YARD_M); }
void convMileToKM() { PRINT_CONV_RESULT("mile", "km", CONV_MILE_KM); }
void convCMToX() {
  PRINT_CONV_RESULT("cm", "inch", CONV_CM_INCH);
  PRINT_CONV_RESULT("cm", "m", CONV_CM_M);
}
void convMToX() {
  PRINT_CONV_RESULT("m", "foot", CONV_M_FOOT);
  PRINT_CONV_RESULT("m", "yard", CONV_M_YARD);
  PRINT_CONV_RESULT("m", "cm", CONV_M_CM);
}
void convKMToX() {
  PRINT_CONV_RESULT("km", "mile", CONV_KM_MILE);
  PRINT_CONV_RESULT("km", "m", CONV_KM_M);
}
void convBitChain(int level) {
  switch (level) {
  case 0:
    printf("%.1lf Bits\n",  src_val);
    printf("\t= %.1lf Bytes\n", src_val / 8);
    printf("\t= %.1lf KB\n", src_val / 8 / 1024);
    printf("\t= %.1lf MB\n", src_val / 8 / 1024 / 1024);
    printf("\t= %.1lf GB\n", src_val / 8 / 1024 / 1024 / 1024);
    printf("\t= %.1lf TB\n", src_val / 8 / 1024 / 1024 / 1024 / 1024);
    break;
  case 1:
    /* TODO */
    printf("\t= %.1lf Bits\n",  src_val * 8);
    printf("%.1lf Bytes\n", src_val);
    printf("\t= %.1lf KB\n", src_val / 1024);
    printf("\t= %.1lf MB\n", src_val / 1024 / 1024);
    printf("\t= %.1lf GB\n", src_val / 1024 / 1024 / 1024);
    printf("\t= %.1lf TB\n", src_val / 1024 / 1024 / 1024 / 1024);
    break;
  case 2:
    printf("\t= %.1lf Bits\n",  src_val * 8 * 1024);
    printf("\t= %.1lf Bytes\n", src_val * 1024);
    printf("%.1lf KB\n", src_val);
    printf("\t= %.1lf MB\n", src_val / 1024);
    printf("\t= %.1lf GB\n", src_val / 1024 / 1024);
    printf("\t= %.1lf TB\n", src_val / 1024 / 1024 / 1024);
    break;
  case 3:
    printf("\t= %.1lf Bits\n",  src_val * 8 * 1024 * 1024);
    printf("\t= %.1lf Bytes\n", src_val * 1024 * 1024);
    printf("\t= %.1lf KB\n", src_val * 1024);
    printf("%.1lf MB\n", src_val);
    printf("\t= %.1lf GB\n", src_val / 1024);
    printf("\t= %.1lf TB\n", src_val / 1024 / 1024);
    break;
  case 4:
    printf("\t= %.1lf Bits\n",  src_val * 8 * 1024 * 1024 * 1024);
    printf("\t= %.1lf Bytes\n", src_val * 1024 * 1024 * 1024);
    printf("\t= %.1lf KB\n", src_val * 1024 * 1024);
    printf("\t= %.1lf MB\n", src_val * 1024);
    printf("%.1lf GB\n", src_val);
    printf("\t= %.1lf TB\n", src_val / 1024);
    break;
  case 5:
    printf("\t= %.1lf Bits\n",  src_val * 8 * 1024 * 1024 * 1024 * 1024);
    printf("\t= %.1lf Bytes\n", src_val * 1024 * 1024 * 1024 * 1024);
    printf("\t= %.1lf KB\n", src_val * 1024 * 1024 * 1024);
    printf("\t= %.1lf MB\n", src_val * 1024 * 1024);
    printf("\t= %.1lf GB\n", src_val * 1024);
    printf("%.1lf TB\n", src_val);
    break;
  default:
    break;
  }
}
void convBitChain0() { convBitChain(0); }
void convBitChain1() { convBitChain(1); }
void convBitChain2() { convBitChain(2); }
void convBitChain3() { convBitChain(3); }
void convBitChain4() { convBitChain(4); }
void convBitChain5() { convBitChain(5); }

#define EXPAND_UNIT_CONV_ITEM_TEMP(n, fn)                                      \
  { .class = TEMPERATURE, .name = n, .convFunc = fn }
#define EXPAND_UNIT_CONV_ITEM_LEN(n, fn)                                      \
  { .class = LENGTH, .name = n, .convFunc = fn }
#define EXPAND_UNIT_CONV_ITEM_DATA(n, fn)                                      \
  { .class = DATA, .name = n, .convFunc = fn }

#define SYM_CELSIUS {"Celsius", "c"}
#define SYM_FAHRENEIT {"Fahreneit", "f"}
#define SYM_INCH {"Inch", "in"}
#define SYM_YARD {"Yard", "yd"}
#define SYM_FOOT {"Foot", "ft"}
#define SYM_MILE {"Mile", "mi"}
#define SYM_METRE {"M", "m", "metre"}
#define SYM_CENTIMETRE {"CM", "cm", "centimetre"}
#define SYM_KILOMETRE {"KM", "km", "kilometre"}
#define SYM_BIT {"bit", "Bit", "b"}
#define SYM_BYTE {"byte", "Byte", "B"}
#define SYM_KB {"Kilobyte", "KB"}
#define SYM_MB {"Megabyte", "MB"}
#define SYM_GB {"Gigabyte", "GB"}
#define SYM_TB {"Terabyte", "TB"}

UnitElem unit_conv_table[UNIT_CONV_TABLE_LEN] = {
    EXPAND_UNIT_CONV_ITEM_TEMP(SYM_CELSIUS, convCtoF),
    EXPAND_UNIT_CONV_ITEM_TEMP(SYM_FAHRENEIT, convFtoC),
    EXPAND_UNIT_CONV_ITEM_LEN(SYM_INCH, convInchToCM),
    EXPAND_UNIT_CONV_ITEM_LEN(SYM_YARD, convYardToM),
    EXPAND_UNIT_CONV_ITEM_LEN(SYM_FOOT, convFootToM),
    EXPAND_UNIT_CONV_ITEM_LEN(SYM_MILE, convMileToKM),
    EXPAND_UNIT_CONV_ITEM_LEN(SYM_METRE, convMToX),
    EXPAND_UNIT_CONV_ITEM_LEN(SYM_CENTIMETRE, convCMToX),
    EXPAND_UNIT_CONV_ITEM_LEN(SYM_KILOMETRE, convKMToX),
    EXPAND_UNIT_CONV_ITEM_DATA(SYM_BIT, convBitChain0),
    EXPAND_UNIT_CONV_ITEM_DATA(SYM_BYTE, convBitChain1),
    EXPAND_UNIT_CONV_ITEM_DATA(SYM_KB, convBitChain2),
    EXPAND_UNIT_CONV_ITEM_DATA(SYM_MB, convBitChain3),
    EXPAND_UNIT_CONV_ITEM_DATA(SYM_GB, convBitChain4),
    EXPAND_UNIT_CONV_ITEM_DATA(SYM_TB, convBitChain5),
};

#define EXPAND_HUU_ITEM_TEMP(n, v, d)                                          \
  { .class = TEMPERATURE, .name = n, .value = v, .desc = d }
/* #define EXPAND_HUU_ITEM_LEN(n, v, d)                                          \ */
/*   { .class = LENGTH, .name = n, .value = v, .desc = d } */
/* #define EXPAND_HUU_ITEM_DATA(n, v, d)                                          \ */
/*   { .class = DATA, .name = n, .value = v, .desc = d } */
Huu huu_table[HUU_TABLE_LEN] = {
  EXPAND_HUU_ITEM_TEMP(SYM_CELSIUS, 100, "water's boiling point"),
  EXPAND_HUU_ITEM_TEMP(SYM_FAHRENEIT, 212, "water's boiling point"),
};

UnitDesc desc_less_or_more[] = {
    {.word_less = "Colder", .word_more = "Hotter", .gap = 10},
    {.word_less = "Less", .word_more = "More", .gap = 10},
};

void printAllUnitsName() {
  int i;
  for (i = 0; i < UNIT_CONV_TABLE_LEN; ++i) {
    int j = 0;
    while (unit_conv_table[i].name[j] != NULL) {
      printf("%s ", unit_conv_table[i].name[j]);
      j++;
      if (unit_conv_table[i].name[j] == NULL) {
        putchar(10);
      }
    }
  }
}

// print human understandable unit description base on your input
void printHuuDesc(UnitElem* test_data) {
  int i;
  UnitDesc desc = desc_less_or_more[test_data->class];
  for (i = 0; i < HUU_TABLE_LEN; ++i) {
    int j = 0;
    while (test_data->name[j] != NULL) {
      int k = 0;
      while (huu_table[i].name[k] != NULL) {
        /* same class and same unit name */
        if (test_data->class == huu_table[i].class
            && !strcmp(test_data->name[j], huu_table[i].name[k])) {
          if (IN_GAP(src_val, huu_table[i].value, desc.gap)) {
            if (src_val < huu_table[i].value) {
              printf("%s than ", desc.word_less);
            } else if (src_val > huu_table[i].value){
              printf("%s than ", desc.word_more);
            }
            printf("%s\n", huu_table[i].desc);
            return;
          }
        }
        k++;
      }
      j++;
    }
  }
}

int main(int argc, char *argv[]) {
  int ret;
  puts("For possible units, input 'list'\n"
       "For help, input 'help'\n"
       "For quit, input 'quit' 'exit' or hit Enter key.");
  do {
    printf("> ");
    fgets(line, sizeof(line), stdin);
    if (!strcmp(line, "list\n")) {
      printAllUnitsName();
      continue;
    }
    if (!strcmp(line, "help\n")) {
      puts("usage: [value][unit]");
      continue;
    }
    if (!strcmp(line, "quit\n")
        || !strcmp(line, "exit\n")
        || line[0] == '\n') {
      return 0;
    }
    ret = sscanf(line, "%lf%s", &src_val, unit_str);
    if (ret < 2) {
      printf("ERROR INPUT, PLEASE CHECK YOUR INPUT FORMAT!\n");
      continue;
    }
    int i, founded = 0;
    for (i = 0; i < UNIT_CONV_TABLE_LEN; ++i) {
      int j = 0;
      while (unit_conv_table[i].name[j] != NULL) {
        if (!strcmp(unit_conv_table[i].name[j], unit_str)) {
          unit_conv_table[i].convFunc(unit_conv_table[i]);
          printHuuDesc(&unit_conv_table[i]);
          founded = 1;
          break;
        }
        j++;
      }
    }
    if (!founded) {
      printf("UNKNOW UNIT OF '%s', USE COMMAND 'list' SEE POSSIBLE UNIT\n",
             unit_str);
    }
  } while(1);
  return 0;
}
