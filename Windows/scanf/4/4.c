/*
 *
 *
 *	4.	Write a C Program, which takes 6 input seperately and print the input value immediately after 2 inputs.
 *
 *	{ IP1 is int, IP2 is char, IP3 is float, IP4 is double, IP5 is long int, IP6 in long double}
 *
 *	Output Of Program:
 *	ip1 = M
 *	ip2 = 10
 *	op1 = M
 *	op2 = 10
 *
 *	ip3 = 19.9726
 *	ip4 = 2610.1997171065
 *	op3 = 19.9726
 *	op4 = 2610.1997171065
 *
 *	ip5 = 12345678901234
 *	ip6 = 12312312313123.12123123312132
 *	op5 = 12345678901234
 *	op6 = 12312312313123.12123123312132
 *
 */

#include<stdio.h>
int main(void)
{
    char cNum_bdj;
    int iNum_bdj;
    float fNum_bdj;
    double dNum_bdj;
    long int lNum_bdj;
    long double ldNum_bdj;

    scanf("%c", &cNum_bdj);
    scanf("%d", &iNum_bdj);

    printf("%c\n", cNum_bdj);
    printf("%d\n\n", iNum_bdj);

    scanf("%f", &fNum_bdj);
    scanf("%lf", &dNum_bdj);

    printf("%f\n", fNum_bdj);
    printf("%lf\n\n", dNum_bdj);

    scanf("%ld", &lNum_bdj);
    scanf("%LE", &ldNum_bdj);

    printf("%ld\n", lNum_bdj);
    printf("%LE\n\n", ldNum_bdj);

}