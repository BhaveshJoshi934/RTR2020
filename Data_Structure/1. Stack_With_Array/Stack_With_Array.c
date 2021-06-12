//CODE FOR STACK

//Header Files

#include<stdio.h>
#include<stdlib.h>

//Macros

#define  NUMBER_OF_ELEMENTS    10

//Function Declaration

void Push(int);
void Pop(void);
void Display(void);

//Global Varables

int Stack[NUMBER_OF_ELEMENTS];
int top = 0;

int main(void)
{
    int d,i,ch,n;
    while(1)
    {
            printf("0) EXIT \n 1) Push Element \n 2) Pop Element \n 3) Display Elements \n");
            printf("Please Enter Your Choice:\n");
            scanf("%d",&ch);
            switch(ch)
            {
                case 0:
                    exit(0);
                    break;

                case 1:
                    printf("Enter the Number of Elements You want to Push = \n");
                    scanf("%d",&n);
                    for(i = 0 ; i < n ; i++)
                    {
                        printf("Enter Element = \n");
                        scanf("%d",&d);
                        Push(d);
                    }
                    break;

                case 2:
                    Pop();
                    break;

                case 3:
                    Display();
                    break;

                default:
                    break;
            }
    }

    return(0);
}

void Push(int data)
{
    if(top == NUMBER_OF_ELEMENTS)
    {
        printf("STACK IS FULL\n\n");
    }

    Stack[top] = data;

    top++;
}

void Pop()
{
    if(top == 0)
    {
        printf("STACK IS EMPTY!!!\n\n");
    }

    top -= 1;
}

void Display()
{
    int i;

    if(top == 0)
    {
        printf("STACK IS EMPTY!!!\n\n");
    }

    for(i = 0 ; i < top ; i++)
    {
        printf("Data in Stack : %d\n",Stack[i]);
    }
}
