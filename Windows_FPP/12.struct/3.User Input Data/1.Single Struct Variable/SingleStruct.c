#include<stdio.h>
struct MyData
{
	char c_bdj;
	int i_bdj;
	float f_bdj;
	double d_bdj;
};

int main(void)
{
	struct MyData data_bdj;

	printf("\n\n");
	printf("Enter value for c:\n");
	scanf("%c", &data_bdj.c_bdj);

	printf("Enter value for i:\n");
	scanf("%d", &data_bdj.i_bdj);

	printf("Enter value for f:\n");
	scanf("%f", &data_bdj.f_bdj);

	printf("Enter value for d:\n");
	scanf("%lf", &data_bdj.d_bdj);

	printf("\n\n");
	printf("i = %d\n", data_bdj.i_bdj);
	printf("c = %c\n", data_bdj.c_bdj);
	printf("d = %lf\n",data_bdj.d_bdj);
	printf("f = %f\n", data_bdj.f_bdj);
	printf("\n\n");

	return 0;
}