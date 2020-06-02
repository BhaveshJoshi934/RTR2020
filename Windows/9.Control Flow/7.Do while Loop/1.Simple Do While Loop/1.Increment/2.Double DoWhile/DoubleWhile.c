#include<stdio.h>
int main(void)
{
	int a_bdj = 1, b_bdj = 10;
	printf("\n\n");

	printf("Print 1-10 and 10-100\n\n");
	do
	{
		printf("\t%d\t%d\n", a_bdj, b_bdj);
		a_bdj++;
		b_bdj = b_bdj + 10;
	} while (a_bdj <= 10, b_bdj <= 100);
	printf("\n\n");
	return 0;
}