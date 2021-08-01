#include<stdio.h>

struct MyData
{
	int i_bdj;
	float f_bdj;
	double d_bdj;
	char c_bdj;
};

int main(void)
{
	struct MyData data_bdj;

	int i_size;
	float f_size;
	double d_size;
	char c_size;
	int struc_MyData_size;

	data_bdj.i_bdj = 30;
	data_bdj.f_bdj = 3.14f;
	data_bdj.d_bdj = 1234.5678;
	data_bdj.c_bdj = 'B';

	printf("\n\n");
	printf("Data Members of 'struct MyData' are:\n\n");
	printf("i = %d\n", data_bdj.i_bdj);
	printf("f = %f\n", data_bdj.f_bdj);
	printf("d = %lf\n", data_bdj.d_bdj);
	printf("c = %c\n", data_bdj.c_bdj);
	printf("\n\n");

	i_size = sizeof(data_bdj.i_bdj);
	f_size = sizeof(data_bdj.f_bdj);
	d_size = sizeof(data_bdj.d_bdj);
	c_size = sizeof(data_bdj.c_bdj);

	printf("Sizes of All Memebers :\n");

	printf("i = %d\n", i_size);
	printf("f = %f\n", f_size);
	printf("d = %lf\n", d_size);
	printf("c = %d\n", c_size);

	struc_MyData_size = sizeof(struct MyData);
	printf("Size of Struct = %d\n", struc_MyData_size);

	return 0;
}