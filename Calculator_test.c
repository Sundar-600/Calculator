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
int validator(char *);
float Pow(float ,int );
int Priority(char );

//Conversions
Node* Array_Linked(char *);
Node* Infix_Postfix(Node*,Node *);

//Linked-list Functions
Node* Push(Node *,char ,float );


//Postfix stack Operations
void push_char(char *,int *,char );
char pop_char(char *,int *);

//Evaluation
float Evaluation(Node* );
void push_float(float *,float,int *);
float pop_float(float *,int *);


int main()
{
	char *input = (char *)calloc(MAX,sizeof(char));
	int ch,valid;
	Node *infix = NULL;
	Node *postfix = NULL;
	do
	{
		scanf("%d",&ch);
		switch(ch)
		{
			case 1 :
				scanf("%s",input);
				valid = validator(input);
				if(valid == 1)
				{
					infix = Array_Linked(input);
					postfix = Infix_Postfix(infix, postfix);
					printf("%.3f\n",Evaluation(postfix));
					infix = NULL;
					postfix = NULL;
				}
				else
				{
					printf("Invalid\n");
				}
				break;
			case 2 : 
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
	return result;

}

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
	return postfix;
}


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

void push_float(float* stack,float num,int *top)
{
	if((*top) <= (MIN-1))
	{
		stack[++(*top)] = num;
	}
}

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

