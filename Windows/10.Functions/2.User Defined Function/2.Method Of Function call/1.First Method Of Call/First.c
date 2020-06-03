#include<stdio.h>
int main(int argc, char* argv[], char* envp[])
{
	void Add_bdj(void);
	int Sub_bdj(void);
	void Mult_bdj(int,int);
	int Div_bdj(int,int);

	int result_sub_bdj;
	int a_mult_bdj, b_mult_bdj;
	int a_div_bdj, b_div_bdj, result_div_bdj;

	Add_bdj();

	result_sub_bdj = Sub_bdj();
	printf("\n\n");
	printf("Result of Subtraction: %d\n", result_sub_bdj);

	printf("\n\n");
	printf("Enter interger value A for Multiplication\n");
	scanf("%d", &a_mult_bdj);
	printf("\n\n");
	printf("Enter interger value B for Multiplication\n");
	scanf("%d", &b_mult_bdj);
	Mult_bdj(a_mult_bdj, b_mult_bdj);

	printf("\n\n");
	printf("Enter interger value A for Division\n");
	scanf("%d", &a_div_bdj);
	printf("\n\n");
	printf("Enter interger value B for Division\n");
	scanf("%d", &b_div_bdj);
	result_div_bdj = Div_bdj(a_div_bdj, b_div_bdj);
	printf("\n\n");
	printf("Result of %d and %d gives %d\n\n", a_div_bdj, b_div_bdj, result_div_bdj);

	return 0;
}
void Add_bdj(void)
{
	int a_bdj, b_bdj, sum_bdj;
	printf("Enter A:\n");
	scanf("%d", &a_bdj);

	printf("Enter B:\n");
	scanf("%d", &b_bdj);

	sum_bdj = a_bdj + b_bdj;
	printf("Sum of %d and %d : %d\n", a_bdj, b_bdj, sum_bdj);
}

int Sub_bdj(void)
{
	int a_bdj, b_bdj, sub_bdj;
	printf("Enter A:\n");
	scanf("%d", &a_bdj);

	printf("Enter B:\n");
	scanf("%d", &b_bdj);

	sub_bdj = a_bdj - b_bdj;
	return sub_bdj;
} 

void Mult_bdj(int a, int b)
{
	int mult_bdj;
	mult_bdj = a * b;
	printf("Multiplication of %d and %d : %d\n", a, b, mult_bdj);
}

int Div_bdj(a, b)
{
	int div_quo;
	if (a > b)
	{
		div_quo = a / b;
	}
	else
	{
		div_quo = b / a;
	}
	return div_quo;
}