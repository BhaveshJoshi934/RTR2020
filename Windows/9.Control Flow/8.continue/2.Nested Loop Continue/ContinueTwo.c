#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj;
	printf("\n\n");
	printf("Outer Loop prints Odd Numbers\n\n");
	printf("Even Numbers prints Even Numbers\n\n");

	for (i_bdj = 1; i_bdj <= 10; i_bdj++)
	{
		if (i_bdj % 2 != 0)
		{
			printf("i = %d\n", i_bdj);
			printf("-------------\n");
			for (j_bdj = 1; j_bdj <= 10; j_bdj++)
			{
				if (j_bdj % 2 == 0)
				{
					printf("\t%d\n", j_bdj);
				}
				else
				{
					continue;
				}
			}
			printf("\n\n");
		}
		else
		{
			continue;
		}
	}
	printf("\n\n");
	return 0;
}