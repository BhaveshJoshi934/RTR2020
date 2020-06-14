#include<stdio.h>

#define NUM_ROWS   5
#define NUM_COLS   3

int main(void)
{
	int i_bdj, j_bdj,num_bdj;
	int iArray_2D_bdj[NUM_ROWS][NUM_COLS];
	int iArray_1D_bdj[NUM_ROWS * NUM_COLS];

	//Accept Logic
	printf("Enter the Elements for 2D array:\n\n");
	for (i_bdj = 0; i_bdj < NUM_ROWS; i_bdj++)
	{
		printf("Number of Row = %d\n", (i_bdj + 1));
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{
			printf("Enter element Number = %d\n", (j_bdj + 1));
			scanf("%d", &num_bdj);
			iArray_2D_bdj[i_bdj][j_bdj] = num_bdj;
		}
		printf("\n\n");
	}

	//Display Logic
	printf("2D array is :\n\n");

	for (i_bdj = 0; i_bdj < NUM_ROWS; i_bdj++)
	{
		printf("******ROW %d******\n",(i_bdj + 1));
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{
			printf("iArray_2D[%d][%d] = %d\n", i_bdj, j_bdj, iArray_2D_bdj[i_bdj][j_bdj]);
		}
		printf("\n\n");
	}
	
	//Conversion Logic

	for (i_bdj = 0; i_bdj < NUM_ROWS; i_bdj++)
	{
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{
			iArray_1D_bdj[(i_bdj*NUM_COLS)+j_bdj] = iArray_2D_bdj[i_bdj][j_bdj];
		}
	}

	printf("\n\n");

	//Display converted 1D Array

	for (i_bdj = 0; i_bdj < (NUM_ROWS * NUM_COLS); i_bdj++)
	{
		printf("iArray_1D[%d] = %d\n", i_bdj, iArray_1D_bdj[i_bdj]);
	}

	printf("\n\n");

	return 0;
}