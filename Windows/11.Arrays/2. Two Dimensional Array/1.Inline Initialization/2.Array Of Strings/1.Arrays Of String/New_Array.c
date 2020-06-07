#include<stdio.h>

#define MAX_STRING_LENGTH_BDJ 512

int main(void)
{
	int MyStrlen_bdj(char[]);

	char strArr_bdj[10][15] = { "Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2020-21)", "Of", "ASTROMEDICOMP." };

	int char_size_bdj;
	int strArr_size_bdj;
	int strArr_num_elements_bdj, strArr_num_rows_bdj, strArr_num_column_bdj;
	int strActual_num_characters_bdj = 0 ;
	int i_bdj;

	printf("\n\n");

	char_size_bdj = sizeof(char);
	printf("Size of character : %d\n", char_size_bdj);

	strArr_size_bdj = sizeof(strArr_bdj);
	printf("Size of character Array : %d\n", strArr_size_bdj);

	strArr_num_rows_bdj = strArr_size_bdj / sizeof(strArr_bdj[0]);
	printf("Number of Rows of 2-D array : %d\n", strArr_num_rows_bdj);

	strArr_num_column_bdj = sizeof(strArr_bdj[0]) / char_size_bdj;
	printf("Number of Columns of 2-D array : %d\n", strArr_num_column_bdj);

	strArr_num_elements_bdj = strArr_num_rows_bdj * strArr_num_column_bdj;
	printf("Total Number of Elements : %d\n", strArr_num_elements_bdj);

	for (i_bdj = 0; i_bdj < strArr_num_rows_bdj; i_bdj++)
	{
		strActual_num_characters_bdj = strActual_num_characters_bdj + MyStrlen_bdj(strArr_bdj[i_bdj]);
	}
	printf("Actal Number Of Elements are : %d\n\n", strActual_num_characters_bdj);

	printf("\n\n");
	printf("Strings in 2-D Arrays are:\n");
	printf("str_1[]  = %s \n", strArr_bdj[0]);
	printf("str_2[]  = %s \n", strArr_bdj[1]);
	printf("str_3[]  = %s \n", strArr_bdj[2]);
	printf("str_4[]  = %s \n", strArr_bdj[3]);
	printf("str_5[]  = %s \n", strArr_bdj[4]);
	printf("str_6[]  = %s \n", strArr_bdj[5]);
	printf("str_7[]  = %s \n", strArr_bdj[6]);
	printf("str_8[]  = %s \n", strArr_bdj[7]);
	printf("str_9[]  = %s \n", strArr_bdj[8]);
	printf("str_10[] = %s \n\n", strArr_bdj[9]);

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