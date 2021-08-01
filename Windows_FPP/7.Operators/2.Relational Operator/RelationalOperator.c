#include<stdio.h>
int main(void)
{
	int a_bdj;
	int b_bdj;
	int result_bdj;

	printf("\n\n");
	printf("Enter one integer:\n");
	scanf("%d", &a_bdj);
	printf("\n\n");

	printf("\n\n");
	printf("Enter another integer:\n");
	scanf("%d", &b_bdj);
	printf("\n\n");

	printf("If result = FALSE Then print 0\n");
	printf("If result = TRUE Then print 1\n\n");

	result_bdj = (a_bdj < b_bdj);
	printf("If %d is less than %d then               \t Answer = %d\n", a_bdj, b_bdj, result_bdj);

	result_bdj = (a_bdj > b_bdj);
	printf("If %d is greater than %d then            \t Answer = %d\n", a_bdj, b_bdj, result_bdj);

	result_bdj = (a_bdj <= b_bdj);
	printf("If %d is less than or equal to %d then   \t Answer = %d\n", a_bdj, b_bdj, result_bdj);

	result_bdj = (a_bdj >= b_bdj);
	printf("If %d is greater than or equal to %d then \t Answer = %d\n", a_bdj, b_bdj, result_bdj);

	result_bdj = (a_bdj == b_bdj);
	printf("If %d is equal to %d then                  \t Answer = %d\n", a_bdj, b_bdj, result_bdj);

	result_bdj = (a_bdj != b_bdj);
	printf("If %d is not equal %d then                 \t Answer = %d\n\n", a_bdj, b_bdj, result_bdj);

	return 0;
}