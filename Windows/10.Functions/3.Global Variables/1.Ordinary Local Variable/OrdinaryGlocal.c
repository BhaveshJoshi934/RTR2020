#include<stdio.h>

int global_cnt_bdj = 0;

int main(void)
{
	void change_cnt_one(void);
	void change_cnt_two(void);
	void change_cnt_three(void);

	printf("\n");
	printf("Value of global count : %d\n", global_cnt_bdj);

	change_cnt_one();
	change_cnt_two();
	change_cnt_three();

	printf("\n");
	return 0;
}
void change_cnt_one(void)
{
	global_cnt_bdj = 100;
	printf("Value of global count after change_one : %d\n\n", global_cnt_bdj);
}

void change_cnt_two(void)
{
	global_cnt_bdj = global_cnt_bdj + 1;
	printf("Value of global count after change_two : %d\n\n", global_cnt_bdj);
}

void change_cnt_three(void)
{
	global_cnt_bdj = global_cnt_bdj + 10;
	printf("Value of global count after change_three : %d\n\n", global_cnt_bdj);
}