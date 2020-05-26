#include <stdio.h>

int main()
{
	int a_bdj;
	int b_bdj;
	int x_bdj;

	printf("\n\n");
	printf("Enter a Number : ");
	scanf("%d", &a_bdj);

	printf("\n\n");
	printf("Enter another number : ");
	scanf("%d", &b_bdj);

	printf("\n\n");

	x_bdj = a_bdj;
	a_bdj += b_bdj;
	printf("Addition of a_bdj = %d and b_bdj = %d gives %d\n", x_bdj, b_bdj, a_bdj);

	x_bdj = a_bdj;
	a_bdj -= b_bdj;
	printf("Subtraction of a_bdj = %d and b_bdj = %d gives %d\n", x_bdj, b_bdj, a_bdj);

	x_bdj = a_bdj;
	a_bdj *= b_bdj;
	printf("Multiplication of a_bdj = %d and b_bdj = %d gives %d\n", x_bdj, b_bdj, a_bdj);

	x_bdj = a_bdj;
	a_bdj /= b_bdj;
	printf("Division of a_bdj = %d and b_bdj = %d gives quotient  %d\n", x_bdj, b_bdj, a_bdj);

	x_bdj = a_bdj;
	a_bdj %= b_bdj;
	printf("Addition of a_bdj = %d and b_bdj =%d gives remainder %d\n", x_bdj, b_bdj, a_bdj);

	return 0;
}