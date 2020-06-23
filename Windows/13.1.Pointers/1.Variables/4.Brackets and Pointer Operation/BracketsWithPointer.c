#include<stdio.h>

int main(void)
{
	int num_bdj;
	int* ptr_bdj = NULL;
	int ans_bdj;

	num_bdj = 27;
	ptr_bdj = &num_bdj;

	printf("\n\n");
	printf("Number is:                           %d\n", num_bdj);
	printf("Number's Address is:                 %p\n", &num_bdj);
	printf("Value of Number using pointer is:    %d\n", *(&num_bdj));
	printf("Number's Address is:                 %p\n", ptr_bdj);
	printf("Value of Number using pointer is:    %d\n", *ptr_bdj);

	printf("\n\n");

	printf("(ptr + 10):                          %p\n", (ptr_bdj + 10));
	printf("*(ptr + 10):                         %d\n", *(ptr_bdj + 10));
	printf("(*ptr + 10):                         %d\n", (*ptr_bdj + 10));
	++*ptr_bdj;
	printf("Answer of Pre-Increment *ptr_bdj:    %d\n", *ptr_bdj);
	*ptr_bdj++;
	printf("Answer of Post-Increment *ptr_bdj:   %d\n", *ptr_bdj);
	ptr_bdj = &num_bdj;
	(*ptr_bdj)++;
	printf("Answer of (*ptr_bdj)++ :             %d\n", *ptr_bdj);


	return 0;
}