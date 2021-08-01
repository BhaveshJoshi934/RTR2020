#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj;
	char ch_01_bdj, ch_02_bdj;

	int a_bdj, result_int_bdj;
	float f_bdj, result_float_bdj;

	int i_explicit_bdj;
	float f_explicit_bdj;

	printf("\n\n");
	i_bdj = 65;
	ch_01_bdj = i_bdj;
	printf("I = %d\n", i_bdj);
	printf("Character 1 after (ch_01_bdj = i_bdj) = %c\n\n", ch_01_bdj);

	ch_02_bdj = 'B';
	j_bdj = ch_02_bdj;
	printf("Character 2 = %c\n", ch_02_bdj);
	printf("J after(j_bdj = ch_02_bdj) = %d\n\n", j_bdj);

	a_bdj = 50;
	f_bdj = 8.7f;
	result_int_bdj = a_bdj + f_bdj;
	printf("a_bdj = %d and f_bdj = %f gives output = %d\n\n", a_bdj, f_bdj, result_int_bdj);

	result_float_bdj = a_bdj + f_bdj;
	printf("a_bdj = %d and f_bdj = %f gives output = %f\n\n", a_bdj, f_bdj, result_float_bdj);

	f_explicit_bdj = 08.1111996f;
	i_explicit_bdj = (int)f_explicit_bdj;
	printf("Floating Point Number Which Will Be Type Casted Explicitly = %f\n", f_explicit_bdj);
	printf("Resultant Integer After Explicit Type Casting Of %f = %d\n\n", f_explicit_bdj, i_explicit_bdj);

	return 0;
}