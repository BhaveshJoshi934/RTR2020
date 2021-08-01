#include<stdio.h>

#define MAX_STRING_LENGTH_BDJ 512

int main(void)
{

	void MyStringCopy_bdj(char[], char[]);

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

	MyStringCopy_bdj(strArr_bdj[0], "My");
	MyStringCopy_bdj(strArr_bdj[1], "Name");
	MyStringCopy_bdj(strArr_bdj[2], "Is");
	MyStringCopy_bdj(strArr_bdj[3], "Bhavesh");
	MyStringCopy_bdj(strArr_bdj[4], "Joshi");

	printf("\n\n");

	for (i_bdj = 0; i_bdj < strArr_num_rows_bdj; i_bdj++)
	{
		printf("%s ", strArr_bdj[i_bdj]);
	}

	printf("\n\n");
	return 0;
}

void MyStringCopy_bdj(char str_dest[] , char str_src[])
{
	int MyStrlen_bdj(char[]);
	int iStringLength = 0;
	int j = 0;

	iStringLength = MyStrlen_bdj(str_src);
	for (j = 0; j < iStringLength; j++)
	{
		str_dest[j] = str_src[j];
	}
	str_dest[j] = '\0';
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


