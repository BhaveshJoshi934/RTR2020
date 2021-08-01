#include<stdio.h>
int main(int argc, char* argv[],char *envp[])
{
	int i_bdj;
	printf("\n\n");
	printf("Hi Ajay!!!\n");
	printf("Number of arguments is/are: %d\n", argc);

	printf("Number of command lines passed to program are as below...\n");
	for (i_bdj = 0; i_bdj < argc; i_bdj++)
	{
		printf("Commnd Line argument number %d = %s\n\n", (i_bdj + 1), argv[i_bdj]);
	}
	printf("\n\n");

	printf("First 20 environmental variable paased to program are...\n\n");
	for (i_bdj = 0; i_bdj < 20; i_bdj++)
	{
		printf("Environmental variable number %d = %s\n\n", (i_bdj + 1), envp[i_bdj]);
	}
	printf("\n\n");

	return 0;
}