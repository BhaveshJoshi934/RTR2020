#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj,k_bdj;
	printf("\n\n");
	for (i_bdj = 1; i_bdj <= 10; i_bdj++)
	{
		printf("i=%d\n", i_bdj);
		printf("--------------------\n\n");
		for (j_bdj = 1; j_bdj <= 5; j_bdj++)
		{
			printf("\tj=%d\n", j_bdj);
			printf("\t--------------------\n\n");
			for (k_bdj = 1; k_bdj <= 3; k_bdj++)
			{
				printf("\t\tk=%d\n",k_bdj);
			}
			printf("\n\n");
		}
		printf("\n\n");
	}
	return 0;
}