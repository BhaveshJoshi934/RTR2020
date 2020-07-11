#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS   5
#define NUM_COLS   3

int main(void)
{
	int i_bdj,j_bdj;
	int** ptr_iArray_bdj = NULL;

	ptr_iArray_bdj = (int**)malloc(NUM_ROWS * sizeof(int*));
	if (ptr_iArray_bdj == NULL)
	{
		printf("Memeory Allocation Falied!!!\n\n");
		exit(0);
	}
	else
	{
		printf("Memeory Allocation succeeded!!!\n\n");
	}

	for (i_bdj = 0; i_bdj < NUM_ROWS; i_bdj++)
	{
		ptr_iArray_bdj[i_bdj] = (int*)malloc(NUM_COLS * sizeof(int));
		if (ptr_iArray_bdj == NULL)
		{ 
			printf("Memeory Allocation Failed!!!\n\n");  
			exit(0);
		}
		else
		{
			printf("Memeory Allocation Succeeded!!!\n\n");
		}
	}

	for (i_bdj = 0; i_bdj < NUM_COLS; i_bdj++)
	{
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{
			*(*(ptr_iArray_bdj + i_bdj) + j_bdj) = (i_bdj + 1) * (j_bdj + 1);
		}
	}

	printf("\n\n");
	printf("2D Integer Array Elements Along With Addresses : \n\n");
	for (i_bdj = 0; i_bdj < NUM_ROWS; i_bdj++)
	{ 
		for (j_bdj = 0; j_bdj < NUM_COLS; j_bdj++)
		{ 
			printf("ptr_iArray_Row[%d][%d] = %d \t \t At Address &ptr_iArray_Row[%d][%d] : %p\n", i_bdj, j_bdj, ptr_iArray_bdj[i_bdj][j_bdj], i_bdj, j_bdj, &ptr_iArray_bdj[i_bdj][j_bdj]); 
		}  
		printf("\n\n"); 
	}

	for (i_bdj = (NUM_ROWS - 1); i_bdj >= 0; i_bdj--)
	{
		if (*(ptr_iArray_bdj + i_bdj))  
		{   free(*(ptr_iArray_bdj + i_bdj)); 
		*(ptr_iArray_bdj + i_bdj) = NULL; 
		printf("MEMORY FREED !!! \n\n");
		} 
	}

	if (ptr_iArray_bdj)
	{
		free(ptr_iArray_bdj);
		ptr_iArray_bdj = NULL;  
		printf("MEMORY FREED !!! \n\n");
	}
	return 0;
}