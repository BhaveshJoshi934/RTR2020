#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	void Addition_bdj(int, int);

	int a_bdj, b_bdj;
	printf("\n\n");
	printf("Enter A:\n");
	scanf("%d", &a_bdj);

	printf("Enter B:\n");
	scanf("%d", &b_bdj);
	printf("\n\n");

	Addition_bdj(a_bdj, b_bdj);
	return 0;
}

void Addition_bdj(int a_bdj, int b_bdj)
{
	int result_bdj;
	result_bdj = a_bdj + b_bdj;
	printf("\n\n");
	printf("Sum of %d and %d : %d\n\n", a_bdj, b_bdj, result_bdj);
}