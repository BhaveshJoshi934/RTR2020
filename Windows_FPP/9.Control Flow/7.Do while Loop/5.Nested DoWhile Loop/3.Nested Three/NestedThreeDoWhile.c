#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj, c_bdj;
	printf("\n\n");
	i_bdj = 0;
	do
	{
		j_bdj = 0;
		do
		{
			c_bdj = ((i_bdj & 0x8) == 0) ^ ((j_bdj & 0x8) == 0);
			if (c_bdj == 0)
			{
				printf("  ");
			}
			if (c_bdj == 1)
			{
				printf("* ");
			}
			j_bdj++;
		}while (j_bdj < 64);
		printf("\n\n");
		i_bdj++;
	}while (i_bdj < 64);
	return 0;
}