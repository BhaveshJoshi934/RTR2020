#include<stdio.h>

int main(void)
{
	void change_cnt(void);
	extern int global_bdj;
	printf("\n\n");
	printf("Value of global before change : %d\n\n", global_bdj);
	change_cnt();
	printf("Value of global after change : %d\n\n", global_bdj);
	return 0;
}

int global_bdj = 0;

void change_cnt(void)
{
	global_bdj = 500;
	printf("Value of gloabl variable : %d\n\n", global_bdj);
}