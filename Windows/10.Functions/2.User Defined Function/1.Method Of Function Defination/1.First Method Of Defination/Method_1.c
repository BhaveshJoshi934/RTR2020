#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	void Addition_bdj(void);

	Addition_bdj();
	return 0;
}

void Addition_bdj(void)
{
	int a_bdj, b_bdj;
	int result_bdj;

	printf("\n\n");
	printf("Enter First Number:\n");
	scanf("%d", &a_bdj);

	printf("Enter Second Number:\n");
	scanf("%d", &b_bdj);

	result_bdj = a_bdj + b_bdj;
	printf("Sum of %d and %d : %d\n\n",a_bdj,b_bdj, result_bdj);
}