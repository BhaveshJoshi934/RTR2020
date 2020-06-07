#include<stdio.h>

#define INT_NUM_ARRAY_ELEMENTS_BDJ 5
#define FLOAT_NUM_ARRAY_ELEMENTS_BDJ 3
#define CHAR_NUM_ARRAY_ELEMENTS_BDJ 5

int main(void)
{
	int i_bdj;

	int iArr[INT_NUM_ARRAY_ELEMENTS_BDJ];
	float fArr[FLOAT_NUM_ARRAY_ELEMENTS_BDJ];
	char cArr[CHAR_NUM_ARRAY_ELEMENTS_BDJ];

	printf("\n\n");
	printf("Enter Elements for Integer Array:\n\n");
	for (i_bdj = 0; i_bdj < INT_NUM_ARRAY_ELEMENTS_BDJ; i_bdj++)
	{
		scanf("%d",&iArr[i_bdj]);
	}

	printf("\n\n");
	printf("Enter Elements for Flaot Array:\n\n");
	for (i_bdj = 0; i_bdj < FLOAT_NUM_ARRAY_ELEMENTS_BDJ; i_bdj++)
	{
		scanf("%f",&fArr[i_bdj]);
	}

	printf("\n\n");
	printf("Enter Elements for Character Array:\n\n");
	for (i_bdj = 0; i_bdj < CHAR_NUM_ARRAY_ELEMENTS_BDJ; i_bdj++)
	{
		cArr[i_bdj] = getch();
		printf("%c",cArr[i_bdj]);
	}


	printf("\n\n");
	printf("Elements for Integer Array:\n\n");
	for (i_bdj = 0; i_bdj < INT_NUM_ARRAY_ELEMENTS_BDJ; i_bdj++)
	{
		printf("%d\n", iArr[i_bdj]);
	}

	printf("\n\n");
	printf("Elements for Float Array:\n\n");
	for (i_bdj = 0; i_bdj < FLOAT_NUM_ARRAY_ELEMENTS_BDJ; i_bdj++)
	{
		printf("%f\n", fArr[i_bdj]);
	}

	printf("\n\n");
	printf("Elements for Character Array:\n\n");
	for (i_bdj = 0; i_bdj < CHAR_NUM_ARRAY_ELEMENTS_BDJ; i_bdj++)
	{
		printf("%c\n", cArr[i_bdj]);
	}

	return 0;
}