/*
 *
 *
 *	7.	Write a C Program, which takes two space seperated in a SINGLE LINE 3 times and print their SUM.
 *
 *	{ IP1 & IP2 are int, IP3 & IP4 are float, IP5 & IP6 are double }
 *
 *	Input/Output Constraints:
 *	10
 *	20
 *	-> 30
 *
 *	26.10
 *	12.44
 *	-> 38.54
 *
 *	1262.1007
 *	2610.0017
 *	->  3872.1024
 *
 */

#include<stdio.h>
int main(void)
{
    int inum1_bdj, inum2_bdj;
    float fnum1_bdj, fnum2_bdj;
    double dnum1_bdj, dnum2_bdj;

    scanf("%d %d", &inum1_bdj, &inum2_bdj);
    printf("%d\n\n", inum1_bdj + inum2_bdj);

    scanf("%f %f", &fnum1_bdj, &fnum2_bdj);
    printf("%f\n\n", fnum1_bdj + fnum2_bdj);

    scanf("%lf %lf", &dnum1_bdj, &dnum2_bdj);
    printf("%lf\n\n", dnum1_bdj + dnum2_bdj);

    return 0;
}