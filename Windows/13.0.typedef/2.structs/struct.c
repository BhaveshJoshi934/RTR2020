#include<stdio.h>

#define MAX_NAME_LENGTH 100

struct Employee
{
	char name[MAX_NAME_LENGTH];
	unsigned int age;
	char gender;
	double salary;
};

struct MyData
{
	int i;
	float f;
	double d;
	char c;
};

int main(void)
{
	typedef struct Employee MY_EMMPLOYEE_TYPE;
	typedef struct MyData MY_DATA_TYPE;

	struct Employee emp = {"Funny",25,'F',1000.00};
	MY_EMMPLOYEE_TYPE emp_typedef = {"Bunny",23,'B',2000.00};

	struct MyData md = { 1,3.24f,78.578,'F' };
	MY_DATA_TYPE md_typedef;

	md_typedef.i = 2;
	md_typedef.f = 54.8f;
	md_typedef.d = 578.076;
	md_typedef.c = 'L';

	printf("\n\n");
	printf("Struct Employee:\n");
	printf("Name = %s\n", emp.name);
	printf("Age = %d\n", emp.age);
	printf("Gender = %c\n", emp.gender);
	printf("Salary = %lf\n", emp.salary);

	printf("\n\n");
	printf("My Struct Employee:\n");
	printf("Name = %s\n", emp_typedef.name);
	printf("Age = %d\n", emp_typedef.age);
	printf("Gender = %c\n", emp_typedef.gender);
	printf("Salary = %lf\n\n", emp_typedef.salary);

	printf("Struct MyData:\n");
	printf("md.i = %d\n", md.i);
	printf("md.f = %f\n", md.f);
	printf("md.d = %lf\n", md.d);
	printf("md.c = %c\n\n", md.c);

	printf("Struct My_Data_Type:\n");
	printf("md_typedef.i = %d\n", md_typedef.i);
	printf("md_typedef.f = %f\n", md_typedef.f);
	printf("md_typedef.d = %lf\n", md_typedef.d);
	printf("md_typedef.c = %c\n", md_typedef.c);

	printf("\n\n");
	return 0;
}