#include<stdio.h>

int main(void)
{
	double dArr_bdj[10];
	int i_bdj;

	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		dArr_bdj[i_bdj] = (double)(i_bdj + 1) * 1.5467278;
	}

	printf("\n\n");
	printf("Elements in the Double Array are:\n\n");

	printf("\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("dArr[%d] = %lf\n", i_bdj, dArr_bdj[i_bdj]);
	}
	printf("Elements in the Double Array are:\n\n");
	printf("\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("dArr[%d] = %lf And Address is  = %p\n", i_bdj, dArr_bdj[i_bdj], &dArr_bdj[i_bdj]);
	}
	printf("\n\n");
	return 0;
}