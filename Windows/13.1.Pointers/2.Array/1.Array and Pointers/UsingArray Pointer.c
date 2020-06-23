#include<stdio.h>
int main(void)
{
	int iArr_bdj[] = { 1,2,3,4,5,6,7,8,9,10 };
	int ptr_iArr_bdj = NULL;

	printf("\n\n");
	printf("USING ARRAY NAME AS NAME:\n\n");
	printf("iArr_bdj[0] =     %d \t   At Address =     %p\n", iArr_bdj[0], &iArr_bdj[0]);
	printf("iArr_bdj[1] =     %d \t   At Address =     %p\n", iArr_bdj[1], &iArr_bdj[1]);
	printf("iArr_bdj[2] =     %d \t   At Address =     %p\n", iArr_bdj[2], &iArr_bdj[2]);
	printf("iArr_bdj[3] =     %d \t   At Address =     %p\n", iArr_bdj[3], &iArr_bdj[3]);
	printf("iArr_bdj[4] =     %d \t   At Address =     %p\n", iArr_bdj[4], &iArr_bdj[4]);
	printf("iArr_bdj[5] =     %d \t   At Address =     %p\n", iArr_bdj[5], &iArr_bdj[5]);
	printf("iArr_bdj[6] =     %d \t   At Address =     %p\n", iArr_bdj[6], &iArr_bdj[6]);
	printf("iArr_bdj[7] =     %d \t   At Address =     %p\n", iArr_bdj[7], &iArr_bdj[7]);
	printf("iArr_bdj[8] =     %d \t   At Address =     %p\n", iArr_bdj[8], &iArr_bdj[8]);
	printf("iArr_bdj[9] =     %d \t   At Address =     %p\n", iArr_bdj[9], &iArr_bdj[9]);


	ptr_iArr_bdj = iArr_bdj;

	printf("USING POINTER VALUE:\n\n");
	printf("iArr_bdj[0] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 0), (ptr_iArr_bdj + 0));
	printf("iArr_bdj[1] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 1), (ptr_iArr_bdj + 1));
	printf("iArr_bdj[2] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 2), (ptr_iArr_bdj + 2));
	printf("iArr_bdj[3] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 3), (ptr_iArr_bdj + 3));
	printf("iArr_bdj[4] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 4), (ptr_iArr_bdj + 4));
	printf("iArr_bdj[5] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 5), (ptr_iArr_bdj + 5));
	printf("iArr_bdj[6] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 6), (ptr_iArr_bdj + 6));
	printf("iArr_bdj[7] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 7), (ptr_iArr_bdj + 7));
	printf("iArr_bdj[8] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 8), (ptr_iArr_bdj + 8));
	printf("iArr_bdj[9] =     %d \t   At Address =     %p\n", *(ptr_iArr_bdj + 9), (ptr_iArr_bdj + 9));
	


	return 0;
}