#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	int* ptrArray = NULL;
	unsigned int ArrayLength = 0;
	int i;

	printf("\n\n");
	printf("Enter the Numbers of elements u want to enter:\n\n");
	scanf("%d", &ArrayLength);

	ptrArray = (int*)malloc(sizeof(int) * ArrayLength);
	if (ptrArray == NULL)
	{
		printf("\n\n");
		printf("Memeory is not allocated Properly...Exiting Now!!!\n\n");
		exit(0);
	}
	else
	{
		printf("\n\n");
		printf("Memory Allocated Perfectly!!!\n\n");
		printf("Memory Allocated from %p to %p ...\n\n", ptrArray, (ptrArray + (ArrayLength - 1)));
	}

	printf("Enter the elements in an Array:\n\n");

	for (i = 0; i < ArrayLength; i++)
	{
		scanf("%d", (ptrArray + i));
	}
	printf("The array of length %d entered by u :\n", ArrayLength);

	for (i = 0; i < ArrayLength; i++)
	{
		printf("Entered element : %d   and its address : %p\n", ptrArray[i], &ptrArray[i]);
	}
	printf("Using pointer\n\n");

	for (i = 0; i < ArrayLength; i++)
	{
		printf("Entered element : %d   and its address : %p\n", *(ptrArray + i), (ptrArray + i));
	}

	if (ptrArray)
	{
		free(ptrArray);
		ptrArray = NULL;
		printf("Allcated memory is successfully freed!!!!\n\n");
	}
	return 0;
}