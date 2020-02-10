
#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

using namespace std;
int main()
{
  printf(" -------------------------------------------------------------------------------");
  printf("\n|%-19s    |%-4s   |%-18s     | %-17s     | ", "type", "size", "min", "max");
  printf("\n -------------------------------------------------------------------------------");
  printf("\n|%-19s    |%-4d   |%-18d     | %-17d     | ", "char", sizeof(char), CHAR_MIN, CHAR_MAX);
  printf("\n|%-19s    |%-4d   |%-20hhu   | %-19hhu   | ", "unsigned char", sizeof(unsigned char), 0, UCHAR_MAX);
  printf("\n|%-19s    |%-4d   |%-20hhi   | %-19hhi   | ", "signed char", sizeof(signed char), SCHAR_MIN, SCHAR_MAX);
  printf("\n|%-19s    |%-4d   |%-19hi    | %-18hi    | ", "short", sizeof(short), SHRT_MIN, SHRT_MAX);
  printf("\n|%-19s    |%-4d   |%-19hu    | %-18hu    | ", "unsigned short", sizeof(unsigned short), 0, USHRT_MAX);
  printf("\n|%-19s    |%-4d   |%-18i     | %-17i     | ", "int", sizeof(int), INT_MIN, INT_MAX);
  printf("\n|%-19s    |%-4d   |%-18u     | %-17u     | ", "unsigned int", sizeof(unsigned int), 0, UINT_MAX);
  printf("\n|%-19s    |%-4d   |%-19li    | %-18li    | ", "long", sizeof(long), LONG_MIN, LONG_MAX);
  printf("\n|%-19s    |%-4d   |%-19lu    | %-18lu    | ", "unsigned long", sizeof(unsigned long), 0, ULONG_MAX);
  printf("\n|%-19s    |%-4d   |%-20lli   | %-19lli   | ", "long long", sizeof(long long), LLONG_MIN, LLONG_MAX);
  printf("\n|%-19s    |%-4d   |%-20lli   | %-19lli   | ", "unsigned long long", sizeof(unsigned long long), 0, ULLONG_MAX);
  printf("\n|%-19s    |%-4d   |%-18e     | %-17e     | ", "float", sizeof(float), FLT_MIN, FLT_MAX);
  printf("\n|%-19s    |%-4d   |%-19le    | %-18le    | ", "double", sizeof(double), DBL_MIN, DBL_MAX);
  printf("\n|%-19s    |%-4d   |%-19le    | %-18le    | ", "long double", sizeof(long double), LDBL_MIN, LDBL_MAX);
  printf("\n|%-19s    |%-4d   |%-18d     | %-18d    | ", "bool", sizeof(bool), true, false);
  printf("\n -------------------------------------------------------------------------------");
  return 0;
}

