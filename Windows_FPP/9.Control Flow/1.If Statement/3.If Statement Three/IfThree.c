#include<stdio.h>
int main(void)
{
	int num_bdj;
	printf("\n\n");
	printf("Enter a Number:\n");
	scanf("%d", &num_bdj);
	if (num_bdj < 0)
	{
		printf("Number is Negative!!!\n",num_bdj);
	}
	if (num_bdj > 0 && num_bdj <= 100)
	{
		printf("Number is Between 1 to 100\n", num_bdj);
	}
	if (num_bdj > 100 && num_bdj <= 200)
	{
		printf("Number is Between 101 to 200\n", num_bdj);
	}
	if (num_bdj > 200 && num_bdj <= 300)
	{
		printf("Number is Between 201 to 300\n", num_bdj);
	}
	if (num_bdj > 300 && num_bdj <= 400)
	{
		printf("Number is Between 301 to 400\n", num_bdj);
	}
	if (num_bdj > 400 && num_bdj <= 500)
	{
		printf("Number is Between 401 to 500\n", num_bdj);
	}
	if (num_bdj > 500)
	{
		printf("Number is Greater than 500\n", num_bdj);
	}
	return 0;
}