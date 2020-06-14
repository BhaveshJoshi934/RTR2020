#include<stdio.h>
#define MAX_LENGTH_NAME 512

int main(void)
{
	char ch_bdj_src[MAX_LENGTH_NAME];
	char ch_bdj_dest[MAX_LENGTH_NAME];

	printf("\n\n");
	printf("Enter a String:\n");
	gets_s(ch_bdj_src, MAX_LENGTH_NAME);

	printf("\n\n");
	printf("String Enter By you:\n");
	printf("%s\n", ch_bdj_src);

	strcpy(ch_bdj_dest, ch_bdj_src);

	printf("\n\n");
	printf("String Copied:\n");
	printf("%s\n", ch_bdj_dest);

	return 0;
}