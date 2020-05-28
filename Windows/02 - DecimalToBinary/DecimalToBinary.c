#include <stdio.h>
int main(void)
{
	//code 
	int num, i;
	int arr[100];
	printf("Enter a Positive Number:\n");
	scanf("%d", &num);
	
	if (num > 0)
	{
		printf("Number is Valid!!!\n");
		printf("Binary Representation of %d is :", num);
		for (i = 0; num > 0; i++)
		{
			arr[i] = num % 2;
			num = num / 2;
		}
	}
	else
	{
		printf("The Number is NOT Valid!!!\n");
	}
	for (i = i - 1; i >= 0; i--)
	{
		printf("%d", arr[i]);
	}
	return 0;
}