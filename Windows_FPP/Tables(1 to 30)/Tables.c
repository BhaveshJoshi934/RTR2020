#include<stdio.h>
int main(void)
{
    int i,j;
    printf("1 to 30 Tables are : \n");
    for(i = 1 ; i <= 30;  i++ )
    {
        printf("%d's Table is : \n\n",i);
        for(j = 1 ; j <= 10 ; j++)
        {
            printf("%d * %d = %d \n", i , j , i*j);
        }
        printf("\n\n");
    }
	return 0;
}
