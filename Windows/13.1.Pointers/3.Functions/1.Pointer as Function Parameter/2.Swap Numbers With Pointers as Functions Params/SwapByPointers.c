#include<stdio.h>

int main(void)
{
	void SwapNumbers(int*, int*);

	int num1_bdj, num2_bdj;

	printf("\n\n");
	printf("Enter First Num:\n");
	scanf("%d", &num1_bdj);

	printf("Enter Second Num:\n");
	scanf("%d", &num2_bdj);

	printf("\n\n");

	printf("Before Swapping Numbers are:\n\n");
	printf("Num1 = %d\n", num1_bdj);
	printf("Num2 = %d\n", num2_bdj);

	SwapNumbers(&num1_bdj, &num2_bdj);

	printf("\n\n");
	printf("After Swapping Numbers are:\n\n");
	printf("Num1 = %d\n", num1_bdj);
	printf("Num2 = %d\n", num2_bdj);

	printf("\n\n");
	return 0;
}

void SwapNumbers(int *x, int *y)
{
	int temp;
	printf("\n\n");
	printf("Before Swapping X and Y are : %d and %d\n", *x, *y);

	temp = *x;
	*x = *y;
	*y = temp;
	printf("\n\n");
	printf("After Swapping X and Y are : %d and %d\n", *x, *y);
}




