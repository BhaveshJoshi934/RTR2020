#include<stdio.h>
#define MAX_LENGTH_BDJ 512

int main(void)
{
	int My_strlen_bdj(char []);

	int string_length = 0;
	char str_bdj[MAX_LENGTH_BDJ];

	printf("\n\n");
	printf("Enter a string:\n");
	gets_s(str_bdj, MAX_LENGTH_BDJ);

	printf("\n\n");
	printf("Your String: %s\n", str_bdj);

	string_length = My_strlen_bdj(str_bdj);

	return 0;
}

int My_strlen_bdj(char string[])
{
	int k = 0;
	int string_count = 0;
	for (k = 0; string[k] != '\0'; k++)
	{
		if (string[k] == '\0')
		{
			break;
		}
		else
		{
			string_count++;
		}
	}
	return string_count;
}