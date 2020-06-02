#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj;
	printf("\n\n");
	for (i_bdj = 1; i_bdj <= 10; i_bdj++)
	{
		printf("i=%d\n", i_bdj);
		printf("--------------------\n\n");
		for (j_bdj = 1; j_bdj <= 3; j_bdj++)
		{
			printf("\t%d\n", j_bdj);
		}
		printf("\n\n");
	}
	return 0;
}