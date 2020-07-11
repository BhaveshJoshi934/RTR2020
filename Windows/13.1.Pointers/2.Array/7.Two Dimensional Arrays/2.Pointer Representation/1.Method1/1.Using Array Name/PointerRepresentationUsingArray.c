#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS    5
#define NUM_COLS    3

int main(void)
{
	int iArray_bdj[NUM_ROWS][NUM_COLS];
	int i_bdj, j_bdj;

	for (i_bdj = 0; i_bdj < NUM_ROWS; i_bdj++)
	{
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{
			*(iArray_bdj[i_bdj] + j_bdj) = (i_bdj + 1) * (j_bdj + 1);
		}
	}

	printf("\n\n");
	printf("2 D Array Elemenys along with address:\n\n");

	for (i_bdj = 0; i_bdj < NUM_ROWS; i_bdj++)
	{
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{
			printf("*(iArray[%d] + %d) = %d\t\t At Address : %p\n\n", i_bdj, j_bdj, *(iArray_bdj[i_bdj] + j_bdj), (iArray_bdj[i_bdj] + j_bdj));
		}
		printf("\n\n");
	}

	return 0;
}