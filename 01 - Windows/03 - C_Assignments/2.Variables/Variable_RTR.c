#include <stdio.h>

int main(void)
{
	//variable declarations 
	int i_bdj = 202;
	float f_bdj = 5.6f;
	double d_bdj = 3.071996;
	char c_bdj = 'K';

	//code 
	printf("\n\n");

	printf("i_bdj = %d\n", i_bdj);
	printf("f_bdj = %f\n", f_bdj);
	printf("d_bdj = %lf\n", d_bdj);
	printf("c_bdj = %c\n", c_bdj);

	printf("\n\n");
	
	i_bdj = 041;
	f_bdj = 6.7f;
	d_bdj = 8.111996;
	c_bdj = 'B';

	printf("i_bdj = %d\n", i_bdj);
	printf("f_bdj = %f\n", f_bdj);
	printf("d_bdj = %lf\n", d_bdj);
	printf("c_bdj = %c\n", c_bdj);

	printf("\n\n");
	return 0;
}