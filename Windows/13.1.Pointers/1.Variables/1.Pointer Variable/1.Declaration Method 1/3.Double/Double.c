#include<stdio.h>

int main(void)
{
	double d_num_bdj;
	double* d_ptr_bdj = NULL;

	d_num_bdj = 778.985453;

	printf("\n\n");
	printf("********Before d_ptr_bdj = &d_num_bdj********\n\n");
	printf("Number is :              %lf\n", d_num_bdj);
	printf("Address is :             %p\n", &d_num_bdj);
	printf("Value using Address:     %lf\n", *(&d_num_bdj));

	d_ptr_bdj = &d_num_bdj;

	printf("\n\n");
	printf("********After d_ptr_bdj = &d_num_bdj********\n\n");
	printf("Number is :              %lf\n", d_num_bdj);
	printf("Address is :             %p\n", &d_ptr_bdj);
	printf("Value using Address:     %lf\n", *d_ptr_bdj);

	return 0;
}