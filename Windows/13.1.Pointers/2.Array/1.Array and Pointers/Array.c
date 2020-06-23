#include<stdio.h>

int main(void)
{
	int iArr_bdj[] = { 1,2,3,4,5 };
	float fArr_bdj[] = { 1.1,2.2,3.3,4.4,5.5 };
	double dArr_bdj[] = { 1.1111,2.2222,3.3333,4.44444,5.55555 };
	char cArr_bdj[] = { 'B','H','A','V','E','S','H','\0' };

	printf("\n\n");
	printf("Integer Array Elements and their Addresses are as:\n\n");
	printf("iArr_bdj[0] :   %d\t *iArr_bdj :     %p\n", *(iArr_bdj + 0), (iArr_bdj + 0));
	printf("iArr_bdj[1] :   %d\t *iArr_bdj :     %p\n", *(iArr_bdj + 1), (iArr_bdj + 1));
	printf("iArr_bdj[2] :   %d\t *iArr_bdj :     %p\n", *(iArr_bdj + 2), (iArr_bdj + 2));
	printf("iArr_bdj[3] :   %d\t *iArr_bdj :     %p\n", *(iArr_bdj + 3), (iArr_bdj + 3));
	printf("iArr_bdj[4] :   %d\t *iArr_bdj :     %p\n", *(iArr_bdj + 4), (iArr_bdj + 4));

	printf("\n\n");
	printf("Flaot Array Elements and their Addresses are as:\n\n");
	printf("fArr_bdj[0] :   %f\t *fArr_bdj :     %p\n", *(fArr_bdj + 0), (fArr_bdj + 0));
	printf("fArr_bdj[1] :   %f\t *fArr_bdj :     %p\n", *(fArr_bdj + 1), (fArr_bdj + 1));
	printf("fArr_bdj[2] :   %f\t *fArr_bdj :     %p\n", *(fArr_bdj + 2), (fArr_bdj + 2));
	printf("fArr_bdj[3] :   %f\t *fArr_bdj :     %p\n", *(fArr_bdj + 3), (fArr_bdj + 3));
	printf("fArr_bdj[4] :   %f\t *fArr_bdj :     %p\n", *(fArr_bdj + 4), (fArr_bdj + 4));

	printf("\n\n");
	printf("Double Array Elements and their Addresses are as:\n\n");
	printf("dArr_bdj[0] :   %lf\t *dArr_bdj :     %p\n", *(dArr_bdj + 0), (dArr_bdj + 0));
	printf("dArr_bdj[1] :   %lf\t *dArr_bdj :     %p\n", *(dArr_bdj + 1), (dArr_bdj + 1));
	printf("dArr_bdj[2] :   %lf\t *dArr_bdj :     %p\n", *(dArr_bdj + 2), (dArr_bdj + 2));
	printf("dArr_bdj[3] :   %lf\t *dArr_bdj :     %p\n", *(dArr_bdj + 3), (dArr_bdj + 3));
	printf("dArr_bdj[4] :   %lf\t *dArr_bdj :     %p\n", *(dArr_bdj + 4), (dArr_bdj + 4));

	printf("\n\n");
	printf("Char Array Elements and their Addresses are as:\n\n");
	printf("cArr_bdj[0] :   %c\t *cArr_bdj :     %p\n", *(cArr_bdj + 0), (cArr_bdj + 0));
	printf("cArr_bdj[1] :   %c\t *cArr_bdj :     %p\n", *(cArr_bdj + 1), (cArr_bdj + 1));
	printf("cArr_bdj[2] :   %c\t *cArr_bdj :     %p\n", *(cArr_bdj + 2), (cArr_bdj + 2));
	printf("cArr_bdj[3] :   %c\t *cArr_bdj :     %p\n", *(cArr_bdj + 3), (cArr_bdj + 3));
	printf("cArr_bdj[4] :   %c\t *cArr_bdj :     %p\n", *(cArr_bdj + 4), (cArr_bdj + 4));
	printf("cArr_bdj[5] :   %c\t *cArr_bdj :     %p\n", *(cArr_bdj + 5), (cArr_bdj + 5));
	printf("cArr_bdj[5] :   %c\t *cArr_bdj :     %p\n", *(cArr_bdj + 6), (cArr_bdj + 6));
	printf("cArr_bdj[7] :   %c\t *cArr_bdj :     %p\n", *(cArr_bdj + 7), (cArr_bdj + 7));



	return 0;
}