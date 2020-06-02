#include<stdio.h>
int main(void)
{
	int a_bdj = 1;
	printf("\n\n");

	printf("Print 1-10\n\n");
	do
	{
		printf("\t%d\n", a_bdj);
		a_bdj++;
	} while (a_bdj <= 10);

	return 0;
}