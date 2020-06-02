#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj, c_bdj;
	printf("\n\n");
	for (i_bdj = 0; i_bdj < 64; i_bdj++)
	{
		for (j_bdj = 0; j_bdj < 64; j_bdj++)
		{
			c_bdj = ((i_bdj & 0x8) == 0) ^ ((j_bdj & 0x8) == 0);
			if(c_bdj == 0)
			{
				printf("  ");
			}
			if (c_bdj == 1)
			{
				printf("* ");
			}
		}
		printf("\n\n");
	}
	return 0;
}