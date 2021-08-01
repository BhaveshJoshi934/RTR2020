#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	FILE* pFile = NULL;
	if (fopen_s(&pFile, "BhaveshLog.txt", "w"))
	{
		printf("Can't Open the file\n");
		exit(0);
	}
	fprintf(pFile, "India is My Country.");
	fclose(pFile);
	pFile = NULL;
	return 0;
}