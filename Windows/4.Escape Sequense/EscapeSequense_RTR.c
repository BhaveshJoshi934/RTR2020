#include <stdio.h>
int main(void)
{
	//code 
	printf("\n\n");

	printf("Going on Next Line using.....\\n Escape Sequense\n\n");
	printf("Demonstarting \t Horizontal \t Tab \t using \\t Escape sequense!!!!!\n\n");
	printf("\"This is double quoted output\" done using \\\" \\\" Escape Sequnse\n\n");
	printf("\' This is single quoted output\' done using \\\' \\\' Escape Sequnse\n\n");

	printf("\r Demonstarting Carriage Return using \\r Escape Sequense!!!\n\n");
	printf("Demonstarting \r Carriage Return using \\r Escape Sequense!!!\n\n");
	printf("\r Demonstarting Carriage \r Return using \\r Escape Sequense!!!\n\n");

	printf("Demonstrating \x41 using \\xxh Escape Sequense\n\n");
	printf("Demonstrating \102 using \\ooo Escape Sequense\n\n");

	return 0;
}