#include<stdio.h>
int main(void)
{
	int i_bdj;
	int iArr_bdj[] = { 5,50,19,45,76,34,92,84,65,81 };
	int int_size_bdj;
	int iArr_size_bdj;
	int iArr_num_elements_bdj;

	float fArr_bdj[] = { 5.6f,5.0f,1.9f,4.5f,7.6f,3.4f,9.2f,8.4f,6.5f,8.1f };
	float float_size_bdj;
	float fArr_size_bdj;
	float fArr_num_elements_bdj;

	char cArr_bdj[] = { 'B','H','A','V','E','S','H','J','O','S','H','I' };
	char char_size_bdj;
	char cArr_size_bdj;
	char cArr_num_elements_bdj;

	int_size_bdj = sizeof(int);
	iArr_size_bdj = sizeof(iArr_bdj);
	iArr_num_elements_bdj = iArr_size_bdj / int_size_bdj;

	printf("\n\n");
	for (i_bdj = 0; i_bdj < iArr_num_elements_bdj; i_bdj++)
	{
		printf("iArr[%d] (Element %d) = %d\n", i_bdj, (i_bdj + 1), iArr_bdj[i_bdj]);
	}

	printf("Size Of int                          : %d bytes\n", int_size_bdj);
	printf("Number of Elements in iArr           : %d elements\n", iArr_num_elements_bdj);
	printf("Size of iArr(%d elements * %d bytes) : %d bytes\n", iArr_num_elements_bdj, int_size_bdj, iArr_size_bdj);

	float_size_bdj = sizeof(float);
	fArr_size_bdj = sizeof(fArr_bdj);
	fArr_num_elements_bdj = fArr_size_bdj / float_size_bdj;

	printf("\n\n");
	i_bdj = 0;
	while (i_bdj < fArr_num_elements_bdj)
	{
		printf("fArr[%d] (Element %d) = %f\n", i_bdj, (i_bdj + 1), fArr_bdj[i_bdj]);
		i_bdj++;
	}

	printf("Size Of float                          : %f bytes\n", float_size_bdj);
	printf("Number of Elements in fArr             : %f elements \n", fArr_num_elements_bdj);
	printf("Size of fArr(%f elements * %f bytes)   : %f bytes\n", fArr_num_elements_bdj, float_size_bdj, fArr_size_bdj);

	char_size_bdj = sizeof(char);
	cArr_size_bdj = sizeof(cArr_bdj);
	cArr_num_elements_bdj = cArr_size_bdj / char_size_bdj;

	printf("\n\n");
	i_bdj = 0;
	do
	{
		printf("cArr[%d] (Element %d) = %c\n", i_bdj, (i_bdj + 1), cArr_bdj[i_bdj]);
		i_bdj++;
	} while (i_bdj < cArr_num_elements_bdj);

	printf("Size Of char                           : %d \n", char_size_bdj);
	printf("Number of Elements in cArr             : %d\n", cArr_num_elements_bdj);
	printf("Size of cArr(%d elements * %d bytes)   : %d \n", cArr_num_elements_bdj, char_size_bdj, cArr_size_bdj);

	return 0;
}