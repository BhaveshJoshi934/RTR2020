
/*
 *
 *
 *	6.	Write a C Program, which takes 6 input space separated in a SINGLE LINE and print the input value after the inputs.
 *
 *	{ IP1 is int, IP2 is char, IP3 is float, IP4 is double, IP5 is long int, IP6 in long double}
 *
 *	Input/Output Constrainst:
 *
 *	Input:
 *	M
 *	10
 *	19.9726
 *	2610.1997171065
 *	2345678901234
 *	12312312313123.12123123312132
 *
 *	Output:
 *	op1 = M
 * 	op2 = 10
 *	op3 = 19.9726
 *	op4 = 2610.1997171065
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
    long int liNum_bdj;
    long double ldNum_bdj;

    scanf("%c%d%f%lf%ld%LE", &cNum_bdj, &iNum_bdj, &fNum_bdj, &dNum_bdj, &liNum_bdj, &ldNum_bdj);

    printf("\n");
    printf("%c\n", cNum_bdj);
    printf("%d\n", iNum_bdj);
    printf("%f\n", fNum_bdj);
    printf("%lf\n", dNum_bdj);
    printf("%ld\n", liNum_bdj);
    printf("%LE\n", ldNum_bdj);

    return 0;
}