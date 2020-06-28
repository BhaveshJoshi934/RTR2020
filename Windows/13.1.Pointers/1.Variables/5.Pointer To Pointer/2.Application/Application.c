#include<stdio.h>

int main(void)
{
	int num_bdj = 5;
	int* ptr_bdj = NULL;
	int** pptr_bdj = NULL;

	printf("*********Before ptr_bdj = &num*********\n\n");
	printf("Value of num_bdj           :      %d\n", num_bdj);
	printf("Address of num_bdj         :      %p\n", &num_bdj);
	printf("Value using pointer        :      %d\n", *(&num_bdj));

	ptr_bdj = &num_bdj;

	printf("*********After ptr_bdj = &num*********\n\n");
	printf("Value of num_bdj           :      %d\n", num_bdj);
	printf("Address of num_bdj         :      %p\n", ptr_bdj);
	printf("Value using pointer        :      %d\n", *ptr_bdj);

	pptr_bdj = &ptr_bdj;

	printf("*********After pptr_bdj = &ptr_bdj*********\n\n");
	printf("Value of num_bdj           :      %d\n", num_bdj);
	printf("Address of num_bdj         :      %p\n", ptr_bdj);
	printf("Address of ptr_bdj         :      %p\n", pptr_bdj);
	printf("Value of ptr using pptr    :      %p\n", *pptr_bdj);
	printf("Value of num using pptr    :      %d\n", **pptr_bdj);

	return 0;
}