#include<stdio.h>

int main(void)
{
	float fArr_bdj[10];
	int i_bdj;
	float *ptr_bdj = NULL;

	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		fArr_bdj[i_bdj] = (float)(i_bdj + 1) * 7.14f;
	}

	printf("\n\n");
	printf("Elements in the Float Array are:\n\n");

	ptr_bdj = fArr_bdj;

	printf("\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("fArr[%d] = %f\n", i_bdj, *(ptr_bdj + i_bdj));
	}
	printf("Elements in the Float Array are:\n\n");
	printf("\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("fArr[%d] = %f And Address is  = %p\n", i_bdj, *(ptr_bdj + i_bdj), (ptr_bdj + i_bdj));
	}
	printf("\n\n");
	return 0;
}