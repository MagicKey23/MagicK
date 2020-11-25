#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32
#include <string.h>


static char buffer[2048];

/* Evaluate */

/* Use operator string to see which operation to perform */
long eval_op(long x, char* op, long y) {
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  if (strcmp(op, "%") == 0) { return x % y; }
  return 0;
}
	
long eval(mpc_ast_t* a){
	//Assumed the user didn't input any operator
	if(strstr(a->tag, "int")){
		return atoi(a->contents);
	}else if(strstr(a->tag, "double")){
		return atof(a->contents);
	}
	//If operator exist 
	char* op = a->children[1]->contents;
	//Store the third child is x
	long x = eval(a->children[2]);
	
	int i = 3;
	while(strstr(a->children[i]->tag,"expr")){
		x = eval_op(x, op, eval(a->children[i]));
		i++;
	}
	
	return x;
}


/* Fake readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* Otherwise inclu		de the editline headers */
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {
//Create a parser
 mpc_parser_t* Int = mpc_new("int");
 mpc_parser_t* Operator = mpc_new("operator");
 mpc_parser_t* Expr = mpc_new("expr");
 mpc_parser_t* MagicK = mpc_new("magicK");
 mpc_parser_t* Double = mpc_new("double");
 
//Set up structure grammar

 mpca_lang(MPCA_LANG_DEFAULT,
	  "                                                   		  					   	\
	double	 :	/-?[0-9]+\\.[0-9]+/;   			  		 	 	  						 \
    int      : /-?[0-9]+/;             					 	 							 \
    operator : '+' | '-' | '*' | '/' | '%' | \"add\" | \"sub\" | \"mul\" | \"modus\";                 			  \
    expr     : <double> | <int> | '(' <operator> <expr>+ ')' ;  						 \
    magicK   : /^/ <operator> <expr>+ /$/ ;             	    						  \
  ",
  Int, Double, Operator, Expr, MagicK);
  
  puts("MagicK Version 0.0.0.0.1");
  puts("Press Ctrl+c to Exit\n");

  while (1) {

    /* Now in either case readline will be correctly defined */
    char* input = readline("MagicK> ");
    add_history(input);


	//* Parse the user Input
	mpc_result_t r;
	if(mpc_parse("<stdin>", input, MagicK, &r)){
		/* Input Valid Print the AST */
		long result = eval(r.output);
		printf("%li\n", result);
		mpc_ast_delete(r.output);
		
		
	}else{
		/* Otherwise Print the Error */
		mpc_err_print(r.error);
		mpc_err_delete(r.error);
		
	}
	


    free(input);
  }
	
	//FREE THE HEAP AND REMOVE THE PARCER
	mpc_cleanup(5, Int, Double, Operator, Expr, MagicK);
  return 0;
}