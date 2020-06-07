#include<stdio.h>
int main(void)
{
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

	printf("\n\n");
	printf("Inline Initialization and PriceMeal Display of elements of iArr :\n\n");
	printf("iArr[0] of 1st element is  : %d\n", iArr_bdj[0]);
	printf("iArr[1] of 2nd element is  : %d\n", iArr_bdj[1]);
	printf("iArr[2] of 3rd element is  : %d\n", iArr_bdj[2]);
	printf("iArr[3] of 4th element is  : %d\n", iArr_bdj[3]);
	printf("iArr[4] of 5th element is  : %d\n", iArr_bdj[4]);
	printf("iArr[5] of 6th element is  : %d\n", iArr_bdj[5]);
	printf("iArr[6] of 7th element is  : %d\n", iArr_bdj[6]);
	printf("iArr[7] of 8th element is  : %d\n", iArr_bdj[7]);
	printf("iArr[8] of 9th element is  : %d\n", iArr_bdj[8]);
	printf("iArr[9] of 10th element is : %d\n", iArr_bdj[9]);

	int_size_bdj = sizeof(int);
	iArr_size_bdj = sizeof(iArr_bdj);
	iArr_num_elements_bdj = iArr_size_bdj / int_size_bdj;

	printf("Size Of int                          : %d bytes\n", int_size_bdj);
	printf("Number of Elements in iArr           : %d elements\n", iArr_num_elements_bdj);
	printf("Size of iArr(%d elements * %d bytes) : %d bytes\n", iArr_num_elements_bdj, int_size_bdj, iArr_size_bdj);


	printf("\n\n");
	printf("Inline Initialization and PriceMeal Display of elements of fArr :\n\n");
	printf("fArr[0] of 1st element is  : %f\n", fArr_bdj[0]);
	printf("fArr[1] of 2nd element is  : %f\n", fArr_bdj[1]);
	printf("fArr[2] of 3rd element is  : %f\n", fArr_bdj[2]);
	printf("fArr[3] of 4th element is  : %f\n", fArr_bdj[3]);
	printf("fArr[4] of 5th element is  : %f\n", fArr_bdj[4]);
	printf("fArr[5] of 6th element is  : %f\n", fArr_bdj[5]);
	printf("fArr[6] of 7th element is  : %f\n", fArr_bdj[6]);
	printf("fArr[7] of 8th element is  : %f\n", fArr_bdj[7]);
	printf("fArr[8] of 9th element is  : %f\n", fArr_bdj[8]);
	printf("fArr[9] of 10th element is : %f\n", fArr_bdj[9]);

	float_size_bdj = sizeof(float);
	fArr_size_bdj = sizeof(fArr_bdj);
	fArr_num_elements_bdj = fArr_size_bdj / float_size_bdj;

	printf("Size Of float                          : %f bytes\n", float_size_bdj);
	printf("Number of Elements in fArr             : %f elements \n", fArr_num_elements_bdj);
	printf("Size of fArr(%f elements * %f bytes)   : %f bytes\n", fArr_num_elements_bdj, float_size_bdj, fArr_size_bdj);


	printf("\n\n");
	printf("Inline Initialization and PriceMeal Display of elements of cArr :\n\n");
	printf("cArr[0] of 1st element is  : %c\n", cArr_bdj[0]);
	printf("cArr[1] of 2nd element is  : %c\n", cArr_bdj[1]);
	printf("cArr[2] of 3rd element is  : %c\n", cArr_bdj[2]);
	printf("cArr[3] of 4th element is  : %c\n", cArr_bdj[3]);
	printf("cArr[4] of 5th element is  : %c\n", cArr_bdj[4]);
	printf("cArr[5] of 6th element is  : %c\n", cArr_bdj[5]);
	printf("cArr[6] of 7th element is  : %c\n", cArr_bdj[6]);
	printf("cArr[7] of 8th element is  : %c\n", cArr_bdj[7]);
	printf("cArr[8] of 9th element is  : %c\n", cArr_bdj[8]);
	printf("cArr[9] of 10th element is : %c\n", cArr_bdj[9]);
	printf("cArr[9] of 11th element is : %c\n", cArr_bdj[10]);
	printf("cArr[9] of 12th element is : %c\n", cArr_bdj[11]);

	char_size_bdj = sizeof(char);
	cArr_size_bdj = sizeof(cArr_bdj);
	cArr_num_elements_bdj = cArr_size_bdj / char_size_bdj;

	printf("Size Of char                           : %d \n", char_size_bdj);
	printf("Number of Elements in cArr             : %d\n", cArr_num_elements_bdj);
	printf("Size of cArr(%d elements * %d bytes)   : %d \n", cArr_num_elements_bdj, char_size_bdj, cArr_size_bdj);

	return 0;
}