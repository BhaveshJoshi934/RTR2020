/*
 *
 *
 *	9.	Write a C Program, which takes three input in a SINGLE LINE and print as following.
 *
 *
 *	Input/Output Constraints:
 *
 *	Input:	M
 *		10
 *		21.110
 *
 *	Output:	M = 10 * 21.110
 *		M = 211.1;
 *
 */

#include<stdio.h>
int main(void)
{
    char cNum_bdj;
    int iNum_bdj;
    float fNum_bdj;

    scanf("%c %d %f", &cNum_bdj, &iNum_bdj, &fNum_bdj);

    printf("\n\n");
    printf("%c = %d * %f\n\n", cNum_bdj, iNum_bdj, fNum_bdj);
    printf("%c = %f", cNum_bdj, iNum_bdj * fNum_bdj);

    return 0;
}