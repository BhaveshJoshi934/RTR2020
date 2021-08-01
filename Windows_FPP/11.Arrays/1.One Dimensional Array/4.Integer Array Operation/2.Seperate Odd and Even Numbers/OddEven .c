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

	printf("The Even Numbers From Array are:\n\n");
	for (i_bdj = 0; i_bdj < NUM_ELEMENTS; i_bdj++)
	{
		if (iArr[i_bdj] % 2 == 0)
		{
			printf("%d\n", iArr[i_bdj]);
		}
	}

	printf("The Odd Numbers From Array are:\n\n");
	for (i_bdj = 0; i_bdj < NUM_ELEMENTS; i_bdj++)
	{
		if (iArr[i_bdj] % 2 != 0)
		{
			printf("%d\n", iArr[i_bdj]);
		}
	}
	return 0;
}