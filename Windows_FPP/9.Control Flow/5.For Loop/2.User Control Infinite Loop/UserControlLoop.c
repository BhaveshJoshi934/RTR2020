#include<stdio.h>
int main(void)
{
	char option_bdj, ch_bdj = '\0';
	printf("\n\n");
	printf("Enter 'Q' or 'q' : Quit the loop\n");
	printf("Enter 'Y' or 'y' : User control loop\n");
	printf("\n\n");
	option_bdj = getch();
	if (option_bdj == 'Y' || option_bdj == 'y')
	{
		for (;;)
		{
			printf("In Loop...\n");
			ch_bdj = getch();
			if (ch_bdj == 'Q' || ch_bdj == 'q')
			{
				break;
			}
		}
	}
	printf("\n\n");
	printf("EXITING USER CONTROLLED LOOP...\n");
	printf("\n\n");
	return 0;
}