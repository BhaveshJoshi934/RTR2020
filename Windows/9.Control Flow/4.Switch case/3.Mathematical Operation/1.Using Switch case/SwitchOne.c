#include<stdio.h>
#include<conio.h>

int main(void)
{
	int a_bdj, b_bdj,result_bdj;
	char option_bdj, option_division_bdj;

	printf("\n\n");
	printf("Enter value A = \n");
	scanf("%d", &a_bdj);

	printf("Enter value B = \n");
	scanf("%d", &b_bdj);

	printf("Enter a character for Operation\n");
	printf("FOR 'A' or 'a' => Addition\n" );
	printf("FOR 'S' or 's' => Subtraction\n" );
	printf("FOR 'M' or 'm' => Multiplication\n" );
	printf("FOR 'D' or 'd' => Division\n\n");

	printf("Enter an option:\n");
	option_bdj = getch();
	printf("\n\n");

	switch (option_bdj)
	{
	case 'A':
	case 'a':
		result_bdj = a_bdj + b_bdj;
		printf("Addition of A = %d and B = %d gives result = %d\n", a_bdj, b_bdj, result_bdj);
		break;

	case 'S':
	case 's':
		if (a_bdj >= b_bdj)
		{
			result_bdj = a_bdj - b_bdj;
			printf("Subtraction of A = %d and B = %d gives result = %d\n", a_bdj, b_bdj, result_bdj);
		}
		else
		{
			result_bdj = b_bdj - a_bdj;
			printf("Subtraction of B = %d and A = %d gives result = %d\n", b_bdj, a_bdj, result_bdj);
		}
		break;

	case 'M':
	case 'm':
		result_bdj = a_bdj * b_bdj;
		printf("Multiplication of A = %d and B = %d gives result = %d\n", a_bdj, b_bdj, result_bdj);
		break;

	case 'D':
	case 'd':
		printf("Enter an option character:\n");
		printf("Enter 'Q' or 'q' or '/' for quotient upon division\n");
		printf("Enter 'R' or 'r' or '%%' for remainder upon division\n");

		printf("Enter an option:\n");
		option_division_bdj = getch();

		printf("\n\n");
		switch (option_division_bdj)
		{
		case 'Q':
		case 'q':
		case '/':
			if (a_bdj >= b_bdj)
			{
				result_bdj = a_bdj / b_bdj;
				printf("Division of A = %d and B = %d gives quotient as a result = %d\n", a_bdj, b_bdj, result_bdj);
			}
			else
			{
				result_bdj = b_bdj / a_bdj;
				printf("Division of B = %d and A = %d gives quotient as a result = %d\n", b_bdj, a_bdj, result_bdj);
			}
			break;

		case 'R':
		case 'r':
		case '%':
			if (a_bdj >= b_bdj)
			{
				result_bdj = a_bdj % b_bdj;
				printf("Division of A = %d and B = %d gives remainder as a result = %d\n", a_bdj, b_bdj, result_bdj);
			}
			else
			{
				result_bdj = b_bdj % a_bdj;
				printf("Division of B = %d and A = %d gives remainder as a result = %d\n", b_bdj, a_bdj, result_bdj);
			}
			break;

		default:
			printf("You have entered invalid %c for division...Please try again\n\n",option_division_bdj);
			break;
		}

		break;

	default:
		printf("You have entered invalid %c for operation...Please try again\n\n", option_bdj);
		break;
	}
	printf("You are out of switch case\n\n");

	return 0;
}