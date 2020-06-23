#include<stdio.h>

int main(void)
{
	int num_bdj;
	int *ptr_bdj = NULL;

	num_bdj = 10;

	printf("\n\n");
	printf("***********Before Pointer is ptr = &num***********\n\n");
	printf("Number is:             %d \n", num_bdj);
	printf("Address is:            %p\n", &num_bdj);
	printf("Value using pointer:   %d\n", *(&num_bdj));


	ptr_bdj = &num_bdj;

	printf("\n\n");
	printf("***********After Pointer is ptr = &num***********\n\n");
	printf("Number is:             %d \n", num_bdj);
	printf("Address is:            %p\n", ptr_bdj);
	printf("Value using pointer:   %d\n\n", *ptr_bdj);

	return 0;
}