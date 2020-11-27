#include "errorEval.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include "mpc.h"

//CREATE Enumeration of possible lval types 
enum {LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_SEXPR};
//CREATE Enumeration of possible error types
enum {LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM};
/* Construct a pointer to a new Number lval */
lval* lval_num(long x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}
/* Construct a pointer to a new Error lval */
lval* lval_err(char* m) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);
  return v;
}


/* Construct a pointer to a new Symbol lval */
lval* lval_sym(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->sym = malloc(strlen(s) + 1);
  strcpy(v->sym, s);
  return v;
}

/* A pointer to a new empty Sexpr lval */
lval* lval_sexpr(void){
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

//FREE MEMORY
void lval_del(lval* v) {

  switch (v->type) {
    /* Do nothing special for number type */
    case LVAL_NUM: break;

    /* For Err or Sym free the string data */
    case LVAL_ERR: free(v->err); break;
    case LVAL_SYM: free(v->sym); break;

    /* If Sexpr then delete all elements inside */
    case LVAL_SEXPR:
      for (int i = 0; i < v->count; i++) {
        lval_del(v->cell[i]);
      }
      /* Also free the memory allocated to contain the pointers */
      free(v->cell);
    break;
  }

  /* Free the memory allocated for the "lval" struct itself */
  free(v);
}

lval* lval_read_num(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("invalid number");
}
lval* lval_read(mpc_ast_t* t) {

  /* If Symbol or Number return conversion to that type */
  if (strstr(t->tag, "int")) { return lval_read_num(t); }
  else if (strstr(t->tag, "double")) { return lval_read_num(t); }
  
  if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

  /* If root (>) or sexpr then create empty list */
  lval* x = NULL;
  if (strcmp(t->tag, ">") == 0) { x = lval_sexpr(); }
  if (strstr(t->tag, "sexpr"))  { x = lval_sexpr(); }

  /* Fill this list with any valid expression contained within */
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
    x = lval_add(x, lval_read(t->children[i]));
  }

  return x;
}


lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

//EXP PRINT
void lval_expr_print(lval* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {

    /* Print Value contained within */
    lval_print(v->cell[i]);

    /* Don't print trailing space if last element */
    if (i != (v->count-1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void lval_print(lval* v) {
  switch (v->type) {
    case LVAL_NUM:   printf("%li", v->num); break;
    case LVAL_ERR:   printf("Error: %s", v->err); break;
    case LVAL_SYM:   printf("%s", v->sym); break;
    case LVAL_SEXPR: lval_expr_print(v, '(', ')'); break;
  }
}

void println(lval* v) { lval_print(v); putchar('\n'); }


lval* lval_eval_sexpr(lval* v) {
	// Evaluate Children
	for(int i = 0; i < v->count;i++){
		v->cell[i] = lval_eval(v->cell[i]);
	}
	
	for(int i = 0; i < v->count; i++){
		if(v->cell[i]->type == LVAL_ERR){return lval_take(v,i);}
	}
			
		//EMPTY EXPRESSION
	if(v->count == 0){return v;}
		//SINGLE EXPRESSION
	if(v->count == 1){return lval_take(v,0);}
	/* Ensure First Element is Symbol */
	lval* f = lval_pop(v,0);
	
	if (f->type != LVAL_SYM) {
    lval_del(f); 
	lval_del(v);
    return lval_err("S-expression Does not start with symbol!");
  }

  /* Call builtin with operator */
  lval* result = builtin_op(v, f->sym);
  lval_del(f);
  return result;
}


lval* lval_eval(lval* v){
  /* Evaluate Sexpressions */
  
  if (v->type == LVAL_SEXPR){ return lval_eval_sexpr(v); }
  
  /* All other lval types remain the same */
  
  return v;
		
}

lval* lval_pop(lval* v, int i) {
  /* Find the item at "i" */
  
  lval* x = v->cell[i];

  /* Shift memory after the item at "i" over the top */
  
  memmove(&v->cell[i], &v->cell[i+1],
    sizeof(lval*) * (v->count-i-1));

  /* Decrease the count of items in the list */
  
  v->count--;

  /* Reallocate the memory used */
  
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  return x;
}

lval* lval_take(lval* v, int i) {
  lval* x = lval_pop(v, i);
  lval_del(v);
  return x;
}

lval* builtin_op(lval* a, char* op){
	// Ensure all arguments are numbers
	for(int i =0; i < a->count; i++){
		if(a->cell[i]->type != LVAL_NUM){
			lval_del(a);
		      return lval_err("Cannot operate on non-number!");
		}
		
	}
	
	//POP THE FIRST ELEMENT
	lval* x = lval_pop(a,0);
	
  /* If no arguments and sub then perform unary negation */
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num; //<--- Negation
  }
  
  //While there still elements are remaining
  
  while(a->count > 0){
		
		//POP the next element
		
		lval* y = lval_pop(a, 0);
			
		if (strcmp(op, "+") == 0) { x->num += y->num; }
		if (strcmp(op, "-") == 0) { x->num -= y->num; }
		if (strcmp(op, "*") == 0) { x->num *= y->num; }
		if (strcmp(op, "%") == 0) { x->num %= y->num; }
		if (strcmp(op, "/") == 0) {
		  if (y->num == 0) {
			lval_del(x); 
			lval_del(y);
			x = lval_err("Division By Zero!"); break;
		  }
		  x->num /= y->num;
		}
		
	  lval_del(y);
	  
  }
	
  lval_del(a); 
  
  return x;
  
  
}

