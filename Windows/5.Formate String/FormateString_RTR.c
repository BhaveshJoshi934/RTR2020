#include <stdio.h>

int main(void)
{
	printf("\n\n");
	printf("######################################################################################\n");
	printf("\n\n");

	printf("Hello World : Bhavesh Joshi!!!\n");

	int a_bdj = 202;
	printf("Integer Decimal Value of a_bdj = %d\n", a_bdj);
	printf("Integer Octal value of a_bdj = %o\n", a_bdj);
	printf("Integer Hexadecimal value of a_bdj (In Lower case) = %x\n", a_bdj);
	printf("Integer Hexadecimal value of a_bdj (In Upper case) = %X\n\n", a_bdj);

	char ch_bdj = 'A';
	printf("Character ch_bdj = %c\n", ch_bdj);
	char str_bdj[] = "This is Bhavesh Joshi from Fragment Gruop!!!\n";
	printf("String str_bdj = %s\n\n", str_bdj);

	long num_bdj = 30071996L;
	printf("Number_bdj = %ld\n\n", num_bdj);

	unsigned int b_bdj = 7;
	printf("Unsigned int value = %u\n\n", b_bdj);

	float f_bdj = 2607.1993f;
	printf("Float point number with just %%f f_bdj = %f\n", f_bdj);
	printf("Floating Point Number With %%4.2f f_bdj = %4.2f\n", f_bdj);
	printf("Floating Point Number With %%6.5f f_bdj = %6.5f\n\n", f_bdj);
	 
	double d_bdj = 3.14159265358979323846;
	printf("Double Precision Floating Point Number Without Exponential = %g\n", d_bdj);
	printf("Double Precision Floating Point Number With Exponential (lower Case) = %e\n", d_bdj);
	printf("Double Precision Floating Point Number With Exponential (upper Case) = %E\n\n", d_bdj);
	printf("Double Hexadecimal Value Of d_bdj (Hexadecimal : Lower Case) = %a\n", d_bdj);
	printf("Double Hexadecimal Value Of d_bdj (Hexadecimal : Upper Case) = %A\n\n", d_bdj);

	printf("######################################################################################\n");
	printf("\n\n");
	


	return 0;
}