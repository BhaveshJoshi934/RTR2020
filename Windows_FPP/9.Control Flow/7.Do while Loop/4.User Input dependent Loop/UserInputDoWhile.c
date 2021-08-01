#include<stdio.h>
int main(void)
{
	int a_bdj, b_bdj, i_bdj;
	printf("Enter From where iteration should begin:\n");
	scanf("%d", &a_bdj);

	printf("Enter how many digit you want to occure from %d :\n", a_bdj);
	scanf("%d", &b_bdj);

	printf("The digits are from %d to %d as \n\n", a_bdj, (a_bdj + b_bdj));
	i_bdj = a_bdj;
	do
	{
		printf("\t%d\n", i_bdj);
		i_bdj++;
	}while (i_bdj <= (a_bdj + b_bdj));

	return 0;
}