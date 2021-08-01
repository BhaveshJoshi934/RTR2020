#include<stdio.h>
int main(void)
{
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
	printf("Total Elements :%d\n", i_num_Elements_bdj);

	printf("Elements Are:\n\n");

	printf("iArr[0][0]    = %d\n", iArr_bdj[0][0]);
	printf("iArr[0][1]    = %d\n", iArr_bdj[0][1]);
	printf("iArr[0][2]    = %d\n", iArr_bdj[0][2]);


	printf("\n\n");
	printf("iArr[1][0]    = %d\n", iArr_bdj[1][0]);
	printf("iArr[1][1]    = %d\n", iArr_bdj[1][1]);
	printf("iArr[1][2]    = %d\n", iArr_bdj[1][2]);


	printf("\n\n");
	printf("iArr[2][0]    = %d\n", iArr_bdj[2][0]);
	printf("iArr[2][1]    = %d\n", iArr_bdj[2][1]);
	printf("iArr[2][2]    = %d\n", iArr_bdj[2][2]);

	printf("\n\n");
	printf("iArr[3][0]    = %d\n", iArr_bdj[3][0]);
	printf("iArr[3][1]    = %d\n", iArr_bdj[3][1]);
	printf("iArr[3][2]    = %d\n", iArr_bdj[3][2]);

	printf("\n\n");
	printf("iArr[4][0]    = %d\n", iArr_bdj[4][0]);
	printf("iArr[4][1]    = %d\n", iArr_bdj[4][1]);
	printf("iArr[4][2]    = %d\n", iArr_bdj[4][2]);


	return 0;
}