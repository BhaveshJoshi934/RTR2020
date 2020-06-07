#include<stdio.h>
int main(void)
{
	int iArrOne[10];
	int iArrTwo[10];

	iArrOne[0] = 3;
	iArrOne[1] = 53;
	iArrOne[2] = 73;
	iArrOne[3] = 99;
	iArrOne[4] = 54;
	iArrOne[5] = 24;
	iArrOne[6] = 1;
	iArrOne[7] = 18;
	iArrOne[8] = 47;
	iArrOne[9] = 81;

	printf("\n\n");
	printf("Piece Meal Initialization of iArrOne[] is as below:\n\n");
	printf("Oth index at array of iArrOne[]      = %d\n", iArrOne[0]);
	printf("1st index at array of iArrOne[]      = %d\n", iArrOne[1]);
	printf("2nd index at array of iArrOne[]      = %d\n", iArrOne[2]);
	printf("3rd index at array of iArrOne[]      = %d\n", iArrOne[3]);
	printf("4th index at array of iArrOne[]      = %d\n", iArrOne[4]);
	printf("5th index at array of iArrOne[]      = %d\n", iArrOne[5]);
	printf("6th index at array of iArrOne[]      = %d\n", iArrOne[6]);
	printf("7th index at array of iArrOne[]      = %d\n", iArrOne[7]);
	printf("8th index at array of iArrOne[]      = %d\n", iArrOne[8]);
	printf("9th index at array of iArrOne[]      = %d\n", iArrOne[9]);

	printf("\n\n");
	printf("Enter 1st element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[0]);
	printf("Enter 2nd element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[1]);
	printf("Enter 3rd element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[2]);
	printf("Enter 4th element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[3]);
	printf("Enter 5th element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[4]);
	printf("Enter 6th element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[5]);
	printf("Enter 7th element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[6]);
	printf("Enter 8th element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[7]);
	printf("Enter 9th element of iArrTwo[]  :\n");
	scanf("%d", &iArrTwo[8]);
	printf("Enter 10th element of iArrTwo[] :\n");
	scanf("%d", &iArrTwo[9]);

	printf("Piece Meal Initialization of iArrTwo[] is as below:\n\n");
	printf("Oth index at array of iArrTwo[]      = %d\n", iArrTwo[0]);
	printf("1st index at array of iArrTwo[]      = %d\n", iArrTwo[1]);
	printf("2nd index at array of iArrTwo[]      = %d\n", iArrTwo[2]);
	printf("3rd index at array of iArrTwo[]      = %d\n", iArrTwo[3]);
	printf("4th index at array of iArrTwo[]      = %d\n", iArrTwo[4]);
	printf("5th index at array of iArrTwo[]      = %d\n", iArrTwo[5]);
	printf("6th index at array of iArrTwo[]      = %d\n", iArrTwo[6]);
	printf("7th index at array of iArrTwo[]      = %d\n", iArrTwo[7]);
	printf("8th index at array of iArrTwo[]      = %d\n", iArrTwo[8]);
	printf("9th index at array of iArrTwo[]      = %d\n", iArrTwo[9]);

	return 0;
}