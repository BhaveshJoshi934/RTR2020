#include<stdio.h>   // printf();
#include<ctype.h>   // atoi();
#include<stdlib.h>  // exit(0);

int main(int argc, char* argv[], char* envp[])
{
	int i_bdj;
	int num_bdj;
	int sum_bdj = 0;

	if (argc == 1)
	{
		printf("\n\n");
		printf("No number is given to command line argument...So Exiting from application!!!\n");
		printf("Usage : Applicatio.exe <First Number> <Second Number>\n\n");
		exit(0);
	}
	printf("\n\n");
	printf("Sum of Command Line argument is :\n\n");
	for (i_bdj = 1; i_bdj < argc; i_bdj++)
	{
		num_bdj = atoi(argv[i_bdj]);
		sum_bdj = sum_bdj + num_bdj;
	}
	printf("Sum = %d\n\n", sum_bdj);

	return 0;
}