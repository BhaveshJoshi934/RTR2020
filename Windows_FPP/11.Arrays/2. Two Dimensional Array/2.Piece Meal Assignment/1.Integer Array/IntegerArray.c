#include<stdio.h>
int main(void)
{
	int iArr_bdj[3][5];
	int int_size_bdj;
	int iArr_size_bdj;
	int iArr_num_elements, iArr_num_rows, iArr_num_columns;
	int i_bdj, j_bdj;

	int_size_bdj = sizeof(int);
	printf("Size of int : %d\n", int_size_bdj);

	iArr_size_bdj = sizeof(iArr_bdj);
	printf("Size of Array is : %d\n", sizeof(iArr_bdj));

	iArr_num_rows = iArr_size_bdj / sizeof(iArr_bdj[0]);
	printf("Number Of rows : %d\n", iArr_num_rows);

	iArr_num_columns = sizeof(iArr_bdj[0]) / int_size_bdj;
	printf("Number of columns : %d\n", iArr_num_columns);

	iArr_num_elements = iArr_num_rows * iArr_num_columns;
	printf("Total Elements : %d\n\n", iArr_num_elements);

	printf("\n\n");
	printf("Elements in 2-D array:\n");

	iArr_bdj[0][0] = 33;
	iArr_bdj[0][1] = 78;
	iArr_bdj[0][2] = 49;
	iArr_bdj[0][3] = 75;
	iArr_bdj[0][4] = 20;

	iArr_bdj[1][0] = 38;
	iArr_bdj[1][1] = 5;
	iArr_bdj[1][2] = 95;
	iArr_bdj[1][3] = 28;
	iArr_bdj[1][4] = 18;

	iArr_bdj[2][0] = 87;
	iArr_bdj[2][1] = 58;
	iArr_bdj[2][2] = 16;
	iArr_bdj[2][3] = 07;
	iArr_bdj[2][4] = 64;

	for (i_bdj = 0; i_bdj < iArr_num_rows; i_bdj++)
	{
		printf("---------Rows %d---------\n", (i_bdj + 1));
		for (j_bdj = 0; j_bdj < iArr_num_columns; j_bdj++)
		{
			printf("iArr[%d][%d] = %d\n", i_bdj, j_bdj, iArr_bdj[i_bdj][j_bdj]);
		}
		printf("\n\n");
	}

	return 0;
}