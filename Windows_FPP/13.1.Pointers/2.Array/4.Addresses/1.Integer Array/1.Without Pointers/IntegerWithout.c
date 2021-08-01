#include<stdio.h>

int main(void)
{
	int iArr_bdj[10];
	int i_bdj;

	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		iArr_bdj[i_bdj] = (i_bdj + 1) * 3;
	}

	printf("\n\n");
	printf("Elements in the Integer Array are:\n\n");

	printf("\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("iArr[%d] = %d\n", i_bdj, iArr_bdj[i_bdj]);
	}

	printf("\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("iArr[%d] = %d\t And Address is  = %p\n", i_bdj, iArr_bdj[i_bdj],&iArr_bdj[i_bdj]);
	}
	printf("\n\n");
	return 0;
}