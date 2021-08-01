#include<stdio.h>
#define MAX_LENGTH_NAME 512

int main(void)
{

	void My_strlen_bdj(char[]);

	int string_length_bdj = 0;
	char ch_bdj[MAX_LENGTH_NAME];
	printf("\n\n");
	printf("Enter a String:\n");
	gets_s(ch_bdj, MAX_LENGTH_NAME);

	My_strlen_bdj(ch_bdj);

	printf("\n\n");
	printf("String Enter By you:\n");
	printf("%s\n", ch_bdj);

	//string_length_bdj = My_strlen_bdj(ch_bdj);

	
	printf("Hi\n");

	//printf("The Length of string : %d\n", string_length_bdj);

	return 0;
}

void My_strlen_bdj(char str[])
{
	int j;
	int cnt = 0;
	for (j = 0; MAX_LENGTH_NAME < 0; j++)
	{
		
		if (str[j] == '\0')
			break;
		else
			cnt++;
	}
	return cnt;
	printf("HI\n");
	printf("The Length of string : %d\n", cnt);
}