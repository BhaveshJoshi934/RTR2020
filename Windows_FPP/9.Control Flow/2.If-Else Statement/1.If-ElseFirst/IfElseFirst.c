#include<stdio.h>
int main(void)
{
	int a_bdj = 2, b_bdj = 5, p_bdj = 5;
	printf("\n\n");
	if (a_bdj < p_bdj)
	{
		printf("You are in first If block!!\n");
		printf("A is Less Than P\n\n");
	}
	else
	{
		printf("You are in first Else block!!\n");
		printf("A is NOT Less Than P\n\n");
	}
	printf("\n\n");
	if (b_bdj != p_bdj)
	{
		printf("You are in Second If block!!\n");
		printf("B Is Not Equal To P\n\n");
	}
	else
	{
		printf("You are in second Else block!!\n");
		printf("B Is Equal To P\n\n");
	}

	printf("Second If-Else Performed...\n\n");
	return 0;
}