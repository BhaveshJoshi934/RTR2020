#include<stdio.h>

struct MyData
{
	int i_bdj;
	char c_bdj;
	float f_bdj;
	double d_bdj;
};

struct MyData data_bdj = { 12,'B',3.14f,123.654 };

int main(void)
{
	printf("\n\n");

	printf("i = %d\n", data_bdj.i_bdj);
	printf("c = %c\n", data_bdj.c_bdj);
	printf("f = %f\n", data_bdj.f_bdj);
	printf("d = %lf\n", data_bdj.d_bdj);
	printf("\n\n");

	return 0;
}