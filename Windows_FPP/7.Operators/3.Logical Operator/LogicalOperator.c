#include<stdio.h>
int main(void)
{
	int a_bdj;
	int b_bdj;
	int c_bdj;
	int output_bdj;

	printf("\n\n");
	printf("Enter First Integer:\n");
	scanf("%d", &a_bdj);
	printf("\n\n");

	printf("\n\n");
	printf("Enter Second Integer:\n");
	scanf("%d", &b_bdj);
	printf("\n\n");

	printf("\n\n");
	printf("Enter Third Integer:\n");
	scanf("%d", &c_bdj);
	printf("\n\n");

	printf("It will print '0' If answer is FALSE\n");
	printf("It will print '1' If answer is TRUE\n\n");

	output_bdj = (a_bdj <= b_bdj) && (b_bdj != a_bdj);
	printf("&& : Answer is 1 If And Only If BOTH Conditions Are True. The Answer is 0, If Any One Or Both Conditions Are False. \n\n");
	printf("a_bdj = %d is less than or equal to b_bdj = %d AND b_bdj = %d is not equal to c_bdj = %d      \t Answer = %d\n\n", a_bdj, b_bdj, b_bdj, c_bdj, output_bdj);

	output_bdj = (b_bdj >= a_bdj) || (a_bdj == c_bdj);
	printf("|| : Answer is 1 if any of the condition is 1;Otherwise 0\n");
	printf("b_bdj = %d is greater than or equal to a_bdj = %d OR a_ndj = %d is equal to c_bdj = %d        \t Answer = %d\n\n", b_bdj, a_bdj, a_bdj, c_bdj, output_bdj);

	output_bdj = !a_bdj;
	printf("a_bdj = %d Using '!' gives Output_bdj = %d\n\n", a_bdj, output_bdj);

	output_bdj = !b_bdj;
	printf("b_bdj = %d Using '!' gives Output_bdj = %d\n\n", b_bdj, output_bdj);

	output_bdj = !c_bdj;
	printf("c_bdj = %d Using '!' gives Output_bdj = %d\n\n", c_bdj, output_bdj);

	output_bdj = (!(a_bdj <= b_bdj) && !(b_bdj != c_bdj));
	printf("NOT of a_bdj = %d is less than or equal to b_bdj = %d AND NOT of b_bdj = %d is not equal to c_bdj = %d gives Output = %d\n\n", a_bdj, b_bdj, b_bdj, c_bdj, output_bdj);

	output_bdj = (!(b_bdj >= a_bdj) || (a_bdj == c_bdj));
	printf("NOT of b_bdj = %d is greater than or equal to a_bdj = %d OR a_ndj = %d is equal to c_bdj = %d gives Output = %d\n\n", b_bdj, a_bdj, a_bdj, c_bdj, output_bdj);

	return 0;
}