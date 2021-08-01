/*
 *
 *
 *	2.	Write a C Program, which takes three input seperately and print the input value after all values are entered.
 *
 *	{ IP1 is int, IP2 is char, IP3 is float}
 *
 *	Output Of Program:
 *	ip1 = M
 *	ip2 = 10
 *	ip3 = 19.9726
 *
 *	op1 = M
 *	op2 = 10
 *	op3 = 19.9726
 *
 */


#include<stdio.h>
int main(void)
{
    char ip1;
    int ip2;
    float ip3;

    scanf("%c", &ip1);
    printf("ip1 = %c\n", ip1);
    
    scanf("%d", &ip2);
    printf("ip2 = %d\n", ip2);
  
    scanf("%f", &ip3);
    printf("ip3 = %f\n", ip3);

    printf("op1 = %c\n\n", ip1);
    printf("op2 = %d\n\n", ip2);
    printf("op3 = %f\n\n", ip3);

    return 0;
}