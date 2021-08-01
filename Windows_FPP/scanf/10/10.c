
/*
 *
 *
 *	10.	Write a C Program, Take Initials of your name, age, salary, phone number and print them.
 *
 *	Input/Output Constraints.
 *	Input:
 *		M
 *		S
 *		T
 *		23
 *		111111.1111
 *		12345678890
 *
 *	Output:
 *		M.S.T, 23, 111111.1111, 1234567890
 *
 */

#include <stdio.h>

int main(void)
{
    char f_bdj, m_bdj, l_bdj;
    int age_bdj;
    float salary_bdj;
    long int phn_bdj= 0;

    scanf(" %c %c %c %d %f %ld", &f_bdj, &m_bdj, &l_bdj, &age_bdj, &salary_bdj, &phn_bdj);

    printf("\n");
    printf(" %c.%c.%c, %d, %f, %ld", f_bdj, m_bdj, l_bdj, age_bdj, salary_bdj, phn_bdj);
    printf("\n");
}