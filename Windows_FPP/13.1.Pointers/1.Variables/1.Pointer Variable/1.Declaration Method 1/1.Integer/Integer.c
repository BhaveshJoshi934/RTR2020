#include<stdio.h>

int main(void)
{
	int num;
	int* ptr = NULL;

	num = 10;

	printf("\n\n");
	printf("Before Pointer is ptr = &num\n\n");
	printf("Number is: \t\t %d \n", num);
	printf("Address is: \t\t %p\n", &num);
	printf("Value using pointer: \t\t %d\n\n", *(&num));


	ptr = &num;

	printf("\n\n");
	printf("After Pointer is ptr = &num\n\n");
	printf("Number is: \t\t %d \n", num);
	printf("Address is: \t\t %p\n", ptr);
	printf("Value using pointer: \t\t %d\n\n", *ptr);

	return 0;
}