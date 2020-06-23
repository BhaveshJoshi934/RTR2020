#include<stdio.h>

int main(void)
{
	float f_num_bdj;
	float *f_ptr_bdj = NULL;

	f_num_bdj = 3.14f;

	printf("\n\n");
	printf("********Before f_ptr_bdj = &f_num_bdj********\n\n");
	printf("Number is :              %f\n", f_num_bdj);
	printf("Address is :             %p\n", &f_num_bdj);
	printf("Value using Address:     %f\n", *(&f_num_bdj));

	f_ptr_bdj = &f_num_bdj;

	printf("\n\n");
	printf("********After f_ptr_bdj = &f_num_bdj********\n\n");
	printf("Number is :              %f\n", f_num_bdj);
	printf("Address is :             %p\n", &f_ptr_bdj);
	printf("Value using Address:     %f\n", *f_ptr_bdj);

	return 0;
}