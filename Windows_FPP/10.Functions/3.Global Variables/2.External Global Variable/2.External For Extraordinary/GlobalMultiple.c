#include<stdio.h>

int global_cnt_bdj = 0;

int main(void)
{
	void change_cnt_1(void);
	void change_cnt_2(void);
	void change_cnt_3(void);

	printf("\n\n");
	
	change_cnt_1();
	change_cnt_2();
	change_cnt_3();

	return 0;
}
void change_cnt_1(void)
{
	global_cnt_bdj = global_cnt_bdj + 1;
	printf("Value of global : %d\n\n", global_cnt_bdj);
}