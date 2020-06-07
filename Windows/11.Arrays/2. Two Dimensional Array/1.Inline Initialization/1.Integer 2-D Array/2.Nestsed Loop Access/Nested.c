#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj;
	int iArr_bdj[5][3] = { {1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15} };
	int int_size_bdj;
	int iArr_size_bdj;
	int i_num_Elements_bdj, i_num_rows_bdj, i_num_columns_bdj;

	printf("\n\n");
	int_size_bdj = sizeof(int);
	iArr_size_bdj = sizeof(iArr_bdj);
	printf("Size of 2-D array : %d\n", iArr_size_bdj);

	i_num_rows_bdj = iArr_size_bdj / sizeof(iArr_bdj[0]);
	printf("Number of Rows in 2-D array :%d\n", i_num_rows_bdj);

	i_num_columns_bdj = sizeof(iArr_bdj[0]) / int_size_bdj;
	printf("Number of Columns in 2-D array:%d\n", i_num_columns_bdj);

	i_num_Elements_bdj = i_num_rows_bdj * i_num_columns_bdj;
	printf("Total Elements :%d\n\n", i_num_Elements_bdj);

	printf("Elements in 2-D array are:\n");

	for (i_bdj = 0; i_bdj < i_num_rows_bdj; i_bdj++)
	{
		for (j_bdj = 0; j_bdj < i_num_columns_bdj; j_bdj++)
		{
			printf("iArr[%d][%d] = %d\n", i_bdj, j_bdj,iArr_bdj[i_bdj][j_bdj]);
		}
		printf("\n\n");
	}

	return 0;
}