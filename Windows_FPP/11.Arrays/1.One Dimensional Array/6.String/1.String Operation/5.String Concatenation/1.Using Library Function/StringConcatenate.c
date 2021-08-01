#include<stdio.h>
#define MAX_LENGTH_BDJ 512

int main(void)
{
	char str_bdj_1[MAX_LENGTH_BDJ];
	char str_bdj_2[MAX_LENGTH_BDJ];

	printf("\n\n");
	printf("Enter First String :\n");
	gets_s(str_bdj_1, MAX_LENGTH_BDJ);

	printf("\n\n");
	printf("Enter Second String :\n");
	gets_s(str_bdj_2, MAX_LENGTH_BDJ);

	printf("\n\n");
	printf("Your First String(Before Concatenation): %s\n", str_bdj_1);

	printf("\n\n");
	printf("Your Second String(Before Concatenation): %s\n", str_bdj_2);

	strcat(str_bdj_1, str_bdj_2);

	printf("\n\n");
	printf("Your First String(After Concatenation): %s\n", str_bdj_1);

	printf("\n\n");
	printf("Your Second String(After Concatenation): %s\n", str_bdj_2);




	return 0;
}