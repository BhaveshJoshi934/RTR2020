#include<stdio.h>
int main(void)
{
	int a_bdj, b_bdj;
	int p_bdj, q_bdj;
	char ch_result_01_bdj, ch_result_02_bdj;
	int i_result_01_bdj, i_result_02_bdj;

	printf("\n\n");
	a_bdj = 77;
	b_bdj = 55;
	ch_result_01_bdj = (a_bdj > b_bdj) ? 'A' : 'B';
	i_result_01_bdj = (a_bdj > b_bdj) ? a_bdj : b_bdj;
	printf("Ternary operator ANS:1 ----------> %c and %i\n\n", ch_result_01_bdj, i_result_01_bdj);

	p_bdj = 300;
	q_bdj = 300;
	ch_result_02_bdj = (p_bdj != q_bdj) ? 'P' : 'Q';
	i_result_02_bdj = (p_bdj != q_bdj) ? p_bdj : q_bdj ;
	printf("Ternary operator ANS:2 ----------> %c and %i\n\n", ch_result_02_bdj, i_result_02_bdj);

	printf("\n\n");
	return 0;
}