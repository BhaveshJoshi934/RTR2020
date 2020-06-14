#include<stdio.h>
int main(void)
{
	int iArray_3D[5][3][2] = { { {1,2} , {3,4} , {5,6} },
							{ {7,8} , {9,10} , {11,12} },
							{ {13,14},{15,16},{17,18} },
							{ {19,20},{21,22},{23,24} },
							{ {25,26},{27,28},{29,30} } };

	int int_size;
	int iArray_size;
	int iArrayNum, iWidth, iHeight, iDepth;

	printf("\n\n");
	int_size = sizeof(int);
	printf("Size of int : %d\n\n", int_size);

	iArray_size = sizeof(iArray_3D);
	printf("Size of 3D array : %d\n\n", iArray_size);

	iWidth = iArray_size / sizeof(iArray_3D[0]);
	printf("Width of an array : %d\n\n", iWidth);

	iHeight = sizeof(iArray_3D[0]) / sizeof(iArray_3D[0][0]);
	printf("Height of an array: %d\n\n", iHeight);

	iDepth = sizeof(iArray_3D[0][0]) / int_size;
	printf("Depth of an array : %d\n\n", iDepth);

	iArrayNum = iWidth * iDepth * iHeight;
	printf("Total Number of Element : %d\n\n", iArrayNum);

	printf("\n\n");
	printf("Elements in 3D array:\n\n");

	printf("****Row 1****\n");
	printf("*****Column 1*****\n");
	printf("iArray[0][0][0] = %d\n", iArray_3D[0][0][0]);
	printf("iArray[0][0][1] = %d\n\n", iArray_3D[0][0][1]);
	printf("*****Column 2*****\n");
	printf("iArray[0][1][0] = %d\n", iArray_3D[0][1][0]);
	printf("iArray[0][1][1] = %d\n\n", iArray_3D[0][1][1]);
	printf("*****Column 3*****\n");
	printf("iArray[0][2][0] = %d\n", iArray_3D[0][2][0]);
	printf("iArray[0][2][1] = %d\n\n", iArray_3D[0][2][1]);

	printf("****Row 2****\n");
	printf("*****Column 1*****\n");
	printf("iArray[1][0][0] = %d\n", iArray_3D[1][0][0]);
	printf("iArray[1][0][1] = %d\n\n", iArray_3D[1][0][1]);
	printf("*****Column 2*****\n");
	printf("iArray[1][1][0] = %d\n", iArray_3D[1][1][0]);
	printf("iArray[1][1][1] = %d\n\n", iArray_3D[1][1][1]);
	printf("*****Column 3*****\n");
	printf("iArray[1][2][0] = %d\n", iArray_3D[1][2][0]);
	printf("iArray[1][2][1] = %d\n\n", iArray_3D[1][2][1]);

	printf("****Row 3****\n");
	printf("*****Column 1*****\n");
	printf("iArray[2][0][0] = %d\n", iArray_3D[2][0][0]);
	printf("iArray[2][0][1] = %d\n\n", iArray_3D[2][0][1]);
	printf("*****Column 2*****\n");
	printf("iArray[2][1][0] = %d\n", iArray_3D[2][1][0]);
	printf("iArray[2][1][1] = %d\n\n", iArray_3D[2][1][1]);
	printf("*****Column 3*****\n");
	printf("iArray[2][2][0] = %d\n", iArray_3D[2][2][0]);
	printf("iArray[2][2][1] = %d\n\n", iArray_3D[2][2][1]);

	printf("****Row 4****\n");
	printf("*****Column 1*****\n");
	printf("iArray[3][0][0] = %d\n", iArray_3D[3][0][0]);
	printf("iArray[3][0][1] = %d\n\n", iArray_3D[3][0][1]);
	printf("*****Column 2*****\n");
	printf("iArray[3][1][0] = %d\n", iArray_3D[3][1][0]);
	printf("iArray[3][1][1] = %d\n\n", iArray_3D[3][1][1]);
	printf("*****Column 3*****\n");
	printf("iArray[3][2][0] = %d\n", iArray_3D[3][2][0]);
	printf("iArray[3][2][1] = %d\n\n", iArray_3D[3][2][1]);

	printf("****Row 5****\n");
	printf("*****Column 1*****\n");
	printf("iArray[4][0][0] = %d\n", iArray_3D[4][0][0]);
	printf("iArray[4][0][1] = %d\n\n", iArray_3D[4][0][1]);
	printf("*****Column 2*****\n");
	printf("iArray[4][1][0] = %d\n", iArray_3D[4][1][0]);
	printf("iArray[4][1][1] = %d\n\n", iArray_3D[4][1][1]);
	printf("*****Column 3*****\n");
	printf("iArray[4][2][0] = %d\n", iArray_3D[4][2][0]);
	printf("iArray[4][2][1] = %d\n\n", iArray_3D[4][2][1]);

	return 0;
}