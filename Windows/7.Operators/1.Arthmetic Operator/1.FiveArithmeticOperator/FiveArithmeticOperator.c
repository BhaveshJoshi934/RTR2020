#include <stdio.h>

int main(void)
{
	int a_bdj;
	int b_bdj;
	int output_bdj;

	//code
	printf("\n\n");
	printf("First Number : ");
	scanf("%d", &a_bdj);

	printf("\n\n");
	printf("Second Number : ");
	scanf("%d", &b_bdj);

	printf("\n\n");

	output_bdj = a_bdj + b_bdj;
	printf("Addition of a_bdj = %d and b_bdj = %d gives %d\n", a_bdj, b_bdj, output_bdj);

	output_bdj = a_bdj - b_bdj;
	printf("Subtraction of a_bdj = %d and b_bdj = %d gives %d\n", a_bdj, b_bdj, output_bdj);

	output_bdj = a_bdj * b_bdj;
	printf("Multiplication of a_bdj = %d and b_bdj = %d gives %d\n", a_bdj, b_bdj, output_bdj);

	output_bdj = a_bdj / b_bdj;
	printf("Division of a_bdj = %d and b_bdj = %d gives quotient %d\n", a_bdj, b_bdj, output_bdj);

	output_bdj = a_bdj % b_bdj;
	printf("Division of a_bdj = %d and b_bdj = %d gives remainder %d\n", a_bdj, b_bdj, output_bdj);

	return 0;
}