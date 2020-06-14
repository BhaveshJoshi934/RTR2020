#include<stdio.h>

int main(void)
{
	struct MyData
	{
		int i_bdj;
		float f_bdj;
		double d_bdj;
		char c_bdj;
	}bhavesh_bdj = {11,12.3f,3456.98765,'S'};

	printf("\n\n");
	printf("i = %d\n", bhavesh_bdj.i_bdj);
	printf("c = %c\n", bhavesh_bdj.c_bdj);
	printf("d = %lf\n", bhavesh_bdj.d_bdj);
	printf("f = %f\n", bhavesh_bdj.f_bdj);

	printf("\n\n");

	return 0;
}