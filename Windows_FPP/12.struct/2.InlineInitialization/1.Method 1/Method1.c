#include<stdio.h>

struct MyData
{
	int i_bdj;
	char c_bdj;
	float f_bdj;
	double d_bdj;
}data_bdj = {12,'B',3.14f,123.654};

int main(void)
{
	//data_bdj.i_bdj = 12;
	//data_bdj.c_bdj = 'B';
	//data_bdj.f_bdj = 3.678f;
	//data_bdj.d_bdj = 12340.9876;

	printf("\n\n");

	printf("i = %d\n", data_bdj.i_bdj);
	printf("f = %f\n", data_bdj.f_bdj);
	printf("c = %c\n", data_bdj.c_bdj);
	printf("d = %lf\n", data_bdj.d_bdj);
	printf("\n\n");

	return 0;
}