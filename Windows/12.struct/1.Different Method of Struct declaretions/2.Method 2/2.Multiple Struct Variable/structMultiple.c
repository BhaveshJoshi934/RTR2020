#include<stdio.h>

struct MyPoint
{
	int x_bdj;
	int y_bdj;
};

struct MyPoint A_bdj, B_bdj, C_bdj, D_bdj, E_bdj;

int main(void)
{
	A_bdj.x_bdj = 1;
	A_bdj.y_bdj = 2;

	B_bdj.x_bdj = 3;
	B_bdj.y_bdj = 4;

	C_bdj.x_bdj = 5;
	C_bdj.y_bdj = 6;

	D_bdj.x_bdj = 7;
	D_bdj.y_bdj = 8;

	E_bdj.x_bdj = 9;
	E_bdj.y_bdj = 10;

	printf("Point For A are: x = %d and y = %d\n", A_bdj.x_bdj, A_bdj.y_bdj);
	printf("Point For B are: x = %d and y = %d\n", B_bdj.x_bdj, B_bdj.y_bdj);
	printf("Point For C are: x = %d and y = %d\n", C_bdj.x_bdj, C_bdj.y_bdj);
	printf("Point For D are: x = %d and y = %d\n", D_bdj.x_bdj, D_bdj.y_bdj);
	printf("Point For E are: x = %d and y = %d\n", E_bdj.x_bdj, E_bdj.y_bdj);


	return 0;
}