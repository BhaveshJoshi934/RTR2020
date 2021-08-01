#include<stdio.h>

int main(void)
{
	char cArr_bdj[26];
	int i_bdj;
	char* ptr_bdj = NULL;


	for (i_bdj = 0; i_bdj < 26; i_bdj++)
	{
		cArr_bdj[i_bdj] = (char)(i_bdj + 65);
	}

	printf("\n\n");
	printf("Elements in the Character Array are:\n\n");

	ptr_bdj = cArr_bdj;

	printf("\n\n");
	for (i_bdj = 0; i_bdj < 26; i_bdj++)
	{
		printf("cArr[%d] = %c\n", i_bdj, *(ptr_bdj + i_bdj));
	}

	printf("Elements in the Character Array and its Addresses are:\n\n");
	printf("\n\n");
	for (i_bdj = 0; i_bdj < 26; i_bdj++)
	{
		printf("iArr[%d] = %c\t And Address is  = %p\n", i_bdj, *(ptr_bdj + i_bdj), (ptr_bdj + i_bdj));
	}
	printf("\n\n");
	return 0;
}