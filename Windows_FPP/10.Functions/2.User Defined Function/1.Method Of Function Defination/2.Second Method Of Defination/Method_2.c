#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	int result_bdj;
	int Addition_bdj(void);

	result_bdj = Addition_bdj();
	printf("Sum is : %d\n\n",result_bdj);
	return 0;
}

int Addition_bdj(void)
{
	int a_bdj, b_bdj,result_bdj;

	printf("\n\n");
	printf("Enter First Number:\n");
	scanf("%d", &a_bdj);

	printf("Enter Second Number:\n");
	scanf("%d", &b_bdj);

	result_bdj = a_bdj + b_bdj;
	return result_bdj;
}