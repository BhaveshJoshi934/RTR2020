#include <stdio.h>
int main(void)
{
	int a_bdj = 5;
	int b_bdj = 10;

	//code 

	printf("\n\n");
	printf("a_bdj = %d\n", a_bdj++);
	printf("a_bdj = %d\n", a_bdj);
	printf("a_bdj = %d\n", ++a_bdj);
	printf("a_bdj = %d\n\n", a_bdj);

	printf("b_bdj = %d\n", b_bdj--);
	printf("b_bdj = %d\n", b_bdj);
	printf("b_bdj = %d\n", --b_bdj);
	printf("b_bdj = %d\n\n", b_bdj);

	printf("\n\n");
	return 0;
}