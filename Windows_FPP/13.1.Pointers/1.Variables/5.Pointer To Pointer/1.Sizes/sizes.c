#include<stdio.h>

struct Employee
{
	char name[100];
	int age;
	float salary;
	char sex;
	char marital_status;
};

int main(void)
{
	printf("\n\n");
	printf("Size of data types and thier respective pointer AND pointer to pointer addresses are:\n\n");

	printf("sizes of int,int*,int** are:           %d , %d and %d\n\n", sizeof(int), sizeof(int*), sizeof(int**));

	printf("sizes of float,float*,float** are:     %d , %d and %d\n\n", sizeof(float), sizeof(float*), sizeof(float**));

	printf("sizes of double,double*,double** are:  %d , %d and %d\n\n", sizeof(double), sizeof(double*), sizeof(double**));

	printf("sizes of char,char*,char** are:        %d , %d and %d\n\n", sizeof(char), sizeof(char*), sizeof(char**));

	printf("sizes of struct,struct*,struct** are:  %d , %d and %d\n\n", sizeof(struct Employee), sizeof(struct Employee*), sizeof(struct Employee**));
	
	printf("\n\n");

	return 0;
}