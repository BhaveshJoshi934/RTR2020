/*
 *
 *
 *	8.	Write a C Program, which takes five space seperated in a SINGLE LINE and calculates its average.
 *
 *
 *	Input/Output Constraints:
 *	1
 *	2
 *	3
 *	4
 *	6
 *
 *	-> 3.200000
 *
 */

#include<stdio.h>
int main(void)
{
    int num1, num2, num3, num4, num5;
    scanf("%d%d%d%d%d", &num1, &num2, &num3, &num4, &num5);

    printf("\n\n");
    printf("%d\n", num1);
    printf("%d\n", num2);
    printf("%d\n", num3);
    printf("%d\n", num4);
    printf("%d\n", num5);

    printf("Average: %d\n", (num1 + num2 + num3 + num4 + num5) / 5);

    return 0;
}