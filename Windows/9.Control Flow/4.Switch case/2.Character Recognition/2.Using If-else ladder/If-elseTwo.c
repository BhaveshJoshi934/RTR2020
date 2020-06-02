#include<stdio.h>
#include<conio.h>

#define CHAR_ALPHABET_UPPER_CASE_BEGIN_BDJ 65
#define CHAR_ALPHABET_UPPER_CASE_END_BDJ 90

#define CHAR_ALPHABET_LOWER_CASE_BEGIN_BDJ 97
#define CHAR_ALPHABET_LOWER_CASE_END_BDJ 122

#define CHAR_ALPHABET_DIGIT_BEGIN_BDJ 48
#define CHAR_ALPHABET_DIGIT_END_BDJ 57

int main(void)
{
	char ch_bdj;
	int ch_value_bdj;

	printf("\n\n");
	printf("Enter character:\n");
	ch_bdj = getch();
	printf("\n\n");

	if ((ch_bdj == 'A' || ch_bdj == 'a') || (ch_bdj == 'E' || ch_bdj == 'e') || (ch_bdj == 'I' || ch_bdj == 'i') || (ch_bdj == 'O' || ch_bdj == 'o') || (ch_bdj == 'U' || ch_bdj == 'u'))
	{
		printf("Character \'%c\' Entered By You, Is A VOWEL CHARACTER From The English Alphabet !!!\n\n", ch_bdj);
	}
	else
	{
		ch_value_bdj = (int)ch_bdj;

		if ((ch_value_bdj >= CHAR_ALPHABET_UPPER_CASE_BEGIN_BDJ && ch_value_bdj <= CHAR_ALPHABET_UPPER_CASE_END_BDJ) || (ch_value_bdj >= CHAR_ALPHABET_LOWER_CASE_BEGIN_BDJ && ch_value_bdj <= CHAR_ALPHABET_LOWER_CASE_END_BDJ))
		{
			printf("Character \'%c\' Entered By You, Is A CONSONANT CHARACTER From The English Alphabet !!!\n\n", ch_bdj);
		}
		else if (ch_value_bdj >= CHAR_ALPHABET_DIGIT_BEGIN_BDJ && ch_value_bdj <= CHAR_ALPHABET_DIGIT_END_BDJ)
		{
			printf("Character \'%c\' Entered By You, Is A DIGIT CHARACTER !!!\n \n", ch_bdj);
		}
		else
		{
			printf("Character \'%c\' Entered By You, Is A SPECIAL CHARACTER !!!\n \n", ch_bdj);
		}
	}
	return 0;
}