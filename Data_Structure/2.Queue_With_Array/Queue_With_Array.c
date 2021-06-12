#include<stdio.h>
#include<stdlib.h>

#define NUM_OF_ELEMENTS 10

int Queue_Array[NUM_OF_ELEMENTS];
int front,rear = 0;

void Add(int);
void Delete();
void Display();

int main()
{
    int operation;
    int number;
    int i;
    int data;
while(1)
{
    printf("0) Exit \n 1) ADD\n 2) Delete \n 3) Display\n\n");
    printf("Choose operation you want to perform:\n\n");

    scanf("%d",&operation);

    switch(operation)
    {
    case 0:
        exit(0);
        break;

    case 1:
        printf("Enter Number Of Elements You want to add:\n\n");
        scanf("%d",&number);

        for(i = 0 ; i < number ; i++)
        {
            printf("Enter elemet:\n");
            scanf("%d",&data);
            Add(data);
        }
        break;

    case 2:
        Delete();
        break;

    case 3:
        Display();
        break;
    }
}
    return 0;
}

void Add(int d)
{
    if(rear == NUM_OF_ELEMENTS)
    {
        printf("QUEUE IS FULL!!!\n\n");
    }

    Queue_Array[rear] = d;
    rear++;
}

void Delete()
{
    if(front == rear)
    {
        printf("QUEUE IS EMPTY!!!\n\n");
    }

    front++;
}

void Display()
{
    int i;

    if(front == rear)
    {
        printf("QUEUE IS EMPTY!!!\n\n");
    }

    for(i = front ; i < rear ; i++)
    {
        printf("-> %d\t",Queue_Array[i]);
    }
}
