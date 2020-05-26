#include <stdio.h>
#define MY_PI_bdj 3.1415926535897932
#define AMC_STRING_bdj "AstroMediComp RTR 2020-2021"

enum
{
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY
};

enum
{
	JANUARY = 1,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER
};

enum Numbers_bdj
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE = 5,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN
};

enum boolean_bdj
{
	TRUE = 1,
	FALSE
};

int main(void)
{
	const double epsilon_bdj = 0.000001;

	//code
	printf("\n\n");
	printf("Local constant value = %lf\n\n", epsilon_bdj);

	printf("SUNDAY is DAY Number = %d\n", SUNDAY);
	printf("MONDAY is DAY Number = %d\n", MONDAY);
	printf("TUESDAY is DAY Number = %d\n", TUESDAY);
	printf("WEDNESDAY is DAY Number = %d\n", WEDNESDAY);
	printf("THURSDAY is DAY Number = %d\n", THURSDAY);
	printf("FRIDAY is DAY Number = %d\n", FRIDAY);
	printf("SATURDAY is DAY Number = %d\n\n", SATURDAY);

	printf("ONE is Enum Number = %d\n", ONE);
	printf("TWO is Enum Number = %d\n", TWO);
	printf("THREE is Enum Number = %d\n", THREE);
	printf("FOUR is Enum Number = %d\n", FOUR);
	printf("FIVE is Enum Number = %d\n", FIVE);
	printf("SIX is Enum Number = %d\n", SIX);
	printf("SEVEN is Enum Number = %d\n", SEVEN);
	printf("EIGHT is Enum Number = %d\n", EIGHT);
	printf("NINE is Enum Number = %d\n", NINE);
	printf("TEN is Enum Number = %d\n\n", TEN);

	printf("JANUARY is month Number = %d\n", JANUARY);
	printf("FEBRUARY is month Number = %d\n", FEBRUARY);
	printf("MARCH is month Number = %d\n", MARCH);
	printf("APRIL is month Number = %d\n", APRIL);
	printf("MAY is month Number = %d\n", MAY);
	printf("JUNE is month Number = %d\n", JUNE);
	printf("JULY is month Number = %d\n", JULY);
	printf("AUGUST is month Number = %d\n", AUGUST);
	printf("SEPTEMBER is month Number = %d\n", SEPTEMBER);
	printf("OCTOBER is month Number = %d\n", OCTOBER);
	printf("NOVEMBER is month Number = %d\n", NOVEMBER);
	printf("DECEMBER is month Number = %d\n\n", DECEMBER);

	printf("Value of TRUE = %d\n", TRUE);
	printf("Value of FALSE = %d\n", FALSE);

	printf("Value of MY_PI_bdj = %.101f\n\n", MY_PI_bdj);
	printf("Area of circle = %f\n\n", (MY_PI_bdj * 0.2f * 0.2f));

	printf("\n\n");

	printf("Value of string = %s\n\n", AMC_STRING_bdj);
	printf("\n\n");

	return 0;
}