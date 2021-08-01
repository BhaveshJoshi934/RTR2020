#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
	int i_bdj;

	if (argc != 4)
	{
		printf("\n\n");
		printf("You have entered less arguments..So exiting out of loop!!!\n\n");
		printf("Usage:App_2.exe <name 1> <name 2> <name 3>");
		printf("\n\n");
		exit(0);
	}
	printf("\n\n");
	for (i_bdj = 1; i_bdj < argc; i_bdj++)
	{
		printf("%s ", argv[i_bdj]);
	}
	printf("\n\n");

	return 0;
}