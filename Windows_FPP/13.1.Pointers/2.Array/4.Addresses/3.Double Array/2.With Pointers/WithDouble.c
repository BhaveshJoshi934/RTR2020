#include<stdio.h>

int main(void)
{
	double dArr_bdj[10];
	int i_bdj;
	double* ptr_bdj = NULL;

	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		dArr_bdj[i_bdj] = (double)(i_bdj + 1) * 7.14065;
	}

	printf("\n\n");
	printf("Elements in the Double Array are:\n\n");

	ptr_bdj = dArr_bdj;

	printf("\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("dArr[%d] = %lf\n", i_bdj, *(ptr_bdj + i_bdj));
	}
	printf("Elements in the Double Array are:\n\n");
	printf("\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("dArr[%d] = %lf And Address is  = %p\n", i_bdj, *(ptr_bdj + i_bdj), (ptr_bdj + i_bdj));
	}
	printf("\n\n");
	return 0;
}