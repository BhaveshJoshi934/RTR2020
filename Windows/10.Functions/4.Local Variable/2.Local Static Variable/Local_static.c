#include<stdio.h>
int main(void)
{
	int a_bdj = 5;
	void change_cnt(void);

	printf("\n\n");
	printf("A = %d\n\n", a_bdj);

	change_cnt();
	change_cnt();
	change_cnt();

	return 0;
}
void change_cnt(void)
{
	static int local_bdj = 0;
	local_bdj = local_bdj + 1;
	printf("Local Count : %d\n\n", local_bdj);
}