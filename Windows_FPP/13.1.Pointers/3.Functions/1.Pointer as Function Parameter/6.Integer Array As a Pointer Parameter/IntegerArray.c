#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	void MultiplyArratElementsByNumber(int*, int, int);

	int *iArray_bdj = NULL;
	int num_elemets_bdj;
	int i_bdj, num_bdj;

	printf("Enter Number of elements you want in array:\n\n");
	scanf("%d", &num_elemets_bdj);

	iArray_bdj = (int*)malloc(num_elemets_bdj * sizeof(int));

	if (iArray_bdj == NULL)
	{
		printf("Allocation Failed!!!\n");
		exit(0);
	}
	printf("\n\n");

	printf("Enter %d Elements for integer Array : \n\n", num_elemets_bdj);
	for (i_bdj = 0; i_bdj < num_elemets_bdj; i_bdj++)
	{
		scanf("%d", &iArray_bdj[i_bdj]);
	}

	printf("\n\n");
	printf("Array Before Passing to the Function!!!\n\n");
	for (i_bdj = 0; i_bdj < num_elemets_bdj; i_bdj++)
	{
		printf("iArray[%d] = %d\n", i_bdj, iArray_bdj[i_bdj]);
	}
	printf("\n\n");

	printf("Enter by which num you want to multiply\n\n");
	scanf("%d", &num_bdj);

	MultiplyArratElementsByNumber(iArray_bdj, num_elemets_bdj, num_bdj);

	printf("\n\n");
	printf("Array After Passing to the Function!!!\n\n");
	for (i_bdj = 0; i_bdj < num_bdj; i_bdj++)
	{
		printf("iArray[%d] = %d\n", i_bdj, iArray_bdj[i_bdj]);
	}
	printf("\n\n");

	if (iArray_bdj)
	{
		free(iArray_bdj);
		iArray_bdj = NULL;
		printf("\n\n");
		printf("Allocated Memory Freed!!\n\n");
	}

	return 0;
}

void MultiplyArratElementsByNumber(int *arr, int iNumEle, int n)
{
	int i;
	for (i = 0; i < iNumEle; i++)
	{
		arr[i] = arr[i] * n;
	}
}