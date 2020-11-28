


#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"
#include "errorEval.h"

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32
#include <string.h>


static char buffer[2048];

/* Evaluate */

/* Use operator string to see which operation to perform */

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
 mpc_parser_t* Double = mpc_new("double");
 mpc_parser_t* Expr = mpc_new("expr");
 mpc_parser_t* Sexpr  = mpc_new("sexpr");
 mpc_parser_t* Qexpr = mpc_new("qexpr");
 mpc_parser_t* Symbol = mpc_new("symbol");
 mpc_parser_t* MagicK = mpc_new("magicK");

//Set up structure grammar

 mpca_lang(MPCA_LANG_DEFAULT,
	  "                                                   		  					   	 \
	double	 :	/-?[0-9]+\\.[0-9]+/;   			  		 	 	  						 \
    int      : /-?[0-9]+/;             					 	 							 \
	sexpr    : '(' <expr>* ')';															 \
	qexpr    : '{' <expr>* '}';															 \
	symbol   : \"list\" | \"head\" | \"tail\" | \"join\" | \"eval\" 			         \
			 | '+' | '-'  | '*' | '/' | '%' 											 \
			 | \"add\" | \"sub\" | \"mul\" | \"mod\";                  				     \
    expr     : <double> | <int> | <symbol> | <sexpr> | <qexpr>;  						 \
    magicK   : /^/ <expr>* /$/ ;             	    									 \
  ",
  Int, Double, Expr, Sexpr, Qexpr, Symbol, MagicK);
  // 
  puts("MagicK Version 0.0.0.0.5");
  puts("Press Ctrl+c to Exit\n");

  while(1) {

    /* Now in either case readline will be correctly defined */
    char* input = readline("MagicK> ");
    add_history(input);


	//* Parse the user Input
	mpc_result_t r;
	if(mpc_parse("<stdin>", input, MagicK, &r)){
		/* Input Valid Print the AST */
		lval* x = lval_eval(lval_read(r.output));
		println(x);
		lval_del(x);
		
	}else{
		/* Otherwise Print the Error */
		mpc_err_print(r.error);
		mpc_err_delete(r.error);
		
	}
	


    free(input);
  }
	
	//FREE THE HEAP AND REMOVE THE PARCER
	mpc_cleanup(6, Int, Double, Sexpr, Qexpr, Expr, MagicK);
  return 0;
}