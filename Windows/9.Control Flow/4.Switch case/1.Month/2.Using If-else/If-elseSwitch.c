#include<stdio.h>
int main(void)
{
	int month_bdj;
	printf("\n\n");
	printf("Enter month's number (1-12):\n");
	scanf("%d", &month_bdj);
	printf("\n\n");

	if (month_bdj == 1)
	{
		printf("This is % d : January\n",month_bdj);
	}
	else if (month_bdj == 2)
	{
		printf("This is % d : February\n", month_bdj);
	}
	else if (month_bdj == 3)
	{
		printf("This is % d : March\n", month_bdj);
	}
	else if (month_bdj == 4)
	{
		printf("This is % d : April\n", month_bdj);
	}
	else if (month_bdj == 5)
	{
		printf("This is % d : May\n", month_bdj);
	}
	else if (month_bdj == 6)
	{
		printf("This is % d : June\n", month_bdj);
	}
	else if (month_bdj == 7)
	{
		printf("This is % d : July\n", month_bdj);
	}
	else if (month_bdj == 8)
	{
		printf("This is % d : August\n", month_bdj);
	}
	else if (month_bdj == 9)
	{
		printf("This is % d : September\n", month_bdj);
	}
	else if (month_bdj == 10)
	{
		printf("This is % d : October\n", month_bdj);
	}
	else if (month_bdj == 11)
	{
		printf("This is % d : November\n", month_bdj);
	}
	else if (month_bdj == 12)
	{
		printf("This is % d : December\n", month_bdj);
	}
	else
	{
		printf("%d:Invalid Number\n\n",month_bdj);
	}

	printf("If-else ladder completed!!!\n\n");

	return 0;
}