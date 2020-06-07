#include<stdio.h>
int main(void)
{
	char ch_1[] = { 'H','I','\0' };
	char ch_2[] = { 'H','E','L','L','O','\0' };
	char ch_3[] = { 'H','E','Y','\0' };
	char ch_4[] = "It's";
	char ch_5[] = "Real Time Rendering Batch";

	char Without_Null[] = { 'N','o','N','u','l','l' };

	printf("\n\n");
	printf("Size of ch_1[]         = %lu\n", sizeof(ch_1));
	printf("Size of ch_2[]         = %lu\n", sizeof(ch_2));
	printf("Size of ch_3[]         = %lu\n", sizeof(ch_3));
	printf("Size of ch_4[]         = %lu\n", sizeof(ch_4));
	printf("Size of ch_5[]         = %lu\n", sizeof(ch_5));

	printf("\n\n");
	printf("Strings are :\n");
	printf("ch_1     : %s\n\n", ch_1);
	printf("ch_2     : %s\n\n", ch_2);
	printf("ch_3     : %s\n\n", ch_3);
	printf("ch_4     : %s\n\n", ch_4);
	printf("ch_5     : %s\n\n", ch_5);

	printf("\n\n");
	printf("Size of Array Without Null = %lu\n", sizeof(Without_Null));
	printf("Without Null               = %s\n\n", Without_Null);

	return 0;
}