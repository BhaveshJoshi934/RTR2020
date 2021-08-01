#include<stdio.h>
#include<conio.h>

int main(void)
{
	int i_bdj;
	char ch_bdj;

	printf("Prints 1-100 unless user press 'Q' OR 'q'\n\n");
	for (i_bdj = 1; i_bdj <= 100; i_bdj++)
	{
		printf("\t%d\n", i_bdj);
		ch_bdj = getch();
		if (ch_bdj == 'Q' || ch_bdj == 'q')
		{
			break;
		}
	}

	printf("\n\n");
	printf("EXITING LOOP\n\n");
	printf("\n\n");
	return 0;
}