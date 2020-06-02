#include<stdio.h>
int main(void)
{
	int age_bdj;
	printf("\n\n");
	printf("Enter your age : \n");
	scanf("%d", &age_bdj);

	if (age_bdj >= 18)
	{
		printf("You are Eligible for Voting!!!!\n");
	}
	printf("You are NOT igible for Voting!!!!\n");
	return 0;
}