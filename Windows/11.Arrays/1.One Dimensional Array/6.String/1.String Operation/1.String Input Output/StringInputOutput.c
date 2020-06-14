#include<stdio.h>
#define MAX_LENGTH_NAME 512

int main(void)
{
	char ch_bdj[MAX_LENGTH_NAME];
	printf("\n\n");
	printf("Enter a String:\n");
	gets_s(ch_bdj, MAX_LENGTH_NAME);

	printf("\n\n");
	printf("String Enter By you:\n");
	printf("%s\n", ch_bdj);

	return 0;
}