/*
 *
 *
 *	1.	Write a C Program, which takes two input ( IP1 is int and IP2 is float ) seperately and print the input value immediately.
 *
 *	Output Of Program:
 *	ip = 10
 *	op = 10
 *
 *	ip = 26.1997
 *	op = 26.1997
 *
 */


#include<stdio.h>
int main(void)
{
    int i_bdj;
    float f_bdj;

    scanf("%d", &i_bdj);
    printf("ip = %d\n", i_bdj);
    printf("op = %d\n\n", i_bdj);

    scanf("%f", &f_bdj);
    printf("ip = %f\n", f_bdj);
    printf("op = %f\n\n", f_bdj);

    return 0;
}