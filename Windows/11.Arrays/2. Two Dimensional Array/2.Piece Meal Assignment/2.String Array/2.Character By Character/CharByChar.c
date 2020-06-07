#include<stdio.h>

#define MAX_STRING_LENGTH_BDJ 512

int main(void)
{

	int MyStrlen_bdj(char[]);

	char strArr_bdj[5][10];

	int char_size_bdj;
	int strArr_size_bdj;
	int strArr_num_elements_bdj, strArr_num_rows_bdj, strArr_num_column_bdj;
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


	strArr_bdj[0][0] = 'M';
	strArr_bdj[0][1] = 'Y';
	strArr_bdj[0][2] = '\0';

	strArr_bdj[1][0] = 'N';
	strArr_bdj[1][1] = 'A';
	strArr_bdj[1][2] = 'M';
	strArr_bdj[1][3] = 'E';
	strArr_bdj[1][4] = '\0';

	strArr_bdj[2][0] = 'I';
	strArr_bdj[2][1] = 's';
	strArr_bdj[2][2] = '\0';

	strArr_bdj[3][0] = 'B';
	strArr_bdj[3][1] = 'H';
	strArr_bdj[3][2] = 'A';
	strArr_bdj[3][3] = 'V';
	strArr_bdj[3][4] = 'E';
	strArr_bdj[3][5] = 'S';
	strArr_bdj[3][6] = 'H';
	strArr_bdj[3][7] = '\0';

	strArr_bdj[4][0] = 'J';
	strArr_bdj[4][1] = 'O';
	strArr_bdj[4][2] = 'S';
	strArr_bdj[4][3] = 'H';
	strArr_bdj[4][4] = 'I';
	strArr_bdj[4][5] = '\0';

	printf("\n\n");
	for (i_bdj = 0; i_bdj < strArr_num_rows_bdj; i_bdj++)
	{
		printf("%s ", strArr_bdj[i_bdj]);
	}
	printf("\n\n");

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

