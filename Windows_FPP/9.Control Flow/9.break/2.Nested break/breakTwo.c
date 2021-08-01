#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj;
	printf("\n\n");

	for (i_bdj = 1; i_bdj <= 20; i_bdj++)
	{
		for (j_bdj = 1; j_bdj <= 20; j_bdj++)
		{
			if (j_bdj > i_bdj)
			{
				break;
			}
			else
			{
				printf("* ");
			}
		}
		printf("\n");
	}
	printf("\n\n");
	return 0;
}