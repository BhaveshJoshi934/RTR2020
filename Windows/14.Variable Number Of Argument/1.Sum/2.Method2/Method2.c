#include<stdio.h>
#include<stdarg.h>

int main(void)
{
	int CalculateSum_bdj(int, ...);
	int ans_bdj;
	printf("\n\n");

	ans_bdj = CalculateSum_bdj(5, 10, 20, 30, 40, 50);
	printf("Answer = %d\n", ans_bdj);

	ans_bdj = CalculateSum_bdj(10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	printf("Answer = %d\n", ans_bdj);

	ans_bdj = CalculateSum_bdj(0);
	printf("Answer = %d\n", ans_bdj);

	return 0;
}

int CalculateSum_bdj(int num, ...)
{
	int va_CalculateSum_bdj(int, va_list);

	int sum = 0;
	va_list numbers_list;

	va_start(numbers_list, num);

	sum = va_CalculateSum_bdj(num, numbers_list);

	va_end(numbers_list);
	return sum;
}

int va_CalculateSum_bdj(int num, va_list list)
{
	int n;
	int sum_total = 0;

	while (num)
	{
		n = va_arg(list, int);
		sum_total = sum_total + n;
		num--;
	}
	return sum_total;
}