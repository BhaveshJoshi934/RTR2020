/*
 *
 *
 *	5.	Write a C Program, which takes three input space seperated in a SINGLE LINE and print the input value after all values are entered.
 *
 *	{ IP1 is int, IP2 is char, IP3 is float}
 *
 *	Input and Output Constraints:
 *	Inputs:
 *	M 10 19.9726
 *
 *	Outputs:
 *	op1 = M
 *	op2 = 10
 *	op3 = 19.9726
 *
 */

#include<stdio.h>
int main(void)
{
    char cNum_bdj;
    int iNum_bdj;
    float fNum_bdj;

    scanf("%c %d  %f", &cNum_bdj, &iNum_bdj, &fNum_bdj);
    printf("\n");

    printf("%c\n", cNum_bdj);
    printf("%d\n", iNum_bdj);
    printf("%f\n", fNum_bdj);

}