#include<stdio.h>

int main(void)
{
	char c_char_bdj;
	char *c_ptr_bdj = NULL;

	c_char_bdj = 'B';

	printf("\n\n");
	printf("********Before c_ptr_bdj = &c_char_bdj********\n\n");
	printf("Character is :              %c\n", c_char_bdj);
	printf("Address is :                %p\n", &c_char_bdj);
	printf("Value using Address:        %c\n", *(&c_char_bdj));

	c_ptr_bdj = &c_char_bdj;

	printf("\n\n");
	printf("********After c_ptr_bdj = &c_char_bdj********\n\n");
	printf("Character is :              %c\n", c_char_bdj);
	printf("Address is :                %p\n", &c_ptr_bdj);
	printf("Value using Address:        %c\n", *c_ptr_bdj);

	return 0;
}