#include<stdio.h>

int main(void)
{
	int num_bdj;
	int* num_ptr1 = NULL;
	int* num_ptr2 = NULL;

	num_bdj = 11;
	num_ptr1 = &num_bdj;

	printf("\n\n");
	printf("***********Before num_ptr2 = num_ptr1***********\n\n");

	printf("Number is:                        %d\n", num_bdj);
	printf("Number's Address is:              %p\n", &num_bdj);
	printf("Number Value using Address is:    %d\n", *(&num_bdj));
	printf("Number's Address is:              %p\n", &num_ptr1);
	printf("Number Value using Address is:    %d\n", *num_ptr1);

	num_ptr2 = num_ptr1;

	printf("\n\n");
	printf("***********After num_ptr2 = num_ptr1***********\n\n");

	printf("Number is:                        %d\n", num_bdj);
	printf("Number's Address is:              %p\n", &num_bdj);
	printf("Number Value using Address is:    %d\n", *(&num_bdj));
	printf("Number's Address is:              %p\n", &num_ptr1);
	printf("Number Value using Address is:    %d\n", *num_ptr1);
	printf("Number's Address is:              %p\n", &num_ptr2);
	printf("Number Value using Address is:    %d\n", *num_ptr2);


	return 0;
}