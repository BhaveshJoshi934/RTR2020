#include<stdio.h>
int main(void)
{
	void PrintDecimalToBinary_bdj(unsigned int);

	unsigned int a_bdj;
	unsigned int num_bits_bdj;
	unsigned int output_bdj;

	printf("\n\n");
	printf("Enter an Integer:\n");
	scanf("%d", &a_bdj);
	printf("\n\n");

	printf("\n\n");
	printf("Enter number of bits = %d you want to shift to right:\n", num_bits_bdj);
	scanf("%d", &num_bits_bdj);

	printf("\n\n\n\n");

	output_bdj = a_bdj >> num_bits_bdj;
	printf("Right Shift of a_bdj = %d by num_bits_bdj = %d gives output_bdj = %d\n\n", a_bdj,num_bits_bdj, output_bdj);

	PrintDecimalToBinary_bdj(a_bdj);
	PrintDecimalToBinary_bdj(output_bdj);

	return 0;
}

void PrintDecimalToBinary_bdj(unsigned int decimal_number)
{
	unsigned int quotient_bdj, reminder_bdj;
	unsigned int num_bdj;
	unsigned int binary_arr[8];
	int i;

	for (i = 0; i < 8; i++)
	{
		binary_arr[i] = 0;
	}
	printf("Binary Form of Decimal Integer %d \t=\t", decimal_number);
	num_bdj = decimal_number;
	i = 7;
	while (num_bdj != 0)
	{
		quotient_bdj = num_bdj / 2;
		reminder_bdj = num_bdj % 2;
		binary_arr[i] = reminder_bdj;
		num_bdj = quotient_bdj;
		i--;
	}
	for (i = 0; i < 8; i++)
	{
		printf("%u", binary_arr[i]);
	}
	printf("\n\n");
}