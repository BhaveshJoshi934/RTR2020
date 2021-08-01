#include<stdio.h>
int main(void)
{
	int Array_1D[5];
	int Array_2D[5][3];
	int Array_3D[100][100][5];

	int num_rows_2D;
	int num_col_2D;
	int num_of_ele_2D;

	int num_rows_3D;
	int num_col_3D;
	int depth_3D;
	int num_of_ele_3D;

	printf("\n\n");

	printf("Size of 1D arr = %lu\n", sizeof(Array_1D));
	printf("Number of elements : %lu\n", sizeof(Array_1D) / sizeof(int));

	printf("\n\n");

	printf("Size of 2D arr = %lu\n", sizeof(Array_2D));

	num_rows_2D = sizeof(Array_2D) / sizeof(Array_2D[0]);
	printf("Number of Rows in 2D array: %lu\n", num_rows_2D);

	num_col_2D = sizeof(Array_2D[0]) / sizeof(Array_2D[0][0]);
	printf("Number of Columns in 2D array: %lu\n", num_col_2D);

	num_of_ele_2D = num_rows_2D * num_col_2D;
	printf("Number of Elements : %lu\n", num_of_ele_2D);

	printf("\n\n");

	printf("Size of 3D arr = %lu\n", sizeof(Array_3D));

	num_rows_3D = sizeof(Array_3D) / sizeof(Array_3D[0]);
	printf("Number of Rows in 3D array: %lu\n", num_rows_3D);

	num_col_3D = sizeof(Array_3D[0]) / sizeof(Array_3D[0][0]);
	printf("Number of Columns in 3D array: %lu\n", num_col_3D);

	depth_3D = sizeof(Array_3D[0][0]) / sizeof(Array_3D[0][0][0]);
	printf("Depth of 3D array : %lu\n", depth_3D);

	num_of_ele_3D = num_rows_3D * num_col_3D * depth_3D;
	printf("Number of Elements : %lu\n", num_of_ele_3D);

	printf("\n\n");

	return 0;
}