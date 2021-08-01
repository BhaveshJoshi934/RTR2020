#include<stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	unsigned int num_bdj;

	void recursive_bdj(unsigned int);

	printf("\n\n");
	printf("Enter any Number:\n");
	scanf("%d", &num_bdj);
	printf("\n\n");

	printf("Output of Recursive Function:\n\n");
	recursive_bdj(num_bdj);

	return 0;
}
void recursive_bdj(unsigned int n)
{
	printf("n = %d\n", n);
	while (n > 0)
	{
		recursive_bdj(n - 1);
	}
}
