#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj, k_bdj;
	printf("\n\n");
	i_bdj = 1;
	do
	{
		printf("i=%d\n", i_bdj);
		printf("--------------------\n\n");
		j_bdj = 1;
		do
		{
			printf("\tj=%d\n", j_bdj);
			printf("\t--------------------\n\n");
			k_bdj = 1;
			do
			{
				printf("\t\tk=%d\n", k_bdj);
				k_bdj++;
			}while (k_bdj <= 3);
			printf("\n\n");
			j_bdj++;
		}while (j_bdj <= 5);
		printf("\n\n");
		i_bdj++;
	}while (i_bdj <= 10);
	return 0;
}