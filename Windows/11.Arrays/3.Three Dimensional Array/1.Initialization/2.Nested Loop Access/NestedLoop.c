#include<stdio.h>
int main(void)
{
	int i_bdj, j_bdj, k_bdj;
	int iArray_3D[5][3][2] = { { {1,2} , {3,4} , {5,6} },
							{ {7,8} , {9,10} , {11,12} },
							{ {13,14},{15,16},{17,18} },
							{ {19,20},{21,22},{23,24} },
							{ {25,26},{27,28},{29,30} } };

	int int_size;
	int iArray_size;
	int iArrayNum, iWidth, iHeight, iDepth;

	int_size = sizeof(int);
	printf("Size of int : %d\n\n", int_size);

	iArray_size = sizeof(iArray_3D);
	printf("Size of an array : %d\n\n", iArray_size);

	iWidth = iArray_size / sizeof(iArray_3D[0]);
	printf("Width of an Array: %d\n\n", iWidth);

	iHeight = sizeof(iArray_3D[0]) / sizeof(iArray_3D[0][0]);
	printf("Height of an Array : %d\n\n", iHeight);

	iDepth = sizeof(iArray_3D[0][0]) / sizeof(iArray_3D[0][0][0]);
	printf("Width of an Array : %d\n\n", iDepth);

	iArrayNum = iWidth * iHeight * iDepth;
	printf("Number of elements in an array\n\n", iArrayNum);

	for (i_bdj = 0; i_bdj < iWidth; i_bdj++)
	{
		printf("*************Row %d***************\n",(i_bdj+1));
		for (j_bdj = 0; j_bdj < iHeight; j_bdj++)
		{
			printf("*********Column %d***********\n",(j_bdj+1));
			for (k_bdj = 0; k_bdj < iDepth; k_bdj++)
			{
				printf("iArray3D[%d][%d][%d] = %d\n", i_bdj,j_bdj,k_bdj, iArray_3D[i_bdj][j_bdj][k_bdj]);
			}
			printf("\n");
		}
		printf("\n\n");
	}



	return 0;
}