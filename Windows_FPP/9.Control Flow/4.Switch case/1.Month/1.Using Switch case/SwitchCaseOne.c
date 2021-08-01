#include<stdio.h>
int main(void)
{
	int month_bdj;
	printf("\n\n");
	printf("Enter month's number (1-12):\n");
	scanf("%d", &month_bdj);
	printf("\n\n");
	switch (month_bdj)
	{
	case 1:
		printf("This is %d : January\n",month_bdj);
		break;

	case 2:
		printf("This is %d : February\n", month_bdj);
		break;

	case 3:
		printf("This is %d : March\n", month_bdj);
		break;

	case 4:
		printf("This is %d : April\n", month_bdj);
		break;

	case 5:
		printf("This is %d : May\n", month_bdj);
		break;

	case 6:
		printf("This is %d : June\n", month_bdj);
		break;

	case 7:
		printf("This is %d : July\n", month_bdj);
		break;

	case 8:
		printf("This is %d : August\n", month_bdj);
		break;

	case 9:
		printf("This is %d : September\n", month_bdj);
		break;

	case 10:
		printf("This is %d : October\n", month_bdj);
		break;

	case 11:
		printf("This is %d : November\n", month_bdj);
		break;

	case 12:
		printf("This is %d : December\n", month_bdj);
		break;

	default:
		printf("You have entered %d ie Invalid Month\n\n", month_bdj);
		break;
	}

	printf("Switch-Case Complete!!!\n");
	return 0;
}