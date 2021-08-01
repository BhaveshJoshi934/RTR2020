#include<stdio.h>
int main(void)
{
	char option_bdj, ch_bdj = '\0';
	printf("\n\n");
	printf("Enter 'Q' or 'q' : Quit the loop\n");
	printf("Enter 'Y' or 'y' : User control loop\n");
	printf("\n\n");
	do
	{
		do
		{
			printf("\n");
			printf("In Loop...\n");
			ch_bdj = getch();
		} while (ch_bdj == 'Q' || ch_bdj == 'q');
		printf("\n\n");
		printf("Exiting infinite loop\n");
		printf("\n\n");
		printf("Do you want to start loop again?..Press 'Y' or 'y' if yes...\n");
	} while (ch_bdj == 'Y' || ch_bdj == 'y');

	return 0;
}