#include<stdio.h>

#define MAX_STRING_LENGTH_BDJ 512

int main(void)
{
	int MyStrlen_bdj(char[]);

	char strArr_bdj[10][15] = { "Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2020-21)", "Of", "ASTROMEDICOMP." };

	int iStrArrLength[10];
	int strArr_size_bdj;
	int strArr_num_rows_bdj;
	int i_bdj,j_bdj;

	strArr_size_bdj = sizeof(strArr_bdj);
	strArr_num_rows_bdj = strArr_size_bdj / sizeof(strArr_bdj[0]);

	for (i_bdj = 0; i_bdj < strArr_num_rows_bdj; i_bdj++)
	{
		iStrArrLength[i_bdj] = MyStrlen_bdj(strArr_bdj[i_bdj]);
	}
	printf("\n\n");
	printf("Entire string array : \n");
	for (i_bdj = 0; i_bdj < strArr_num_rows_bdj; i_bdj++)
	{
		printf("%s ", strArr_bdj[i_bdj]);
	}
	printf("\n\n");
	printf("Strings in 2-D Array:\n");

	for (i_bdj = 0; i_bdj < strArr_num_rows_bdj; i_bdj++)
	{
		printf("String Number %d = %s\n", (i_bdj + 1), strArr_bdj[i_bdj]);
		for (j_bdj = 0; j_bdj < iStrArrLength[i_bdj]; j_bdj++)
		{
			printf("Character %d = %c \n", (j_bdj + 1), strArr_bdj[i_bdj][j_bdj]);
		}
		printf("\n\n");
	}
	return 0;
}

int MyStrlen_bdj(char str[])
{
	int j_bdj;
	int stringlrngth_bdj = 0;

	for (j_bdj = 0; j_bdj < MAX_STRING_LENGTH_BDJ; j_bdj++)
	{
		if (str[j_bdj] != '\0')
		{
			stringlrngth_bdj++;
		}
		else
		{
			break;
		}
	}
	return stringlrngth_bdj;
}

