

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
void match(int , int );
extern char* kw_table[];//extertan kw_table
extern int line;//external line 