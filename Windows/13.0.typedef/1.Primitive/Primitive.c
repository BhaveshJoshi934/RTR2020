#include<stdio.h>

typedef int MY_INT;

int main(void)
{
	MY_INT add(MY_INT, MY_INT);

	typedef int MY_INT;
	typedef float BDJ_FLAOT;
	typedef char CHAR;
	typedef double MY_DOUBLE;

	typedef unsigned int UINT;
	typedef UINT HANDEL;
	typedef HANDEL HWND;
	typedef HWND HINSTANCE;

	MY_INT a_bdj = 10, i_bdj;
	MY_INT array[] = { 1,2,3,4,5,6,7,8,9,10 };

	BDJ_FLAOT f_bdj = 300.14f;
	const BDJ_FLAOT pi_bdj = 3.14f;

	CHAR ch_bdj = '*';
	CHAR Array_bdj1[] = "Hello";
	CHAR Array_bdj2[] = "Hello Bhavesh Joshi!!!";

	MY_DOUBLE d_bdj = 123.3456;

	UINT uint_bdj = 3456;
	HANDEL handel_bdj = 987;
	HWND hwnd_bdj = 9876;
	HINSTANCE hinstance_bdj = 14466;

	printf("\n\n");
	printf("MY_INT = %d\n", a_bdj);

	printf("\n\n");

	for (i_bdj = 0; i_bdj < sizeof(array)/sizeof(int); i_bdj++)
	{
		printf("array[%d] = %d \n",i_bdj, array[i_bdj]);
	}
	printf("\n\n");

	printf("BDJ_FLOAT = %f\n", f_bdj);
	printf("BDJ_FLOAT pi = %f\n", pi_bdj);

	printf("\n\n");

	printf("CHAR = %c\n", ch_bdj);
	
	printf("CHAR Array is : %s\n", Array_bdj1);

	printf("\n\n");

	printf("MY_DOUBLE = %lf\n\n", d_bdj);

	printf("UINT = %u\n", uint_bdj);
	printf("HANDEL= %u\n", handel_bdj);
	printf("HWND = %u\n", hwnd_bdj);
	printf("HINSTANCE = %u\n\n", hinstance_bdj);

	MY_INT  x = 90;
	MY_INT  y = 40;
	MY_INT ret;
	 
	ret = add(x, y);
	printf("ret = %d\n", ret);

	return 0;
}
MY_INT add(MY_INT a, MY_INT b)
{
	return a + b;
}