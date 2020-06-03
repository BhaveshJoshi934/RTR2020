#include<stdio.h>

extern int global_cnt_bdj;

void change_cnt_2(void)
{
	global_cnt_bdj = global_cnt_bdj + 1;
	printf("Value of global in File_1.c : %d\n\n", global_cnt_bdj);
}