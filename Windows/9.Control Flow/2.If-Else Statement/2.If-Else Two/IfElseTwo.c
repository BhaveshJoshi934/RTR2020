#include<stdio.h>
int main(void)
{
	int age_bdj;
	printf("\n\n");
	printf("Enter your age : \n");
	scanf("%d", &age_bdj);
	printf("\n\n");
	if (age_bdj >= 18)
	{
		printf("Enter in if block\n\n");
		printf("You are Eligible for Voting!!!!\n");
	}
	else
	{
		printf("Enter in else block\n\n");
		printf("You are NOT Eligible for Voting!!!!\n");
	}
	printf("BYE\n");
	return 0;
}