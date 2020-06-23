#include<stdio.h>

enum
{
	NEGATIVE = -1,
	ZERO,
	POSITIVE
};

int main(void)
{
	int Diff(int, int, int*);

	int a_bdj, b_bdj,ans,ret;

	printf("Enter A:\n");
	scanf("%d", &a_bdj);

	printf("Enter B:\n");
	scanf("%d", &b_bdj);

	ret = Diff(a_bdj, b_bdj, &ans);

	printf("\n\n");
	printf("Difference of %d and %d = %d\n", a_bdj, b_bdj, ans);

	if (ret == POSITIVE)
		printf("%d And %d are positive\n\n", a_bdj, b_bdj);
	else if (ret == NEGATIVE)
		printf("%d And %d are negative\n\n", a_bdj, b_bdj);
	else
		printf("%d And %d are zero", a_bdj, b_bdj);

	return 0;
}

int Diff(int x, int y, int *ans)
{
	*ans = x - y;
	if (*ans > 0)
		return POSITIVE;
	else if (*ans < 0)
		return NEGATIVE;
	else
		return ZERO;
}