#include<stdio.h>

int main(void)
{
	int iArr_bdj[10];
	int i_bdj;
	int* ptr_bdj = NULL;


	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		iArr_bdj[i_bdj] = (i_bdj + 1) * 3;
	}

	ptr_bdj = iArr_bdj;

	printf("\n\n");
	printf("Elemets in the Array are:\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("iArr_bdj[%d] = %d\n", i_bdj, *(ptr_bdj + i_bdj));
	}

	printf("\n\n");
	printf("Elemets in the Array are:\n\n");
	for (i_bdj = 0; i_bdj < 10; i_bdj++)
	{
		printf("iArr_bdj[%d] = %d And its Address = %p\n", i_bdj, *(ptr_bdj + i_bdj), (ptr_bdj + i_bdj));
	}
	printf("\n\n");
	return 0;
}