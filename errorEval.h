
#ifndef error_eval_h
#define error_eval_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include "mpc.h"

//Declare new Struct
typedef struct lval {
  int type;
  long num;
  /* Error and Symbol types have some string data */
  char* err;
  char* sym;
  /* Count and Pointer to a list of "lval*" */
  int count;
  struct lval** cell;
} lval;

//LVAL Gramatical
lval* lval_sexpr(void);
lval* lval_num(long x);
lval* lval_err(char* m);
lval* lval_sym(char* m);
lval* lval_read_num(mpc_ast_t* t);
lval* lval_read(mpc_ast_t* t);
lval* lval_qexpr(void);	
lval* lval_add(lval* v, lval* x);

//Display error to console

void println(lval *v);
void lval_expr_print(lval* v, char open, char close);
void lval_print(lval* v);

//FREE MEMORY
void lval_del(lval* v);


//Evaluation
lval* lval_eval(lval* v);
lval* lval_eval_sexpr(lval* v);
lval* lval_pop(lval* v, int i);
lval* builtin_op(lval* a, char* op);
lval* lval_take(lval* v, int i);

//POP HEAD TAIL JOIN

lval* builtin_head(lval* a);
lval* builtin_tail(lval* a);

//Q-EXPRESSION
lval* builtin_list(lval* a); 
lval* builtin_eval(lval* a);
lval* builtin_join(lval* a);
lval* lval_join(lval* x, lval* y);
lval* builtin(lval* a, char* func);
#ifdef __cplusplus
}
#endif

#endif
