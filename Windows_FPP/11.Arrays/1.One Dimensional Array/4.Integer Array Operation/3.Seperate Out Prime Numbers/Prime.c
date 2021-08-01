#include<stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
	int iArr[NUM_ELEMENTS];
	int i_bdj, j_bdj, num_bdj, count_bdj = 0;
	printf("\n\n");
	printf("Enter the Elements in Array:\n");
	for (i_bdj = 0; i_bdj < NUM_ELEMENTS; i_bdj++)
	{
		scanf("%d", &num_bdj);
		if (num_bdj < 0)
		{
			num_bdj = (-1 * num_bdj);
		}
		iArr[i_bdj] = num_bdj;
	}

	printf("\n\n");
	printf("Array Elements are:\n");
	for (i_bdj = 0; i_bdj < NUM_ELEMENTS; i_bdj++)
	{
		printf("%d\n", iArr[i_bdj]);
	}
	printf("\n\n");
	printf("Prime Number amongst Array are:\n\n");
	for (i_bdj = 0; i_bdj < NUM_ELEMENTS; i_bdj++)
	{
		for (j_bdj = 1; j_bdj <= iArr[i_bdj]; j_bdj++)
		{
			if ((iArr[i_bdj] % j_bdj) == 0)
				count_bdj++;
		}
		if (count_bdj == 2)
			printf("%d\n", iArr[i_bdj]);
		count_bdj = 0;
	}
	return 0;
}



