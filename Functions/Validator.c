#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int validator(char*);

int main()
{
	char *input = (char *)calloc(1000,sizeof(char));
	scanf("%s",input);
	int valid = validator(input);
	if(valid == 1)
	{
		printf("Valid Statement\n");
	}
	else 
	{
		printf("Invalid Statement\n");
	}
	return 0;
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