#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	void MyAlloc(int** ptr, unsigned int numberOfElements);

	int* piArray = NULL;
	unsigned int num_elements; 
	int i;

	printf("\n\n"); 
	printf("Enter the Number of array...\n\n");
	scanf("%u", &num_elements);

	printf("\n\n"); 
	MyAlloc(&piArray, num_elements);
	printf("Enter %u Elements To Fill Up Your Array : \n\n", num_elements);

	for (i = 0; i < num_elements; i++) 
		scanf("%d", &piArray[i]);

	printf("\n\n"); 
	printf("The %u Elements Entered By You In an Array : \n\n", num_elements);

	for (i = 0; i < num_elements; i++) 
		printf("%u\n", piArray[i]);

	printf("\n\n");
	if (piArray)
	{ 
		free(piArray); 
		piArray = NULL; 
		printf("Memory Allocated !!!\n\n");
	}

	return 0;
}

void MyAlloc(int** ptr, unsigned int numberOfElements) 
{ 
	*ptr = (int *)malloc(numberOfElements * sizeof(int)); 
	if (*ptr == NULL)
	{  
		printf("Could Not Allocate Memory !!! Exitting Now ...\n\n");  
		exit(0); 
	}
	printf("MyAlloc() Has Successfully Allocated %lu Bytes For an Array !!!\n \n", (numberOfElements * sizeof(int)));
}