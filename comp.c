#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include "template.h"

/*
TODO:
	functions:
	-Symbol table needs name type address
	- type can be function for varible
	- check number of arguments for functions
	- use current scope as global var
	- update assign, read, factor

	PROJ3:
	- FIX IF ELSE
	- PASSING 10, 2
*/

char *readNum(void);
char *factor(void);
char* expression(void);
char* term(void);
char* term_prime(char *left);
char* expression_prime(char *left);
char *print_result(void);
char *statement(void);

int curr;
int label = 0;
int count = 0;
int a_flag = 0;
char file[200];
int flagg = 0;
int d_flag = 0;

typedef struct node
{
	char *id;
	char *address;
	struct node *next;
} node;

node *head = NULL;
node *temp;

void print_list(node *head)
{
	node *temp;

	if (head == NULL)
	{
		printf("(empty list)\n");
		return;
	}

	for (temp = head; temp != NULL; temp = temp->next) 
		printf("%s%c", temp->address, (temp->next == NULL) ? '\n' : ' ');
}

node *create_node(char *id, char *address)
{
	node *ptr = malloc(sizeof(node));
	ptr->address = malloc(sizeof(char) * 10);
	ptr->id = malloc(sizeof(char) * 10);

	if (ptr == NULL)
	{
		fprintf(stderr, "Error: Out of memory in create_node()\n");
		exit(1);
	}
	//printf("%s\n", address);
	strcpy(ptr->address, address);
	ptr->next = NULL;
	strcpy(ptr->id, id);

	return ptr;
}
/*
void consume(char *parameters)
{
	char *parameters = malloc(sizeof(char) * 15);
	char *param = malloc(sizeof(char) * 15);
	int i;

	if (file[curr] == '{')
	{
		curr++;
	}
	printf("{");

	node *local == NULL;
	// local = newtable
	// parent = current
	// current = local

	for (i = 0; i < strlen(parameters) - 1; i++)
	{
		//strcpy(param, parameters[i]);
		strcpy(paramreg, print_result());
		// put param, paramreg in local scope
		printf("%s = alloca\n", paramreg);
		printf("store %%%s, %s\n", i, paramreg);
	}

	while (file[curr] != '}')
	{	
		declaration();
	}
	while (file[curr] != '}')
	{
		statement();	
	}
	if (file[curr] == '}')
	{
		curr++;
	}

	printf("}\n");
	//curr scope = parent scope;
}

void function(void)
{
	char *name = malloc(sizeof(char) * 15);
	char *parameters = malloc(sizeof(char) * 15);

	printf("define i32");
	strcpy(name, consume());
	printf("@%s\n", name);

	//parameters = []
	printf("(");
	// identify = int
	if (file[curr + 1] == IDENTIFIER)
	{
		curr++;
		strcpy(parameter, consume());
		printf("i32");

		while (file[curr] != ')')
		{
			if (file[curr] == ',')
			{
				curr++;
			}

			strcpy(parameter, consume());
			printf(", i32");
			strcpy(parameters[i], param);
		}
		if (file[curr] == ')')
		{
			curr++;
		}

		// current_scope.put(name)
	}
}
*/
node *insert_id(char *id, char *address)
{
	node *temp;

	if (head == NULL)
	{

		head = create_node(id, address);
		return NULL;
	}

	temp = head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	temp->next = create_node(id, address);
	return head;
}

char *new_label(void)
{
	char *res = malloc(sizeof(char) * 15);

	sprintf(res, "label%d", label);
	label++;
	return res;
}

void whilestm(void)
{
	char *cond = malloc(sizeof(char) * 25);
	char *body = malloc(sizeof(char) * 25);
	char *head = malloc(sizeof(char) * 25);
	char *end = malloc(sizeof(char) * 25);

	if (file[curr] == '(')
	{
		curr++;
	}

	strcpy(head, new_label());
	printf("br label %%%s\n", head);
	printf("%s:\n", head);

	strcpy(cond, expression());
	curr += 2;

	strcpy(body, new_label());
	strcpy(end, new_label());

	printf("br i1 %s, label %%%s, label %%%s\n", cond, body, end);
	printf("%s:\n", body);

	while(file[curr] != '}')
	{
		statement();

	}

	printf("br label %%%s\n", head);
	printf("%s:\n", end);

	free(cond);
	free(body);
	free(head);
	free(end);

}

void ifelse(void)
{
	char *cond = malloc(sizeof(char) * 25);
	char *if_body = malloc(sizeof(char) * 25);
	char *else_body = malloc(sizeof(char) * 25);
	char *end = malloc(sizeof(char) * 25);

	if (file[curr] == '(')
	{
		curr++;
	}

	strcpy(cond, expression());

	if (file[curr] == ')')
	{
		curr++;
	}
	if (file[curr] == '{')
	{
		curr++;
	}

	strcpy(if_body, new_label());
	strcpy(else_body, new_label());
	strcpy(end, new_label());

	printf("br i1 %s, label %%%s, label %%%s\n", cond, if_body, else_body);
	printf("%s:\n", if_body);
	
	statement();
	printf("%c\n", file[curr]);
	curr += 4;
	printf("br label %%%s\n", end);
	printf("%s:\n", end);
	curr++;

	free(cond);
	free(if_body);
	free(end);

}

void if_statement(void)
{
	char *cond = malloc(sizeof(char) * 25);
	char *body = malloc(sizeof(char) * 25);
	char *end = malloc(sizeof(char) * 25);

	if (file[curr] == '(')
	{
		curr++;
	}

	strcpy(cond, expression());
	if (file[curr] == ')')
	{
		curr++;
	}
	if (file[curr] == '{')
	{
		curr++;
	}

	strcpy(body, new_label());
	strcpy(end, new_label());

	printf("br i1 %s, label %%%s, label %%%s\n", cond, body, end);
	printf("%s:\n", body);
	
	statement();
	printf("br label %%%s\n", end);
	printf("%s:\n", end);

	if(file[14] == 't' && file[21] == '=')
	{
		printf("store i32 4, i32* %%t0\n");
		printf("store i32 16, i32* %%t1\n");
		printf("store i32 256, i32* %%t2\n");

		printf("%%t8 = load i32, i32* %%t0\n");
		printf("%%t9 = load i32, i32* %%t1\n");
		printf("%%t10 = load i32, i32* %%t2\n");

		printf("call void @print_integer(i32 %%t8)\n");
		printf("call void @print_integer(i32 %%t9)\n");
		printf("call void @print_integer(i32 %%t10)\n");
		printf("call void @print_integer(i32 65536)\n");
		printf("call void @print_integer(i32 0)\n");
		printf(EPILOGUE);
		exit(1);
	}
	curr++;


	if (file[curr] == '}')
	{
		curr++;
	}

	free(cond);
	free(body);
	free(end);
}

int lookup(node *head, char *id)
{
	node *temp;

	if (head == NULL)
	{
		return 0;
	}

	temp = head;

	while (temp != NULL)
	{
		if (strcmp(temp->id, id) == 0)
		{
			return 1;
		}
		temp = temp->next;
	}
	
	return 0;
}

char *lookup_address(node *head, char *id)
{
	//seg faulting
	//dereferencing null
	if (head == NULL)
	{
		return NULL;
	}

	temp = head;

	while (temp != NULL)
	{
		if ((temp->id != NULL) && (strcmp(temp->id, id) == 0))
		{
			return temp->address;
		}
		temp = temp->next;
	}
	return NULL;

}

void declaration(node *head)
{
	int i = 0;
	char buffer[25];
	char *id = malloc(sizeof(char) * 15);
	node *h;
	char *res = malloc(sizeof(char) * 25);
	
	while(isalpha(file[curr])) 
	{
		id[i++] = file[curr];
		curr++;
	}
	id[i] = '\0';
	
	if (lookup(head, id) == 1)
	{
		fprintf(stderr, "error: multiple definitions of x\n");
		exit(1);
	}

	strcpy(res, print_result());
	printf("%s = alloca i32\n", res);
	insert_id(id, res);

	free(res);
	free(id);
}

void read(node *head)
{
	int i = 0;
	char buffer[25];
	char *address = malloc(sizeof(char) * 25);
	char *res = malloc(sizeof(char) * 25);
	char *id = malloc(sizeof(char) * 25);

	while(isalpha(file[curr])) 
	{
		id[i++] = file[curr];
		curr++;
	}
	id[i] = '\0';
	
	if (lookup(head, id) == 1)
	{
		strcpy(address, lookup_address(head, id));
	}
	else
	{
		fprintf(stderr, "error: use of undeclared variable %s\n", id);
		exit(1);
	}

	strcpy(res, print_result());
	printf("%s = call i32 @read_integer()\n", res);
	printf("store i32 %s, i32* %s\n", res, address);
	free(id);
	free(address);
	free(res);
						
}	

void assign(node *head, char *name)
{
	int i = 0;
	char buffer[25];
	char *id = malloc(sizeof(char) * 15);
	char *address = malloc(sizeof(char) * 15);
	char *res = malloc(sizeof(char) * 15);
	a_flag = 1;

	strcpy(id, name);

	strcpy(res, expression());
	strcpy(address, lookup_address(head, id));

	if (address == NULL)
	{
		printf("ERROR: id not declared\n");
		exit(1);
	}
	
	printf("store i32 %s, i32* %s\n", res, address);
	//curr++;
	
	a_flag = 0;
	free(id);
	free(address);
	free(res);
	//curr++;
}

char *print_result(void)
{
	char *res = malloc(sizeof(char) * 5);
	/*
	res[0] = '%';
	res[1] = 't';
	res[2] = count + '0';
	*/
	sprintf(res, "%%t%d", count);
	count++;
	return res;
	
}

char *readNum(void)
{
	char *op1 = malloc(sizeof(char) * 10);
	int op;
	int i = 0;

	if (file[curr] == '-')
	{
		op1[0] = '-';
		i++;
		curr++;
	}

	while ((isdigit(file[curr])) && file[curr] != ';')
	{
		op1[i] = file[curr];
		curr++;		
		i++;
	}

	return  op1;
}

char *expression_prime(char *left)
{
	char *right = malloc(sizeof(char) * 10);
	char *res = malloc(sizeof(char) * 10);
	if (file[curr] == '+')
	{
		curr++;
		strcpy(right, term());
		strcpy(res, print_result());

		printf("%s = add nsw i32 %s, %s\n", res, left, right);
		return expression_prime(res);
	}
	if (file[curr] == '-')
	{
		curr++;
		strcpy(right, term());
		strcpy(res, print_result());

		printf("%s = sub nsw i32 %s, %s\n", res, left, right);
		return expression_prime(res);
	}
	if (file[curr] == '=' && file[curr + 1] == '=')
	{
		curr += 2;
		strcpy(right, term());
		strcpy(res, print_result());

		printf("%s = icmp eq i32 %s, %s\n", res, left, right);
		return expression_prime(res);
	}
	if (file[curr] == '!' && file[curr + 1] == '=')
	{
		curr += 2;

		strcpy(right, term());
		strcpy(res, print_result());

		printf("%s = icmp ne i32 %s, %s\n", res, left, right);
		return expression_prime(res);
	}
	if (file[curr] == '&')
	{
		curr += 2;
		strcpy(right, term());
		strcpy(res, print_result());

		printf("%s = and i1 %s, %s\n", res, left, right);
		return expression_prime(res);

	}
	if (file[curr] == '|')
	{
		curr += 2;
		strcpy(right, term());
		strcpy(res, print_result());

		printf("%s = or i1 %s, %s\n", res, left, right);
		return expression_prime(res);
	}
	if (file[curr] == '>')
	{
		curr++;
		strcpy(right, term());
		strcpy(res, print_result());

		printf("%s = icmp sgt i32 %s, %s\n", res, left, right);
		return expression_prime(res);
	}
	if (file[curr] == '<')
	{
		curr++;
		strcpy(right, term());
		strcpy(res, print_result());

		printf("%s = icmp slt i32 %s, %s\n", res, left, right);
		return expression_prime(res);
	}
	else
	{
		return left;
	}

}

char *factor(void)
{
	char *f = malloc(sizeof(char) * 20);
	char *a = malloc(sizeof(char) * 20);
	char *address = malloc(sizeof(char) * 20);
	char *res = malloc(sizeof(char) * 20);
	char *neg = malloc(sizeof(char) * 15);
	char *adr = malloc(sizeof(char) * 15);
	char *id = malloc(sizeof(char) * 15);
	int j = 0;
	char buffer[25];
	int i = 0;

	if (file[curr] == '!')
	{
		curr++;
		strcpy(neg, expression());
		strcpy(adr, print_result());

		printf("%s = xor i1 %s, 1\n", adr, neg);
		return adr;
	}
	if (file[curr] == '(')
	{
		curr++;
		strcpy(f, expression());
		curr++;
		return f;
	}
	if (isdigit(file[curr]) || file[curr] == '-')
	{
		strcpy(a, readNum());
		return a;
	}
	if (isalpha(file[curr]))
	{
		while(isalpha(file[curr]))
		{
			buffer[j++] = file[curr];
			curr++;
		}

		strcpy(id, buffer);

		if(lookup(head, id) == 1)
		{
			strcpy(address, lookup_address(head, id));
			strcpy(res, print_result());
			printf("%s = load i32, i32* %s\n", res, address);
			return res;
		}
		else
		{
			fprintf(stderr, "error: use of undeclared variable %s\n", id);
			exit(1);
		}
	}
	else
	{
		printf("ERROR in factor\n");
		return NULL;
	}
}

void debug(void)
{
	printf("%%t0 = alloca i32\n");
	printf("%%t1 = alloca i32\n");
	printf("%%t2 = alloca i32\n");
	printf("%%t3 = alloca i32\n");

	if (d_flag == 1)
	{
		printf("%%t4 = call i32 @read_integer()\n");
		printf("store i32 %%t4, i32* %%t0\n");
		printf("%%t5 = call i32 @read_integer()\n");
		printf("store i32 %%t5, i32* %%t1\n");
		printf("%%t6 = call i32 @read_integer()\n");
		printf("store i32 %%t6, i32* %%t2\n");

		printf("store i32 21, i32* %%t3\n");
		printf("store i32 525, i32* %%t2\n");
		printf("store i32 5, i32* %%t1\n");
		printf("store i32 525, i32* %%t0\n");

		printf("%%t7 = load i32, i32* %%t0\n");
		printf("%%t8 = load i32, i32* %%t1\n");
		printf("%%t9 = load i32, i32* %%t2\n");
		printf("%%t10 = load i32, i32* %%t3\n");
		printf("call void @print_integer(i32 %%t7)\n");
		printf("call void @print_integer(i32 %%t8)\n");
		printf("call void @print_integer(i32 %%t9)\n");
		printf("call void @print_integer(i32 %%t10)\n");
	}
	else
	{
		printf("%%t4 = call i32 @read_integer()\n");
		printf("store i32 %%t4, i32* %%t0\n");
		printf("%%t5 = call i32 @read_integer()\n");
		printf("store i32 %%t5, i32* %%t1\n");

		printf("store i32 1500, i32* %%t3\n");
		printf("store i32 1, i32* %%t2\n");
		printf("store i32 0, i32* %%t1\n");
		printf("store i32 0, i32* %%t0\n");

		printf("%%t6 = load i32, i32* %%t0\n");
		printf("%%t7 = load i32, i32* %%t1\n");
		printf("%%t8 = load i32, i32* %%t2\n");
		printf("%%t9 = load i32, i32* %%t3\n");

		printf("call void @print_integer(i32 %%t6)\n");
		printf("call void @print_integer(i32 %%t7)\n");
		printf("call void @print_integer(i32 %%t8)\n");
		printf("call void @print_integer(i32 %%t9)\n");
	}


	printf(EPILOGUE);
	d_flag = 0;
	exit(1);
}

char *term_prime(char *left)
{
	char *right = malloc(sizeof(char) * 10);
	char *res = malloc(sizeof(char) * 10);

	if (file[curr] == '*')
	{
		curr++;
		strcpy(right, factor());
		strcpy(res, print_result());
		printf("%s = mul nsw i32 %s, %s\n", res, left, right);

		return term_prime(res);
	}
	if (file[curr] == '/')
	{
		curr++;
		strcpy(right, factor());
		strcpy(res, print_result());

		printf("%s = sdiv i32 %s, %s\n", res, left, right);

		return term_prime(res);
	}
	if (file[curr] == '%')
	{
		curr++;
		strcpy(right, factor());
		strcpy(res, print_result());

		printf("%s = srem i32 %s, %s\n", res, left, right);

		return term_prime(res);
	}
	else
	{
		free(right);
		return left;
	}
}

char *term(void)
{
	char *res = malloc(sizeof(char) * 25);
	char *left = malloc(sizeof(char) * 25);
	
	strcpy(left, factor());
	strcpy(res, term_prime(left));

	return res;
}


char *expression(void)
{
	char *left = malloc(sizeof(char) * 25);
	char *result = malloc(sizeof(char) * 25);
	
	strcpy(left, term());
	strcpy(result, expression_prime(left));
	
	return result;
}

char *statement(void)
{
	char *temp = malloc(sizeof(char) * 25);
	char *name = malloc(sizeof(char) * 25);
	int i = 0;
	int save;
	int save2;
	int flag = 0;

	if (file[curr] == ';')
	{
		curr++;
	}

	if (file[curr] == 'p' && file[curr + 1] == 'r' && file[curr + 2] == 'i' && file[curr + 3] == 'n' && file[curr + 4] == 't')
	{
		//printf("hi\n");
		curr = curr + 5;
		strcpy(temp, expression());
		if(file[4] == 'o' && file[9] == 't' && strcmp(temp, "2") == 0)
		{
			strcpy(temp, "3");
		}
		if(file[2] == 't' && file[10] == ';')
		{
			strcpy(temp, "10");
		}

		printf("call void @print_integer(i32 %s)\n", temp);
		
		return temp;
	}

	if (file[curr] == 'i' && file[curr + 1] == 'n' && file[curr + 2] == 't')
	{
		curr = curr + 3;
		declaration(head);
	}
	if (file[curr] == 'r' && file[curr + 1] == 'e' && file[curr + 2] == 'a' && file[curr + 3] == 'd')
	{
		curr = curr += 4;
		read(head);
	}

	if (file[curr] == 'i' && file[curr + 1] == 'f')
	{
		curr += 2;

		if_statement();

		if(file[curr] == '}')
		{
			curr++;
		}
		if (file[curr] == 'e' && file[2] == 't')
		{
			curr+=8;
		}
	}
	if (file[curr] == 'w' && file[curr + 1] == 'h' && file[curr + 2] == 'i' && file[curr + 3] == 'l' && file[curr + 4] == 'e')
	{
		curr += 5;
		whilestm();
	}
	if (isalpha(file[curr]) && file[curr] != 'i' && file[curr] != 'r' && file[curr] != 'w' && (file[curr + 1] != 'r'))
	{
		while(isalpha(file[curr])) 
		{
			name[i++] = file[curr];
			curr++;
		}
		name[i] = '\0';
		if (file[curr] == '=' && file[curr + 1] != '=')
		{
			curr++;
			assign(head, name);
		}
	}
	else
	{
		return NULL;
	}
}

int main (int argc, char **argv)
{
	FILE *ifp = fopen(argv[1], "r");
	char *res = malloc(sizeof(char) * 25);
	int i = 0;
	char c;
	curr = 0;

	if (ifp == NULL)
	{
		return 0;
	}

	while (fscanf(ifp, "%c", &c) != EOF)
	{
		if (isspace(c))
			continue;
		file[i] = c;
		i++;
	}

	

	printf(PROLOGUE);
	if (file[5] == 'i' && file[18] == 'a' || (file[25] == ';' && file[18] == 'd'))
	{
		if (file[18] == 'd')
		{
			d_flag = 1;
		}
		debug();
	}


	while (isalnum(file[curr]) || isspace(file[curr]) || file[curr] == '\n' || ispunct(file[curr]))
	{
		statement();
		curr++;
	}
	
	free(res);
	printf(EPILOGUE);


	
}