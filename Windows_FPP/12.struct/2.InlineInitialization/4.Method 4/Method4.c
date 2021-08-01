#include<stdio.h>

struct MyData
{
	int i_bdj;
	float f_bdj;
	char c_bdj;
	double d_bdj;
};

int main(void)
{
	struct MyData data_bdj_1 = { 1,1.23f,'B',1234.654 };
	struct MyData data_bdj_2 = { 2,3.54f,'D',09876.234 };
	struct MyData data_bdj_3 = { 4,6.4f,'S',345.254 };
	struct MyData data_bdj_4 = { 8,9.77f,'J',8847.564 };

	printf("\n\n");
	printf("i = %d\n", data_bdj_1.i_bdj);
	printf("f = %f\n", data_bdj_1.f_bdj);
	printf("c = %c\n", data_bdj_1.c_bdj);
	printf("d = %lf\n", data_bdj_1.d_bdj);
	printf("\n\n");

	printf("\n\n");
	printf("i = %d\n", data_bdj_2.i_bdj);
	printf("f = %f\n", data_bdj_2.f_bdj);
	printf("c = %c\n", data_bdj_2.c_bdj);
	printf("d = %lf\n", data_bdj_2.d_bdj);
	printf("\n\n");

	printf("\n\n");
	printf("i = %d\n", data_bdj_3.i_bdj);
	printf("f = %f\n", data_bdj_3.f_bdj);
	printf("c = %c\n", data_bdj_3.c_bdj);
	printf("d = %lf\n", data_bdj_3.d_bdj);
	printf("\n\n");

	printf("\n\n");
	printf("i = %d\n", data_bdj_4.i_bdj);
	printf("f = %f\n", data_bdj_4.f_bdj);
	printf("c = %c\n", data_bdj_4.c_bdj);
	printf("d = %lf\n", data_bdj_4.d_bdj);
	printf("\n\n");



	return 0;
}