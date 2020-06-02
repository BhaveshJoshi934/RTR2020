#include<stdio.h>
int main(void)
{
	int a_bdj,b_bdj;
	printf("\n\n");

	printf("Print 1-10\n\n");
	for (a_bdj = 1,b_bdj =10; a_bdj <= 10,b_bdj<=100; a_bdj++,b_bdj=b_bdj+10)
	{
		printf("\t%d\t%d\n", a_bdj,b_bdj);
	}
	printf("\n\n");
	return 0;
}