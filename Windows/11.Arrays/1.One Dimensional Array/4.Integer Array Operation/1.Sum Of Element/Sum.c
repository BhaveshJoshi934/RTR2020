#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
	int i_bdj;
	int num_bdj;
	int sum_bdj = 0;
	int iArr[NUM_ELEMENTS];

	printf("\n\n");
	printf("Enter the Elements:\n\n");
	for (i_bdj = 0; i_bdj < NUM_ELEMENTS; i_bdj++)
	{
		scanf("%d", &num_bdj);
		iArr[i_bdj] = num_bdj;
	}

	for (i_bdj = 0; i_bdj < NUM_ELEMENTS; i_bdj++)
	{
		sum_bdj = sum_bdj + iArr[i_bdj];
	}
	printf("\n\n");
	printf("Sum Of All Elements : %d\n\n", sum_bdj);

	return 0;
}