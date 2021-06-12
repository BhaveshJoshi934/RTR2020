#include<stdio.h>
#include<stdlib.h>

#define NUMBER_OF_ELEMENT 5

int Stack_Array[NUMBER_OF_ELEMENT];
int top = 0;

void push(int);
void pop(void);
void display(void);

int main()
{
    int operation;
    int num_of_elements;
    int i;
    int data;
    while(1)
    {
        printf("0) EXIT \t\n1) PUSH \t\n2)POP \t\n4)DISPLAY\n\n");
        printf("Please select the operation you want to perform:\n\n");
        scanf("%d",&operation);
        switch(operation)
        {
        case 0:
            exit(0);
            break;

        case 1:
            printf("Enter the Number Of Element You want to insert :\n");
            scanf("%d",&num_of_elements);
            for(i = 0 ; i < num_of_elements ; i++)
            {
                printf("Entered Element\n");
                scanf("%d",&data);
                push(data);
            }
            break;

        case 2:
            pop();
            break;

        case 3:
            display();
            break;

        default:
            break;
        }
    }
    return 0;
}

void push(int d)
{
    int i;

    if(top == NUMBER_OF_ELEMENT)
    {
        printf("STACK IS FULL!!!!\n\n");
    }

    Stack_Array[top] = d;
    top++;
}

void pop()
{
    if(top == 0)
    {
        printf("STACK IS EMPTY!!!\n\n");
    }
    top -= 1;
}

void display()
{
    int i;
    if(top == 0)
    {
        printf("STACK IS EMPTY!!!\n\n");
    }

    for(i = 0 ; i < top ; i++)
    {
        printf("Data[%d] = %d \n",i,Stack_Array[i]);
    }
}


