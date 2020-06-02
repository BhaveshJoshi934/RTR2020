#include<stdio.h>
int main(void)
{
	float f_bdj;
	float f_num_bdj = 3.9f;
	printf("\n\n");
	printf("Floating point Numbers %f to %f are:\n", f_num_bdj, (f_num_bdj * 10.0f));

	for (f_bdj = f_num_bdj; f_bdj <= (f_num_bdj * 10.0f); f_bdj = f_bdj + f_num_bdj)
	{
		printf("\t%f\n", f_bdj);
	}
	printf("\n\n");
	return 0;
}