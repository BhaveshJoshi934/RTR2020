
/*
 *
 *
 *	3.	Write a C Program, which takes 5 input seperately and print the input value after all values are entered.
 *
 *	{ IP1 is int, IP2 is char, IP3 is float, IP4 is double, IP5 is long int}
 *
 *	Output Of Program:
 *	ip1 = M
 *	ip2 = 10
 *	ip3 = 19.9726
 *	ip4 = 2610.1997171065
 *	ip5 = 12345678901234
 *
 *	op1 = M
 *	op2 = 10
 *	op3 = 19.9726
 *	op4 = 2610.1997171065
 *	op5 = 12345678901234
 *
 */

#include<stdio.h>
int main(void)
{
    int iNum_bdj = 0;
    float fNum_bdj = 0.0f;
    char cNum_bdj ;
    double dNum_bdj = 0.0;
    long int liNum_bdj = 0;

    scanf("%c", &cNum_bdj);
    scanf("%d", &iNum_bdj);
    scanf("%f", &fNum_bdj);
    scanf("%lf", &dNum_bdj);
    scanf("%ld", &liNum_bdj);

    printf("\n");
    
    printf("%c\n", cNum_bdj);
    printf("%d\n", iNum_bdj);
    printf("%f\n", fNum_bdj);
    printf("%lf\n", dNum_bdj);
    printf("%ld\n", liNum_bdj);

}