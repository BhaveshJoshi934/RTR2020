#include<stdio.h>

int main(void)
{
	void MathematicalOperation(int, int, int*, int*, int*, int*, int*);

	int a_bdj, b_bdj;
	int *sum = NULL;
	int *diff = NULL;
	int *mul = NULL;
	int *div = NULL;
	int *quot = NULL;

	printf("Enter A :\n");
	scanf("%d", &a_bdj);

	printf("Enter B :\n");
	scanf("%d", &b_bdj);


	sum = (int*)malloc(1 * sizeof(int));
	if (sum == NULL)
	{
		printf("Memory allocation Failed!!!\n");
		exit(0);
	}
	
	diff = (int*)malloc(1 * sizeof(int));
	if (diff == NULL)
	{
		printf("Memory allocation Failed!!!\n");
		exit(0);
	}

	mul = (int*)malloc(1 * sizeof(int));
	if (mul == NULL)
	{
		printf("Memory allocation Failed!!!\n");
		exit(0);
	}

	div = (int*)malloc(1 * sizeof(int));
	if (div == NULL)
	{
		printf("Memory allocation Failed!!!\n");
		exit(0);
	}

	quot = (int*)malloc(1 * sizeof(int));
	if (quot == NULL)
	{
		printf("Memory allocation Failed!!!\n");
		exit(0);
	}

	MathematicalOperation(a_bdj, b_bdj, &sum, &diff, &mul, &div, &quot);

	printf("\n\n");
	printf("*********RESULTS*********\n\n");
	printf("Sum is       = %d\n", sum);
	printf("Diff is      = %d\n", diff);
	printf("Mult is      = %d\n", mul);
	printf("Div is       = %d\n", div);
	printf("Quot is      = %d\n", quot);

	return 0;
}

void MathematicalOperation(int x, int y, int* S, int* D, int* M, int* Di, int* Q)
{
	*S = x + y;
	*D = x - y;
	*M = x * y;
	*Di = x / y;
	*Q = x % y;
}
