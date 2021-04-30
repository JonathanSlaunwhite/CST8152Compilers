/****************************************************************************************
 *  File name: Scanner.c
 *  Compiler Visual Studio 2019
 *  Author: Jonathan Slaunwhite , 040939090
 *  Course:CST 8152 –Compilers, Lab Section:013
 *  Assignment 2
 *  Date: 2020-03-22
 *  Professor: Sv.Ranev
 *  Purpose: This is the scanner implementation
 *  Version: 1.20.1
 *  Function list: scanner_init,malar_next_token,get_next_state,char_class
 *  aa_func02,aa_func03,aa_func05,aa_func08,aa_func10,aa_func12,
 *
 ****************************************************************************************/

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

//#include "token.h"//i added this myself
//#include "table.h"//i added this myself


#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern pBuffer str_LTBL; /*String literal table */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static pBuffer lex_buf;/*pointer to temporary lexeme buffer*/
static pBuffer sc_buf; /*pointer to input source buffer*/
/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int char_class(char c); /* character class function */
static int get_next_state(int, char); /* state machine function */
static int iskeyword(char* kw_lexeme); /*keywords lookup functuion */


/*Initializes scanner */

/****************************************************************************************
Purpose: The purpose of this function is to initialize scanner
Author: Jonathan slaunwhite
Histroy/Versions 1.0  2020-03-22
Called Functions psc_buf,b_rewind,b_clear,
Return Value: EXIT_SUCCESS
****************************************************************************************/
int scanner_init(pBuffer psc_buf) {
	if (b_isempty(psc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	b_rewind(psc_buf);
	b_clear(str_LTBL);
	line = 1;
	sc_buf = psc_buf;
	return EXIT_SUCCESS;/*0*/
/*   scerrnum = 0;  *//*no need - global ANSI C */
}


/****************************************************************************************
Purpose: The purpose of this function is to check the token from input buffer and preform assignments
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-03-22
Called Functions b_getc,b_free,b_retract,b_markc,b_getcoffset,isalpha,isdigit,
Return Value: Void
Algorithum
****************************************************************************************/
Token malar_next_token(void) {
	Token t = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	unsigned char c; /* input symbol */
	int state = 0; /* initial state of the FSM */
	short lexstart;  /*start offset of a lexeme in the input char buffer (array) */
	short lexend;    /*end   offset of a lexeme in the input char buffer (array)*/

//	DECLARE YOUR LOCAL VARIABLES HERE IF NEEDED

	while (1) { /* endless loop broken by token returns it will generate a warning */


		  /*  Special case tokens processed separately one by one
   *  in the token-driven part of the scanner
   *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
   *  white space
   *  !!comment , ',' , ';' , '-' , '+' , '*' , '/', # ,
   *  .AND., .OR. , SEOF,
   */


		c = b_getc(sc_buf);//GET THE NEXT SYMBOL FROM THE INPUT BUFFER

		switch (c) {

			/*case SEOF_T://source end of file

				t.code = SEOF_T;
				t.attribute.seof = SEOF_0;
				return t;*/

		case SEOF://source end of file 

			t.code = SEOF_T;
			t.attribute.seof = SEOF_0;
			return t;

		case ' ':
			continue;

		case TB:
			continue;

		case NL://new line encountered
			line++;//encrement line

			continue;

			//atithmatic operations 
		case '+':
			t.code = ART_OP_T;
			t.attribute.arr_op = PLUS;
			return t;

		case '-':
			t.code = ART_OP_T;
			t.attribute.arr_op = MINUS;
			return t;

		case '*':
			t.code = ART_OP_T;
			t.attribute.arr_op = MULT;
			return t;

		case '/':
			t.code = ART_OP_T;
			t.attribute.arr_op = DIV;
			return t;

		case '=':

			if ((c = b_getc(sc_buf)) == '=') {// comparison operator
				t.code = REL_OP_T;
				t.attribute.rel_op = EQ;
				return t;
			}
			else {

				b_retract(sc_buf);
				t.code = ASS_OP_T;//assignment opperator 
				return t;
			}


		case ',':
			t.code = COM_T;
			return t;


		case ';':
			t.code = EOS_T;
			return t;


		case '!':

			if ((c = b_getc(sc_buf)) == '!') {//if ! is proceeded with !

				c = b_getc(sc_buf);

				while (c != NL) {//while c does not encounter new line
					c = b_getc(sc_buf);

					if (c == SEOF || c == S_EOF) {
						t.code = ERR_T;
						t.attribute.err_lex[0] = c;
						t.attribute.err_lex[1] = SEOF;
						return t;
					}
				}

				line++;
				continue;
			}

			else {//if ! is not proceeded with !

				t.code = ERR_T;
				t.attribute.err_lex[0] = '!';
				t.attribute.err_lex[1] = c;
				t.attribute.err_lex[2] = SEOF;
				while (b_getc(sc_buf) != NL) {}
				line++;
				return t;
			}


		case '.':

			b_markc(sc_buf, b_getcoffset(sc_buf));     //messa changed this
			c = b_getc(sc_buf);

			if (c == 'A' && b_getc(sc_buf) == 'N' && b_getc(sc_buf) == 'D' && b_getc(sc_buf) == '.') {//if AND.

				t.code = LOG_OP_T;
				t.attribute.log_op = AND;
				return t;

			}

			else if (c == 'O' && b_getc(sc_buf) == 'R' && b_getc(sc_buf) == '.') {// if OR.

				t.code = LOG_OP_T;
				t.attribute.log_op = OR;
				return t;

			}

			else {	/* Error */
				b_reset(sc_buf);
				t.code = ERR_T;
				t.attribute.err_lex[0] = '.';
				t.attribute.err_lex[1] = '\0';
				return t;
			}


		case '{':
			t.code = LBR_T;
			return t;

		case '}':
			t.code = RBR_T;
			return t;

		case '(':
			t.code = LPR_T;
			return t;

		case ')':
			t.code = RPR_T;
			return t;

		case '>':

			t.code = REL_OP_T;
			t.attribute.rel_op = GT;
			return t;



		case '<':

			if (c = b_getc(sc_buf) == '>') {//if operator is not equal to comparrason 

				t.code = REL_OP_T;
				t.attribute.rel_op = NE;

				return t;
			}

			/*else if (c == '<') {															DELTE         123123123123

				t.code = SCC_OP_T;
				return t;

			}*/


			else {

				b_retract(sc_buf);
				t.code = REL_OP_T;
				t.attribute.rel_op = LT;

				return t;
			}


		case '#':

			if (c = b_getc(sc_buf) == '#') {//String concatenation
				t.code = SCC_OP_T;
				return t;
			}
			else {//String assignment 

				b_retract(sc_buf);
				t.code = SVID_T;
				return t;
			}

		}//end of switch


			/* Part 2: Implementation of Finite State Machine (DFA)
					   or Transition Table driven Scanner
					   Note: Part 2 must follow Part 1 to catch the illegal symbols
			*/



		if (isdigit(c) || isalpha(c) || c == '"') {//check if digit alphabet or string

			lexstart = b_retract(sc_buf); //reset the mark
			b_markc(sc_buf, lexstart);
			state = 0;//set state to zero

			c = b_getc(sc_buf);

			while (as_table[state] == NOAS) {

				state = get_next_state(state, c); /* get next state */

				if (as_table[state] != NOAS) { /* Break loop since now AS state */
					break;
				}

				c = b_getc(sc_buf);
			}


			if (as_table[state] == ASWR) {//if the state is equal to ASWR

				b_retract(sc_buf);//then retract 
			}

			lexend = b_getcoffset(sc_buf);//get the offset for lexend

			lex_buf = b_allocate((lexend - lexstart) + 1, 0, 'f');//then allocate from lexend end to start

			if (lex_buf == NULL) { /* buffer not created correctly s*/
				scerrnum = 1;//set scerrnum to a non negative number
				aa_func12("RUN TIME ERROR");
			}


			b_reset(sc_buf); /* reset the lexeme to the mark set from before */

			for (int i = lexstart; i < lexend; i++) {//LEXEME BETWEEN lexstart AND lexend
				b_addc(lex_buf, b_getc(sc_buf));
			}

			b_addc(lex_buf, SEOF);//add the seof 
			t = aa_table[state](b_markc(lex_buf, 0));//aa_table function on state
			b_free(lex_buf);//free the lux_buf
			return t;//return t


		}
		else {

			/*Set code an attribute to appropriate values*/
			t.code = ERR_T;
			t.attribute.err_lex[0] = c;
			t.attribute.err_lex[1] = SEOF;
			return t;
		}


	}//end while(1)

	return t;
}


//DO NOT MODIFY THE CODE OF THIS FUNCTION
	//YOU CAN REMOVE THE COMMENTS ONLY

/****************************************************************************************
Purpose: The purpose of this function is to get next state
Author: Jonathan slaunwhite
Histroy/Versions 1.0  2020-03-22
Called Functions
Return Value: next
****************************************************************************************/
int get_next_state(int state, char c) {
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif
	assert(next != IS);

#ifdef DEBUG
	if (next == IS) {
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	return next;
}

/****************************************************************************************
Purpose: The purpose of this function is to set val from corresponding value of passed paremeter
Author: Jonathan slaunwhite
Histroy/Versions 1.0  2020-03-23
Called Functions
Return Value: val
****************************************************************************************/
int char_class(char c) {

	int val;

	if (isalpha(c)) {//if c is a letter in alphebet
		val = 0;
	}
	else if (c == '0') {//if zero

		val = 1;
	}
	else if (isdigit(c)) {//if a number
		val = 2;
	}
	else if (c == '.') {//if a dot
		val = 3;
	}
	else if (c == '#') {//if hash symbol
		val = 4;
	}
	else if (c == '"') {//String 
		val = 6;
	}
	else if (c == SEOF || c == S_EOF) {//if source end of file
		val = 7;
	}
	else {//other option is nothing else matches 
		val = 5;
	}
	return val;
}


/*HERE YOU WRITE THE DEFINITIONS FOR YOUR ACCEPTING FUNCTIONS.*/

 /****************************************************************************************
 Purpose: The purpose of this function is ACCEPTING FUNCTION FOR THE
		  arithmentic variable identifier AND keywords (VID - AVID/KW)
 Author: Jonathan Slaunwhite
 Histroy/Versions 1.0  2020-03-23
 Called Functions none
 Parameters lexeme
 Return Value: t
 ****************************************************************************************/
Token aa_func02(char lexeme[]) {


	Token t;

	int index_Of_KeyWord;
	index_Of_KeyWord = -1;

	for (int i = 0; i < KWT_SIZE; i++) {//loop for the KWT_SIZE size

		if (strcmp(lexeme, kw_table[i]) == 0) {
			index_Of_KeyWord = i;//if keyword found, set it to varible 
		}
	}


	if (index_Of_KeyWord != RT_FAIL_1) {//if no keyward found set to -1 and set code and attribute accordingly
		t.code = KW_T;
		t.attribute.kwt_idx = index_Of_KeyWord;
	}
	else {


		t.code = AVID_T;

		if (strlen(lexeme) > VID_LEN) {//string length greater then vidlen


			for (int i = 0; i < VID_LEN; i++) {
				t.attribute.vid_lex[i] = lexeme[i];//set attribute of vid lex at lexeme at i
			}

			t.attribute.vid_lex[VID_LEN] = SEOF;

		}
		else {

			for (int i = 0; i < strlen(lexeme); i++) {
				t.attribute.vid_lex[i] = lexeme[i];//set attribute of vid lex at lexeme at i
			}

			t.attribute.vid_lex[strlen(lexeme)] = SEOF;

		}//end of else
	}
	return t;

}


/****************************************************************************************
Purpose: The purpose of this function is ACCEPTING FUNCTION FOR THE
		 string variable identifier (VID - SVID)
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-03-23
Called Functions none
Parameters lexeme
Return Value: t
****************************************************************************************/
Token aa_func03(char lexeme[]) {

	Token t;

	t.code = SVID_T;

	if (strlen(lexeme) > VID_LEN) {


		for (unsigned int i = 0; i < VID_LEN; i++) {
			t.attribute.vid_lex[i] = lexeme[i];//put in lexeme vavlue into attribute.vid_lex paremeter
		}

		t.attribute.vid_lex[VID_LEN - 1] = '#'; //append a hash symbol to end of string identifier
		t.attribute.vid_lex[VID_LEN] = SEOF;//add source end of file


	}
	else {

		for (unsigned int i = 0; i < strlen(lexeme); i++) {
			t.attribute.vid_lex[i] = lexeme[i];//add attribute to value of lexeme

		}

		t.attribute.vid_lex[strlen(lexeme)] = SEOF;//add source end of file
	}

	return t;
}


/****************************************************************************************
Purpose: The purpose of this function is ACCEPTING FUNCTION FOR
		THE integer literal(IL) - decimal constant (DIL)
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-03-23
Called Functions none
Parameters lexeme
Return Value: t
****************************************************************************************/
Token aa_func05(char lexeme[]) {

	Token t;


	if (atoi(lexeme) < SHRT_MIN || atoi(lexeme) > SHRT_MAX) {//if integer is not within proper range or integer return error token

		return aa_func12(lexeme);//return error
	}

	//set code and attribute
	t.code = INL_T;
	t.attribute.int_value = atoi(lexeme);

	return t;
}




/****************************************************************************************
Purpose: The purpose of this function is ACCEPTING FUNCTION FOR
		 THE floating-point literal (FPL)
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-03-23
Called Functions none
Parameters lexeme
Return Value: t
****************************************************************************************/
Token aa_func08(char lexeme[]) {


	Token t;

	if ((atof(lexeme) >= 0 && strlen(lexeme) > 32) && (atof(lexeme) < FLT_MIN || atof(lexeme) > FLT_MAX) || atof(lexeme) < 0) {//check if the value is within specified expected range
		return aa_func12(lexeme);
	}

	//set code and attribute accordingly
	t.code = FPL_T;
	t.attribute.flt_value = (float)atof(lexeme);

	return t;
}


/****************************************************************************************
Purpose: The purpose of this function is ACCEPTING FUNCTION FOR THE string literal(SL)
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-03-23
Called Functions none
Parameters lexeme
Return Value: t
****************************************************************************************/
Token aa_func10(char lexeme[]) {

	Token t;

	t.attribute.str_offset = b_limit(str_LTBL);

	t.code = STR_T;

	for (unsigned int i = 0; i < strlen(lexeme); i++) {


		if (lexeme[i] != '"') //ignore string quotation marks
			b_addc(str_LTBL, lexeme[i]);

		if (lexeme[i] == NL) //if encounter new line increment counter
			line++;
	}

	b_addc(str_LTBL, SEOF);

	return t;
}


/****************************************************************************************
Purpose: The purpose of this function is ACCEPTING FUNCTION FOR THE ERROR TOKEN
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-03-23
Called Functions none
Parameters lexeme
Return Value: t
****************************************************************************************/
Token aa_func12(char lexeme[]) {

	Token t;

	t.code = ERR_T;

	if (strlen(lexeme) > ERR_LEN) {//check the length of passed parameter

		for (int i = 0; i < ERR_LEN - 3; i++) { /* add lexeme up to error length -3 */

			if (lexeme[i] == NL) {//if encountered new line increment
				line++;
			}

			t.attribute.err_lex[i] = lexeme[i];//set attribute from lexeme index value
		}
		for (int i = ERR_LEN - 3; i < ERR_LEN; i++) {

			t.attribute.err_lex[i] = '.';//add dot to last 3 postions in attribute
		}

		t.attribute.err_lex[ERR_LEN] = SEOF; //add SEOF to end of attrbute
	}
	else {
		for (int i = 0; i < strlen(lexeme); i++) { /* add lexeme to err_lex */

			if (lexeme[i] == NL) {
				line++;
			}

			t.attribute.err_lex[i] = lexeme[i];
		}
		t.attribute.err_lex[strlen(lexeme)] = SEOF; /* add SEOF */
	}

	return t;
}

//HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).FOR EXAMPLE

//unsued isKeyword function that was not implimented in a function
int iskeyword(char* kw_lexeme) {}


//warning for unsued function that will be used in later use
Token aa_func11(char lexeme[]) {}