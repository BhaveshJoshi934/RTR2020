#include<stdio.h>

#define MAX_BDJ(a_bdj, b_bdj) ((a_bdj > b_bdj) ? a_bdj : b_bdj)
int main(int argc, char* argv[], char* envp[])
{
	int num1_i_bdj, num2_i_bdj, result_i_bdj;
	float num1_f_bdj, num2_f_bdj, result_f_bdj;

	printf("\n\n");
	printf("Enter First Integer Number:\n");
	scanf("%d", &num1_i_bdj);
	printf("\n\n");

	printf("Enter Second Integer Number:\n");
	scanf("%d", &num2_i_bdj);
	printf("\n\n");

	result_i_bdj = MAX_BDJ(num1_i_bdj, num2_i_bdj);
	printf("Result (Integer):%d\n", result_i_bdj);
	printf("\n\n");

	printf("Enter First Float Number:\n");
	scanf("%f", &num1_f_bdj);
	printf("\n\n");

	printf("Enter Second Float Number:\n");
	scanf("%f", &num2_f_bdj);
	printf("\n\n");

	result_f_bdj = MAX_BDJ(num1_f_bdj, num2_f_bdj);
	printf("Result (Float):%f\n", result_f_bdj);
	printf("\n\n");

	return 0;
}