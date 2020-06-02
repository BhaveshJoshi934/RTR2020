#include<stdio.h>
int main(void)
{
	int a_bdj;
	printf("\n\n");
	a_bdj = 5;
	if (a_bdj)
	{
		printf("If 1: A exist having value = %d\n\n",a_bdj);
	}
	a_bdj = -5;
	if (a_bdj)
	{
		printf("If 2: A exist having value = %d\n\n", a_bdj);
	}
	a_bdj = 0;
	if (a_bdj)
	{
		printf("If 3: A exist having value = %d\n\n", a_bdj);
	}
	printf("ALL STATEMENTS ARE EXECUTED!!!\n");

	return 0;
}