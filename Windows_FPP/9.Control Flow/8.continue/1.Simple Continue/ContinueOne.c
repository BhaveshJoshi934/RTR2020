#include<stdio.h>
int main(void)
{
	int i_bdj;
	printf("\n\n");
	printf("Print Even Numbers:(1-100):\n\n");

	for (i_bdj = 1; i_bdj <= 100; i_bdj++)
	{
		if (i_bdj % 2 == 0)
		{
			printf("\t%d\n",i_bdj);
		}
		else
		{
			continue;
		}
	}
	printf("\n\n");
	return 0;
}