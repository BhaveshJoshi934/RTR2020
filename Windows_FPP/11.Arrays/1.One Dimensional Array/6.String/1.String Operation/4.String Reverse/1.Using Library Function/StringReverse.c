#include<stdio.h>

#define MAX_LENGTH 512

int main(void)
{
	char str_bdj[MAX_LENGTH];

	printf("\n\n");
	printf("Enter the string you want:\n");
	gets_s(str_bdj, MAX_LENGTH);

	printf("\n\n");
	printf("You Have enter(Original) : %s\n", str_bdj);

	printf("\n\n");
	printf("Reverse of original string is:\n");
	printf("%s\n", strrev(str_bdj));

	return 0;
}