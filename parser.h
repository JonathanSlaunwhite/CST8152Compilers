

#include "Token.h"
#include "buffer.h"


/*step one global varibles*/
Token lookahead;
int synerrno;


#define NO_ATTR -1//no attribute for matching 



/*keywords in table.h at position x*/
#define ELSE 0//else at 0
#define FALSE 1//false at 1 	
#define IF 2//if at 2 			
#define PLATYPUS 3//platypus at 3		
#define READ 4//read at 4	
#define REPEAT 5//repeat at 5	
#define THEN 6//then at 6	
#define TRUE 7//true at 7
#define WHILE 8//while at 8	
#define WRITE 9	//write at 9




/*List of all functions in parser.c*/
void parser(void);
void match(int , int );void syn_eh(int);void syn_printe();void gen_incode(char*);void program();void opt_statements();void statements();void statements_p();void statement();void assignment();void assignment_expression();void selection();void iteration();void pre_condition();void input();void variable_list();void variable_list_p();void variable_identifier();void output();void output_list();void arithmetic();void unary_arithmetic();void additive_arithmetic();void additive_arithmetic_p();void multiplicative_arithmetic();void multiplicative_arithmetic_p();void primary_a_relational();void primary_s_relational();extern Token malar_next_token();//external malar next tokenextern Buffer* str_LTBL;//external str_ltbl
extern char* kw_table[];//extertan kw_table
extern int line;//external line 