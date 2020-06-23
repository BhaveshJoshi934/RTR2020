#include<stdio.h>

struct Employee
{
	char name_bdj[100];
	int age_bdj;
	float salary_bdj;
	char sex_bdj;
};

int main(void)
{
	printf("Sizes of all data types along with their Pointer Sizes are listed Below:\n\n");

	printf("Integer-size : %d  \t Interger-Pointer-size : %d\n", sizeof(int), sizeof(int*));
	printf("Chracter-size: %d  \t Chracter-Pointer-size : %d\n", sizeof(char), sizeof(char*));
	printf("Float-size : %d    \t Float-Pointer-size      : %d\n", sizeof(float), sizeof(float*));
	printf("Double-size : %d   \t Double-Pointer-size    : %d\n", sizeof(double), sizeof(double*));
	printf("Integer-size : %d  \t Interger-Pointer-size : %d\n", sizeof(int), sizeof(int*));
	printf("Struct-size : %d   \t Struct-Pointer-size    : %d\n", sizeof(struct Employee), sizeof(struct Employee*));

	return 0;
}