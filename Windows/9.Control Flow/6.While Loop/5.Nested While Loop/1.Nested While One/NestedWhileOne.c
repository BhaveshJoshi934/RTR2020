#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj;
	printf("\n\n");
	i_bdj = 1;
	while(i_bdj<=10)
	{
		printf("i=%d\n", i_bdj);
		printf("--------------------\n\n");
		j_bdj = 1;
		while(j_bdj <= 3)
		{
			printf("\t%d\n", j_bdj);
			j_bdj++;
		}
		i_bdj++;
		printf("\n\n");
	}
	return 0;
}