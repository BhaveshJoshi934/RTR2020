#include<stdio.h>

void change_cnt_3(void)
{
	extern int global_cnt_bdj;
	global_cnt_bdj = global_cnt_bdj + 1;
	printf("Value of global in File_2 : %d\n\n", global_cnt_bdj);
}