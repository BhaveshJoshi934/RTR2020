#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj, c_bdj;
	printf("\n\n");
	i_bdj = 0;
	while(i_bdj < 64)
	{
		j_bdj = 0;
		while(j_bdj < 64)
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
		}
		printf("\n\n");
		i_bdj++;
	}
	return 0;
}