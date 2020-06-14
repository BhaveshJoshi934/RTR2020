#include<stdio.h>

struct MyPoint
{
	int x_bdj;
	int y_bdj;
}A_bdj, B_bdj, C_bdj, D_bdj, E_bdj;

int main(void)
{
	printf("Enter for A: x and y\n");
	scanf("%d %d", &A_bdj.x_bdj,& A_bdj.y_bdj);

	printf("Enter for B: x and y\n");
	scanf("%d %d", &B_bdj.x_bdj, &B_bdj.y_bdj);

	printf("Enter for C: x and y\n");
	scanf("%d %d", &C_bdj.x_bdj, &C_bdj.y_bdj);

	printf("Enter for D: x and y\n");
	scanf("%d %d", &D_bdj.x_bdj, &D_bdj.y_bdj);

	printf("Enter for E: x and y\n");
	scanf("%d %d", &E_bdj.x_bdj, &E_bdj.y_bdj);


	printf("Point For A are: x = %d and y = %d\n", A_bdj.x_bdj, A_bdj.y_bdj);
	printf("Point For B are: x = %d and y = %d\n", B_bdj.x_bdj, B_bdj.y_bdj);
	printf("Point For C are: x = %d and y = %d\n", C_bdj.x_bdj, C_bdj.y_bdj);
	printf("Point For D are: x = %d and y = %d\n", D_bdj.x_bdj, D_bdj.y_bdj);
	printf("Point For E are: x = %d and y = %d\n", E_bdj.x_bdj, E_bdj.y_bdj);


	return 0;
}