//gcc -shared -o libsum.dll sum.c
//gcc -L. -o abc.exe main.c -lsum
// gcc -Lbuild -o IdGen_test.exe IdGen_test.c -lIdGenLib

//Выполняет генерацию идентификаторов по указанным правилам с учетом "черного списка" для букв и цифр
//
#ifndef __IdGen_h__
#define __IdGen_h__
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#define ABC_LENGTH 26
#define _ABC_OFFSET_ 65
#define _abc_OFFSET_ 97
#define _NUMERALS_OFFSET_ 48 
#define NUMERALS_LENGTH 10
#define STATE_LEN 20
#define MAX_BUFF 256
int keep_alnum(char *p, int len);
void upper(char *p, int len);
int create_tables(char mode,int *table,char *reverse_table, int lt, char *black_list,int lb);
struct IdGen_State_Mashine{
	int LT[ABC_LENGTH];
	char revLT[ABC_LENGTH];
	int  L1;
	int  LA;
	int DT[NUMERALS_LENGTH];
	char revDT[NUMERALS_LENGTH];
	int  States[STATE_LEN];
	char Buffer[STATE_LEN+1];
	char *StartPtr;
	int StartNumber;
	char Buffer_with_delimeters[2*STATE_LEN+1];
	int err_code;
};
typedef struct IdGen_State_Mashine IdGen_State_Mashine;
int init_IdGen_State_Mashine(IdGen_State_Mashine *ptr,char *string,int sl,char *black1,int len1,char *blackA,int lenA);
void run_IdGen_State_Mashine(IdGen_State_Mashine *ptr);
int IdGen_sprintf(IdGen_State_Mashine *ptr,char *p,int len);
char * identificator_generation(IdGen_State_Mashine *ptr,char *in);
#endif
