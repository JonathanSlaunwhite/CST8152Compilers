/****************************************************************************************
 *  File name: Table.h
 *  Compiler Visual Studio 2019
 *  Author: Jonathan Slaunwhite , 040939090
 *  Course:CST 8152 –Compilers, Lab Section:013
 *  Assignment 2
 *  Date: 2020-03-22
 *  Professor: Sv.Ranev
 *  Purpose: This is the table.h where all definitions are stored
 *  Version: 1.20.1
 *  Function list: st_table,as_table,Token
 *  aa_func02,aa_func03,aa_func05,aa_func08,aa_func10,aa_func12,
 *
 ****************************************************************************************/

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

 /*   Source end-of-file (SEOF) sentinel symbol
  *    '\0' or one of 255,0xFF,EOF
  */

#define SEOF '\0' //SEOF source end of file
#define S_EOF (unsigned char)255 //unsigned source end of file 255
#define NL '\n'  //new line 
#define TB '\t' //tab

#define SEOFNEW 0xFF// i added this maybe is right ??							123123123
#define quote '"'



  //PLACE YOUR CONSTANT DEFINITIONS HERE IF YOU NEED ANY



  //REPLACE *ESN* and *ESR* WITH YOUR ERROR STATE NUMBER 
#define ES  11 /* Error state  with no retract */
#define ER  12 /* Error state  with retract */
#define IS -1    /* Inavalid state */

/* State transition table definition */

//REPLACE *CN* WITH YOUR COLUMN NUMBER  

#define TABLE_COLUMNS 8//six rows in table st_table

/*transition table - type of states defined in separate table */



int  st_table[][TABLE_COLUMNS] = {



	//1467

	/* State 0 */  {1,6,4,ES,ES,ES,9,ER},
	/* State 1 */  {1,1,1,2,3,2,ES,ER},
	/* State 2 */  {IS,IS,IS,IS,IS,IS,IS,IS},
	/* State 3 */  {IS,IS,IS,IS,IS,IS,IS,IS},
	/* State 4 */  {ES,4,4,7,5,5,IS,IS},
	/* State 5 */  {IS,IS,IS,IS,IS,IS,IS,IS},
	/* State 6 */  {ES,6,ES,7,ES,5,ES,ER},
	/* State 7 */  {8,7,7,8,8,8,ES,ER},
	/* State 8 */  {IS,IS,IS,IS,IS,IS,IS,IS},
	/* State 9 */  {9,9,9,9,9,9,10,ES},
	/* State 10 */  {IS,IS,IS,IS,IS,IS,IS,IS},
	/* State 11 */  {IS,IS,IS,IS,IS,IS,IS,IS},
	/* State 12 */  {IS,IS,IS,IS,IS,IS,IS,IS},

};



//.
//.YOUR TABLE INITIALIZATION HERE
//.
/* State N */ // {YOUR INITIALIZATION},

/* Accepting state table definition */
//REPLACE* N1*, * N2*, and* N3* WITH YOUR NUMBERS
#define ASWR     2  /* accepting state with retract */
#define ASNR     3  /* accepting state with no retract */
#define NOAS     0  /* not accepting state */

//int as_table[ ] = {YOUR INITIALIZATION HERE - USE ASWR, ASNR, NOAS };

int as_table[] = {

	/*State 0*/ NOAS,
	/*State 1*/ NOAS,
	/*State 2*/ ASWR,
	/*State 3*/ ASNR,
	/*State 4*/ NOAS,
	/*State 5*/ ASWR,
	/*State 6*/ NOAS,
	/*State 7*/ NOAS,
	/*State 8*/ ASWR,
	/*State 9*/ NOAS,
	/*State 10*/ ASNR,
	/*State 11*/ ASNR,
	/*State 12*/ ASWR,

};


/* Accepting action function declarations */

/*
FOR EACH OF YOUR ACCEPTING STATES YOU MUST PROVIDE
ONE FUNCTION PROTOTYPE. THEY ALL RETURN Token AND TAKE
ONE ARGUMENT: A string REPRESENTING A TOKEN LEXEME.
*/
// Token aa_funcXX(char *lexeme); 


Token aa_func02(char* lexeme);
Token aa_func03(char* lexeme);
Token aa_func05(char* lexeme);
Token aa_func08(char* lexeme);
Token aa_func10(char* lexeme);
Token aa_func11(char* lexeme);
Token aa_func12(char* lexeme);

//Replace XX with the number of the accepting state: 02, 03 and so on.

/* defining a new type: pointer to function (of one char * argument)
   returning Token
*/

typedef Token(*PTR_AAF)(char* lexeme);


/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*aa_table[])(char lexeme[]) = {
 */

PTR_AAF aa_table[] = {

	/*
HERE YOU MUST PROVIDE AN INITIALIZATION FOR AN ARRAY OF POINTERS
TO ACCEPTING FUNCTIONS. THE ARRAY HAS THE SAME SIZE AS as_table[ ].
YOU MUST INITIALIZE THE ARRAY ELEMENTS WITH THE CORRESPONDING
ACCEPTING FUNCTIONS (FOR THE STATES MARKED AS ACCEPTING IN as_table[]).
THE REST OF THE ELEMENTS MUST BE SET TO NULL.
*/


	/*State 0*/ NULL,
	/*State 1*/ NULL,
	/*State 2*/ aa_func02,
	/*State 3*/ aa_func03,
	/*State 4*/ NULL,
	/*State 5*/ aa_func05,
	/*State 6*/ NULL,
	/*State 7*/ NULL,
	/*State 8*/ aa_func08,
	/*State 9*/ NULL,
	/*State 10*/ aa_func10,
	/*State 11*/ aa_func11,
	/*State 12*/ aa_func12,


};

/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE  10

char* kw_table[] =
{
"ELSE",
"FALSE",
"IF",
"PLATYPUS",
"READ",
"REPEAT",
"THEN",
"TRUE",
"WHILE",
"WRITE"
};

#endif
