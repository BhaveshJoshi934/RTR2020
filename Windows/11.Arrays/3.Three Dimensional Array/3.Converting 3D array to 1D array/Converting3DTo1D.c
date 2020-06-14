#include<stdio.h>

#define NUM_ROWS   5 
#define NUM_COLS   3
#define DEPTH      2

int main(void)
{
	int i_bdj, j_bdj, k_bdj;
	int iArray_1D[NUM_ROWS * NUM_COLS * DEPTH];

	int iArray_3D[NUM_ROWS][NUM_COLS][DEPTH] =	{ { {1,2} , {3,4} , {5,6} },
													{ {7,8} , {9,10} , {11,12} },
													{ {13,14},{15,16},{17,18} },
													{ {19,20},{21,22},{23,24} },
													{ {25,26},{27,28},{29,30} } };


	

	printf("\n\n");

	for (i_bdj = 0; i_bdj < NUM_ROWS; i_bdj++)
	{
		printf("*************Row %d***************\n", (i_bdj + 1));
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{
			printf("*********Column %d***********\n", (j_bdj + 1));
			for (k_bdj = 0; k_bdj < DEPTH; k_bdj++)
			{
				printf("iArray3D[%d][%d][%d] = %d\n", i_bdj, j_bdj, k_bdj, iArray_3D[i_bdj][j_bdj][k_bdj]);
			}
			printf("\n");
		}
		printf("\n\n");
	}

	for (i_bdj; i_bdj < NUM_ROWS; i_bdj++)
	{
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{
			for (k_bdj = 0; k_bdj < DEPTH; k_bdj++)
			{
				iArray_1D[(i_bdj * NUM_COLS * DEPTH) + (j_bdj * DEPTH) + k_bdj] = iArray_3D[i_bdj][j_bdj][k_bdj];
			}
		}
	}

	printf("\n");

	for (i_bdj = 0; i_bdj < (NUM_ROWS * NUM_COLS * DEPTH); i_bdj++)
	{
		printf("iArray1D[%d] = %d\n", i_bdj, iArray_1D[i_bdj]);
	}

	return 0;
}