#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 15
#define MIN 100
#define MAX 1000

/*
	Program : Expression Calculator
	Author  : R.Sundar Rajan
	Version : 1.0
*/

typedef struct node
{
	float number;
	char symbol;
	struct node *next;
}Node;

//Functions

//Essentials 

int validator(char *); //Check the correctness of the user's input
int Priority(char );//To return the priority value to the operands [^,/,*,+,-]
float Pow(float ,int );//For some reason the pow() function from math.h isn't properly linked while compiling on my linux

//Conversion Operations

Node* Array_Linked(char *);//It converts array values into linked list
Node* Infix_Postfix(Node*,Node *);//It converts the linklist into the postfix version of linked list for evaluation

//Linked-list Operations

Node* Push(Node *,char ,float );//for inserting the numbers or operators inside the linked list


//Postfix stack Operations

void push_char(char *,int *,char ); //used to insert operator into the stack (This function helps Infix_Postfix() to do the work) 
char pop_char(char *,int *); //used to take out the operator from the stack (This function helps Infix_Postfix() to do the work) 

//Evaluation Operations

float Evaluation(Node* );//Evaluate the postfix and gives final answer
void push_float(float *,float,int *);//used to insert numbers into float stack (This function helps Evaluation() to do the work)
float pop_float(float *,int *);//used to take out the numbers from the float stack (This function helps Evaluation() to do the work)


int main()
{
	char *input = (char *)calloc(MAX,sizeof(char));
	int ch,valid;
	Node *infix = NULL;
	Node *postfix = NULL;
	do
	{
		printf("\nSelect number for the Operations\n");
		printf("\n+-----------------------------+");
		printf("\n|   1.Calculator              |");
		printf("\n|   2.Exit                    |");
		printf("\n+-----------------------------+\n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1 :
				printf("\nEnter the Mathematical Expression : \n");
				scanf("%s",input);
				valid = validator(input);//Validate the expression
				if(valid == 1)
				{
					infix = Array_Linked(input);//Converting input to linked list
					postfix = Infix_Postfix(infix, postfix);//Converting infix to postfix
					printf("Answer : %.3f",Evaluation(postfix));//Evaluating and Displaying the answer
					infix = NULL;
					postfix = NULL;
				}
				else
				{
					printf("\nInvalid\n");
				}
				break;
			case 2 : 
				printf("\nThanks for using Calculator\n");
				free(infix); 
				free(postfix);
				free(input); 
				break;
			default :
				printf("\nEnter the Valid Option\n");
				break;
		}

	}while(ch != 2);
	return 0;
}

/*
Input  : Node pointer
Return : float

Description :  
    It Push the numbers one by one from postfix to float stack until it reached an operator 
    Then it pops two numbers from the float stack in the order of op2 and op1 and do the operations
    and push the resultant in the float stack again until it reaches the end of the linked list 
    Then it returns the final floating number
*/
float Evaluation(Node* postfix)
{
	float result = 0;
	float *stack = (float *)calloc(50,sizeof(float));
	int top = -1;
	float op1,op2;
	char op;
	if(postfix == NULL)
	{
		printf("\nInfix is Empty");
	}
	else
	{
		Node* t = postfix;
		while(t != NULL)
		{
			if(t->symbol == '_')
			{
				push_float(stack,t->number,&top);
			}
			else
			{
				if(op != '_')
				{
					op = t->symbol;
					op2 = pop_float(stack,&top);
					op1 = pop_float(stack,&top);
					switch(op)
					{
						case '+':
							push_float(stack,(op1+op2),&top);
							break;
						case '-':
							push_float(stack,(op1-op2),&top);
							break;
						case '*':
							push_float(stack,(op1*op2),&top);
							break;
						case '/':
							push_float(stack,(op1/op2),&top);
							break;
						case '^':
							push_float(stack,(Pow(op1,op2)),&top);
							break;
						default :
							break;
					}
				}
			}
			t = t->next;
		}
	}
	result = pop_float(stack,&top);
	free(stack);
	return result;

}

/*
Input  : Infix Node pointer contains value and empty Postfix Node pointer
Return : postfix Node pointer

Description :
    It takes the the infix pointer and traverse through it node by node until it reaches the end
    if the node->symbol contains '_' it Push the number into the postfix linked list
    Else Based on the priority of the symbol decide to push directly to the char stack 
    Or pop some values already inside the character stack to push it to the postfix linked list
    Once the infix reaches end pop all the values inside the stack and push it to postfix and return it
*/
Node* Infix_Postfix(Node *infix,Node *postfix)
{
	char *stack = (char *)calloc(MIN,sizeof(char));
	char null = '_';
	float num = 0;
	int tos = -1;
	char symbol,check;
	if(infix == NULL)
	{
		printf("\nInfix is Empty");
	}
	else
	{
		Node* t = infix;
		while(t != NULL)
		{
			if(t->symbol == '_')
			{
				num = t->number;
				postfix = Push(postfix,null,num);
				num = 0;
			}
			else
			{
				check = t->symbol;
				switch(check)
				{
					case '(' :
						push_char(stack,&tos,check);
						break;
					case ')' :
						while((symbol = pop_char(stack,&tos)) != '(')
						{
							postfix = Push(postfix,symbol,num);
						}
						break;
					case '+' :
					case '-' :
					case '*' :
					case '/' :
					case '^' :
					case '.' :
						while(Priority(stack[tos]) >= Priority(check))
						{
							symbol = pop_char(stack,&tos);
							postfix = Push(postfix,symbol,num);
						}
						push_char(stack,&tos,check);
				}
			}
			t = t->next;
		}
		while((symbol = pop_char(stack,&tos)) != '0')
		{
			postfix = Push(postfix,symbol,num);
		}

	}
	free(stack);
	return postfix;
}

/*
Input  : Character Array pointer
Return : Node pointer

Description :
    It travese through the character array until it reaches '\0'
    If it encouters the operators [(,),^,*,/,+,-] it push it to the infix linked list
    Else it store the number values in a small temp char array and converts it into actual floating number
	and push it into the infix linked list
	
	Array            Linked List
	+-+-+-+-+-+      +-----+  +---+   +---+
	|4|1|9|+|1|  --> |419|_|->|0|+|-->|1|_|
	+-+-+-+-+-+      +-----+  +---+   +---+
*/
Node* Array_Linked(char *input)
{
	int count = 0,k = 0,m = 0,change = 1;
	char symbol,null = '_';
	float num = 0,number = 0;
	Node *first = NULL;
	char *temp = (char *)calloc(SIZE,sizeof(char));
	for(int i=0,j=0;i<=strlen(input);i++)
		{
			switch(input[i])
			{
				case ' ':
					continue;
					break;
				case '(':
					symbol = input[i];
					first = Push(first,symbol,num);
					break;
				case ')':
					k = i;
					if(input[--k] == ')')
					{
						symbol = input[i];
						first = Push(first,symbol,0);
						break;
					}
					else
					{
						goto here;
					}
				case '+':
				case '-':
					if(i > 0)
					{
						k = i;
						k--;
						if(input[k] == ')')
						{
							symbol = input[i];
							first = Push(first,symbol,0);
							break;
						}
						else if(input[k] == '(' && input[i] == '+')
						{
							change = 1;
							break;
						}
						else if(input[k] == '(' && input[i] == '-')
						{
							change = -1;
							break;
						}
						else
						{
							symbol = input[i];
							number = atof(temp);
							number *= change;
							first  = Push(first,null,number);
							first  = Push(first,symbol,num);
							count  = 0;
							memset(temp,0,SIZE);
							number = 0;
							change = 1;
							break;
						}
					}
					else if(i == 0)
					{
						if(input[i] == '-')
						{
							change = -1;
							break;
						}
						else if(input[i] == '+')
						{
							change = 1;
							break;
						}
					}
					break;
				case '*':
				case '/':
				case '^':
					k = i;
					if(input[--k] == ')')
					{
						symbol = input[i];
						first = Push(first,symbol,0);
						break;
					}
					else
					{
						if(m == 0)
						{
							here:
							symbol = input[i];
							number = atof(temp);
							number *= change;
							first  = Push(first,null,number);
							first  = Push(first,symbol,num);
							count  = 0;
							memset(temp,0,10);
							number = 0;
							change = 1;
						}
					}
					break;
				case '\0':
					j = i; 
					if(input[--j]!=')')
					{
						number = atof(temp);
						count  = 0;
						first = Push(first,null,number);
						memset(temp,0,10);
 					}
					break;
				default :
					temp[count++] = input[i];
					break;
			}
		}
		return first;
}


/*
Input  : floating number and integer number
Return : floating number
  
Description :
    If your math.h works perfectly Include math.h header and remove this function 
    Both definition and declaration and rename every occurance of Pow() into pow() 
	It works perfectly fine
*/
float Pow(float base,int power)
{
	float ans = 1;
	if(power == 0)
	{
		return 1;
	}
	else if(power == 1)
	{
		return base;
	}
	else
	{
		for(int i=1;i<=power;i++)
		{
			ans *= base;
		}
		return ans;
	}
}

/*
Input  : character array pointer ,integer pointer and character value
Return : nothing

Description :
	Checks the overflow condition 
	Store the parameter value symbol in the stack_ptr at the index of the tos(top of stack) and increment it
*/
void push_char(char *stack_ptr,int *tos,char symbol)
{
	if(*tos >= (MIN-1))
	{
		printf("Stack Overflow\nInsertion Failed");
	}
	else
	{
		(*tos)++;
		stack_ptr[*tos] = symbol;
	}
}

/*
Input  : character array pointer ,integer pointer
Return : character

Description :
	Checks the underflow condition 
	Take value from the stack_ptr at the index of the tos(top of stack) and decrement it and return the taken value
*/
char pop_char(char *stack_ptr,int *tos)
{
	char symbol;
	if(*tos < 0)
	{
		return '0';
	}
	else
	{
		symbol = stack_ptr[*tos];
		(*tos)--;
		return symbol;
	}

}

/*
Input  : float array pointer ,float value and integer pointer
Return : nothing

Description :
	Checks the overflow condition 
	Store the parameter value num in the stack at the index of the top and increment it
*/
void push_float(float* stack,float num,int *top)
{
	if((*top) <= (MIN-1))
	{
		stack[++(*top)] = num;
	}
}

/*
Input  : float array pointer ,integer pointer 
Return : nothing

Description :
	Checks the underflow condition 
	Take the value from the stack at the index of the top and decrement it and return the taken value
*/
float pop_float(float* stack,int *top)
{
	float num;
	if((*top) >= 0)
	{
		num = stack[(*top)];
		stack[*top] = 0;
		(*top)--;
		return num;
	}
	else
	{
		return -4.004;
	}
}

/*
Input  : Node pointer ,character value and floating value
Return : Node pointer

Description :
	Create a Node and assign values for node->symbol and node->number with the parameter values and make node->next NULL
	Join the current node with the parameter node->next and return the final node pointer
*/
Node* Push(Node *first,char symbol,float number)
{
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->symbol = symbol;
	temp->number = number;
	temp->next = NULL;
	if(first == NULL)
	{
		first = temp;
	}
	else
	{
		Node *t = first;
		while(t->next != NULL)
		{
			t = t->next;
		}
		t->next = temp;
	}
	return first;
}

/*
Input  : character value
Return : integer value

Description :
	According to the priority we given it returns the integer value
*/
int Priority(char s)
{
	switch(s)
	{
	case '(':
		return 0;
	case '^':
		return 3;
	case '*':
	case '/':
		return 2;
	case '+':
	case '-':
		return 1;
	default :
		return -1;
		break;
	}
}

/*
Input  : character array pointer
Return : integer

Description :
	It Check the equality of () "Brackets" finding invalid like ((2+3) , 3*6)
	Then it checks the wrongly typed expressions like 66++3 ,-4- etc..
	If it finds anything wrong in the input expression it returns -1 
*/
int validator(char *ptr)
{
	int count = 0;
	for(int i=0;i<strlen(ptr);i++)
	{
		if(count < 0)
		{
			return -1;
		}
		if(ptr[i] == '(')
		{
			count++;
		}
		else if(ptr[i] == ')')
		{
			count--;
		}
	}

	if(count != 0)
	{
		return -1;
	}
	else 
	{
		for(int i = 0;i<sizeof(ptr)-1;i++) 
		{
			switch(ptr[i])
			{
				case '+' :
				case '-' :
				case '*' :
				case '^' :
				case '/' :
					switch(ptr[i+1])
					{
						case '+' :
						case '-' :
						case '*' :
						case '^' :
						case '/' :
							return -1;
							break;
						default :
							break;
					}
					break;
				case '(' :
					switch(ptr[i+1])
					{
					  	case '*' :
					  	case '^' :
					  	case '/' :
					  		return -1;
					  		break;
					  	default :
					  		break;
					}
					break;
				case '.' :
					switch(ptr[i+1])
					{
						case '+' :
						case '-' :
						case '*' :
						case '^' :
						case '/' :
						case '.' : 
							return -1;
							break;
						default :
							break;
					}
					break;
				default :
					break;
			}
		}
		return 1;
	}
}

