/****************************************************************************************
 *  File name: parser.c
 *  Compiler Visual Studio 2019
 *  Author: Jonathan Slaunwhite , 040939090
 *  Course:CST 8152 –Compilers, Lab Section:013
 *  Assignment 3
 *  Date: 09 April 2020
 *  Professor: Sv.Ranev
 *  Purpose:This is the parcer for assignment 3 
 *  Version: 1.20.1
 *  Function List 
 *
 ****************************************************************************************/


#include "parser.h"

 /****************************************************************************************
 Purpose: 
 Author: Jonathan Slaunwhite
 Histroy/Versions 1.0  2020-04-90
 Called Functions 
 Parameters none
 Return Value: none
 Algorithum: 
 ****************************************************************************************/
void parser(void) {
	lookahead = malar_next_token();
	program(); match(SEOF_T, NO_ATTR);
	gen_incode("PLATY: Source file parsed");
}/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions 
Parameters pr_token_code,pr_token_attribute
Return Value: none
Algorithum:
****************************************************************************************/void match(int pr_token_code, int pr_token_attribute) {	/* make sure the lookahead.code at this location is the proper code */
	if (lookahead.code != pr_token_code) {
		syn_eh(pr_token_code);
		return;
	}
	/*If the match is successful and the lookahead is SEOF_T, the function returns*/
	if (lookahead.code == SEOF_T) {
		return;
	}	switch (pr_token_attribute) {	case KW_T:		if (pr_token_attribute != lookahead.attribute.kwt_idx) {			syn_eh(pr_token_code);			return;		}		break;	case LOG_OP_T:		if (pr_token_attribute != lookahead.attribute.log_op) {			syn_eh(pr_token_code);			return;		}		break;	case ART_OP_T:		if (pr_token_attribute != lookahead.attribute.arr_op) {
			syn_eh(pr_token_code);
			return;
		}		break;	case REL_OP_T:		if (pr_token_attribute != lookahead.attribute.rel_op) {
			syn_eh(pr_token_code);
			return;		}			break;	default:		break;		}//end of switch		lookahead = malar_next_token();		if (lookahead.code == ERR_T) {//If the new lookahead token is ERR_T			syn_printe();			malar_next_token();// advances to the next input token 			synerrno++;			return;		}	}//end of function /****************************************************************************************
 Purpose:
 Author: Jonathan Slaunwhite
 Histroy/Versions 1.0  2020-04-90
 Called Functions
 Parameters sync_token_code
 Return Value: none
 Algorithum:
 ****************************************************************************************/	void syn_eh(int sync_token_code) {		syn_printe();		synerrno ++ ;//increment error counter		do {			lookahead = malar_next_token();		
			if (sync_token_code == lookahead.code) {
				if (sync_token_code != SEOF_T) 
					lookahead = malar_next_token();
				return;
			}			if (lookahead.code == SEOF_T) {//if seof_t is reached return exit(synerrno)
				exit(synerrno);
				return;
			}		} while (sync_token_code != lookahead.code);	}	/****************************************************************************************
	Purpose:
	Author: Sv.Ranev
	Histroy/Versions 1.0  2020-04-90
	Called Functions
	Parameters none
	Return Value: none
	Algorithum:
	****************************************************************************************/
	void syn_printe() {
		Token t = lookahead;

		printf("PLATY: Syntax error:  Line:%3d\n", line);
		printf("*****  Token code:%3d Attribute: ", t.code);
		switch (t.code) {
		case  ERR_T: /* ERR_T     0   Error token */
			printf("%s\n", t.attribute.err_lex);
			break;
		case  SEOF_T: /*SEOF_T    1   Source end-of-file token */
			printf("SEOF_T\t\t%d\t\n", t.attribute.seof);
			break;
		case  AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
		case  SVID_T:/* SVID_T    3  String Variable identifier token */
			printf("%s\n", t.attribute.vid_lex);
			break;
		case  FPL_T: /* FPL_T     4  Floating point literal token */
			printf("%5.1f\n", t.attribute.flt_value);
			break;
		case INL_T: /* INL_T      5   Integer literal token */
			printf("%d\n", t.attribute.get_int);
			break;
		case STR_T:/* STR_T     6   String literal token */
			printf("%s\n", b_markc(str_LTBL, t.attribute.str_offset));
			break;

		case SCC_OP_T: /* 7   String concatenation operator token */
			printf("NA\n");
			break;

		case  ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */
			printf("NA\n");
			break;
		case  ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */
			printf("%d\n", t.attribute.get_int);
			break;
		case  REL_OP_T: /*REL_OP_T  10   Relational operator token */
			printf("%d\n", t.attribute.get_int);
			break;
		case  LOG_OP_T:/*LOG_OP_T 11  Logical operator token */
			printf("%d\n", t.attribute.get_int);
			break;

		case  LPR_T: /*LPR_T    12  Left parenthesis token */
			printf("NA\n");
			break;
		case  RPR_T: /*RPR_T    13  Right parenthesis token */
			printf("NA\n");
			break;
		case LBR_T: /*    14   Left brace token */
			printf("NA\n");
			break;
		case RBR_T: /*    15  Right brace token */
			printf("NA\n");
			break;

		case KW_T: /*     16   Keyword token */
			printf("%s\n", kw_table[t.attribute.get_int]);
			break;

		case COM_T: /* 17   Comma token */
			printf("NA\n");
			break;
		case EOS_T: /*    18  End of statement *(semi - colon) */
			printf("NA\n");
			break;
		default:
			printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
		}/*end switch*/
	}/* end syn_printe()*/


	void gen_incode(char* message) {

		printf("%s", message);

	}

/****************************************************************************************
Purpose:
Author: Sv.Ranev
Histroy/Versions 1.0  2020-04-90
Called Functions
Parameters none
Return Value: none
Algorithum:
****************************************************************************************/
	void program() {//<program> ->PLATYPUS{ <opt_statements> }
		match(KW_T, PLATYPUS); //PLATYPUS
		match(LBR_T, NO_ATTR); //{
		opt_statements();//opt_statements
		match(RBR_T, NO_ATTR);//}
		gen_incode("PLATY: Program parsed");
	}	/****************************************************************************************
	Purpose:
	Author: Sv.Ranev
	Histroy/Versions 1.0  2020-04-90
	Called Functions
	Parameters none
	Return Value: none
	Algorithum:
	****************************************************************************************/
	void opt_statements() {//<opt_statements> -> <statements> | e
		switch (lookahead.code) {
		case AVID_T:
		case SVID_T:
			statements();
			break;
		case KW_T:
			/* check for IF,WHILE,READ,WRITE in statements_p() as well*/
			if (lookahead.attribute.get_int == IF
				|| lookahead.attribute.get_int == WHILE
				|| lookahead.attribute.get_int == READ
				|| lookahead.attribute.get_int == WRITE) {
				statements();
				break;
			}
		default: /*empty string – optional statements*/;
			gen_incode("PLATY: Opt_statements parsed");
		}
	}/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions statement, statements
Parameters none
Return Value: none
Algorithum: <program>  ->
  	PLATYPUS {<opt_statements>} 
****************************************************************************************/	void statements() {	//<statements> -> <statement><statements'> 		statement();		statements();	}	
	//t
/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions statement, statements_p
Parameters none
Return Value: none
Algorithum: FIRST(<statements’>) = { AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(READ), 
KW_T(WRITE) , e }

****************************************************************************************/
	void statements_p() {/* <statements'> -> <statement><statements'> | e */


		switch (lookahead.code) {

			switch (lookahead.code) {
			case KW_T:
				switch (lookahead.attribute.kwt_idx) {
				case IF:
				case WHILE:
				case READ:
				case WRITE:
					return;
				default:
					break;
				}
			case AVID_T:
			case SVID_T:
				statement();	
				statements_p();
				break;
			}

		}

	}



/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions statement, statements_p
Parameters none
Return Value: none
Algorithum: <statement> ->
  <assignment statement>
| <selection statement>
| <iteration statement>
 	| <input statement>
| <output statement>

****************************************************************************************/
	void statement() {//<statement> -><assignment statement>| <selection statement>| <iteration statement>	| <input statement>| <output statement>


		switch (lookahead.code) {

		case  AVID_T:
		case SVID_T: 

			assignment();
			break;


		case KW_T:

			switch (lookahead.attribute.kwt_idx) {

			case IF:	
				selection();
				break;
			case WHILE:	
				iteration();
				break;
			
			case READ:	
				input();
				break;
			
			case WRITE:
				output();
				break;
		
			default:
				syn_printe();

			}
			break;
		default:
			syn_printe();

		}//end of switch

	}

/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions assignment_expression, match,gen_incode
Parameters none
Return Value: none
Algorithum: FIRST(<assignment statement>) = { FIRST(<assignment expression>) } 
                                                     = { AVID_T, SVID_T }
****************************************************************************************/
	void assignment() {//<assignment statement> -> <assignment expression>;


		assignment_expression();//assignment expression
		match(EOS_T, NO_ATTR);	//;
		gen_incode("PLATY: Assignment statement parsed");

	}


/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions string, match,gen_incode,syn_printe
Parameters none
Return Value: none
Algorithum: IRST(<assignment expression>) = { FIRST(<assignment expression>)
 = { AVID_T, SVID_T}
****************************************************************************************/
	void assignment_expression() {//assignment statement  -> AVID_T | SVID_T }


		switch (lookahead.code) {
		
		case AVID_T:	//AVID_T |
			match(AVID_T, NO_ATTR);
			match(ASS_OP_T, EQ);
			arithmetic();
			gen_incode("PLATY: Assignment expression arithmetic parsed\n");
		    break;

		case SVID_T:	//| SVID_T
			match(SVID_T, NO_ATTR);
			match(ASS_OP_T, EQ);
			string();	
			gen_incode("PLATY: Assignment expression string parsed");
			break;


		default:	
			syn_printe();
			break;
		}
	}

/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions pre_condition, match,gen_incode,opt_statements
Parameters none
Return Value: none
Algorithum:FIRST(<selection statement>) = { KW_T(IF) }
****************************************************************************************/
	void selection() {//<selection statement> ->IF <pre - condition>(<conditional expression>) THEN { <opt_statements> }ELSE{ <opt_statements> };

		match(KW_T, IF);
		pre_condition();
		match(LPR_T, NO_ATTR);//(
		conditional();
		match(RPR_T,NO_ATTR);//)
		match(KW_T, THEN);
		match(LBR_T, NO_ATTR);//{
		opt_statements();
		match(RBR_T, NO_ATTR);//}
		match(KW_T,ELSE);
		match(LBR_T, NO_ATTR);//{
		opt_statements();
		match(RBR_T, NO_ATTR);//}
		match(EOS_T, NO_ATTR);
		gen_incode("PLATY: Selection statement parsed");
	}

/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions pre_condition, match,conditional,statementsconditional
Parameters none
Return Value: none
Algorithum: FIRST(<iteration statement>) = { KW_T(WHILE) }
****************************************************************************************/
	void iteration() {//<iteration statement> ->WHILE <pre - condition>(<conditional expression>)	REPEAT {<statements>
		

		match(KW_T, WHILE);
		pre_condition();
		match(LPR_T, NO_ATTR);//(
		conditional();
		match(RPR_T, NO_ATTR);//)
		match(KW_T, REPEAT);
		match(LBR_T, NO_ATTR);//{
		statements();
		match(RBR_T, NO_ATTR);//}
		match(EOS_T, NO_ATTR);//;
	}


/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions syn_printe
Parameters none
Return Value: none
Algorithum: FIRST(<pre-condition>) = { KW_T(TRUE),KW_T(FALSE) }
****************************************************************************************/
	void pre_condition() {//<pre-condition> ->	TRUE | FALSE

		if (lookahead.code == KW_T) {

			if (lookahead.attribute.kwt_idx == TRUE) {
				match(KW_T, TRUE);
			}
			else if (lookahead.attribute.kwt_idx == FALSE) {
				match(KW_T, FALSE);
			}
			else {
				syn_printe();
			}


		}
		else {
			syn_printe();
		}

	}


/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions match,variable_list,gen_incode
Parameters none
Return Value: none
Algorithum: FIRST(<input statement>) = { KW_T(READ) }
****************************************************************************************/
	void input() {//<input statement> ->READ(<variable list>);

		match(KW_T, READ);
		match(LPR_T, NO_ATTR);//(
		variable_list();
		match(RPR_T, NO_ATTR);//)
		match(EOS_T, NO_ATTR);//;
		gen_incode("PLATY: Input statement parsed");
	}


/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions variable_identifier,variable_list_p,gen_incode
Parameters none
Return Value: none
Algorithum: FIRST(<variable list>) = { FIRST(<variable identifier>) } = { AVID_T, SVID_T } 
****************************************************************************************/
	void variable_list() {//<variable list> ->	<variable identifier> | <variable list>, <variable identifier>
		
		variable_identifier();
		variable_list_p();
		gen_incode("PLATY: Variable list parsed");

	}



/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions variable_identifier,variable_identifier,variable_list_p
Parameters none
Return Value: none
Algorithum: <variable list’> -> 
, <variable identifier><variable list’> | e
****************************************************************************************/
	void variable_list_p() {

		if (lookahead.code==COM_T) {

			match(COM_T, NO_ATTR);
			variable_identifier();
			variable_list_p();

		}

	}



/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions 
Parameters none
Return Value: none
Algorithum: FIRST(<variable identifier>) = { FIRST(<variable identifier>) } = { AVID_T, SVID_T } 

****************************************************************************************/
	void variable_identifier() {//<variable identifier> -> AVID_T | SVID_T 

		if (lookahead.code == AVID_T) {
			match(AVID_T, NO_ATTR);
		}
		else if (lookahead.code == SVID_T) {
			match(SVID_T, NO_ATTR);
		}
		else {
			syn_printe();
		}



	}


/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions match,output_statement,gen_incode
Parameters none
Return Value: none
Algorithum:FIRST(<output statement>) = { KW_T(WRITE) } 

****************************************************************************************/
	void output() {//<output statement> ->WRITE(<output statement’>


		match(KW_T, WRITE);		
		match(LPR_T, NO_ATTR);//(
		output_statement();
		match(RPR_T, NO_ATTR);//)
		match(EOS_T, NO_ATTR);
		gen_incode("PLATY: Output statement parsed\n");

	}

	/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions variable_list,match,gen_incode
Parameters none
Return Value: none
Algorithum:FIRST (<output_list>) = { FIRST (<opt_variable list >) , STR_T} 
= {AVID_T, SVID_T, STR_T, e } 

****************************************************************************************/
	void output_list() {//output_list ->AVID_T, SVID_T, STR_T, E


		switch (lookahead.code) {

		case AVID_T:
		case SVID_T:
			variable_list();
			break;

		case STR_T:

			match(STR_T, NO_ATTR);
			gen_incode("PLATY: Output list string literal parsed");
			break;

		default:
			gen_incode("PLATY: Output list empty parsed");
			break;

		}


	}


/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions variable_list,match,gen_incode
Parameters none
Return Value: none
Algorithum:FIRST(<arithmetic expression>) = { FIRST(<unary arithmetic expression>, 

<arithmetic expression>) = {ART_OP_T(MINUS) , ART_OP_T(PLUS) AVID_T, FPL_T, INL_T, LPR_T}

****************************************************************************************/
	void arithmetic() {//<arithmetic expression> -><unary arithmetic expression>| <additive arithmetic expression>



		switch (lookahead.code) {
		case AVID_T:
		case FPL_T:
		case INL_T:
		case LPR_T:
			additive_arithmetic();
			break;

		case ART_OP_T:
			break;
		default:
			syn_printe();
		}


		gen_incode("PLATY: arithmetic expression parsed");

	}


/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions match,primary_arithmetic,gen_incode,syn_printe
Parameters none
Return Value: none
Algorithum:FIRST(<unary arithmetic expression>) = {ART_OP_T(MINUS) , ART_OP_T(PLUS) }

****************************************************************************************/
	void unary_arithmetic() {//<arithmetic expression> - ><unary arithmetic expression>| <additive arithmetic expression>

		switch (lookahead.code) {

		case ART_OP_T:
			match(lookahead.code, lookahead.attribute.arr_op);
			primary_arithmetic();
			gen_incode("PLATY: Unary arithmetic expression parsed");

		default:
			syn_printe();
			return;

		}

	}

	/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions multiplicative_arithmetic,additive_arithmetic_p
Parameters none
Return Value: none
Algorithum:FIRST(<additive arithmetic expression>) = {FIRST(<multiplicative arithmetic expression>)} 
= { AVID_T, FPL_T, INL_T, LPR_T } 

****************************************************************************************/
	void additive_arithmetic() {//<additive arithmetic expression> -> <multiplicative arithmetic expression> <additive arithmetic expression’>

		multiplicative_arithmetic();
		additive_arithmetic_p();

	}

/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions match,multiplicative_arithmetic,additive_arithmetic_p,gen_incode
Parameters none
Return Value: none
Algorithum:<additive arithmetic expression’> -> 
+ <multiplicative arithmetic expression> <additive arithmetic expression’> 
| - <multiplicative arithmetic expression> <additive arithmetic expression’> 
| e

****************************************************************************************/
	void additive_arithmetic_p() {//FIRST(<additive arithmetic expression’>) = { ART_OP_T(MINUS), ART_OP_T(PLUS), e }


		switch (lookahead.code) {

		case ART_OP_T:

			switch (lookahead.attribute.arr_op) {

			case PLUS:
				match(ART_OP_T, PLUS);
				multiplicative_arithmetic();
				additive_arithmetic_p();
				break;

			case MINUS:
				match(ART_OP_T, MINUS);		
				multiplicative_arithmetic();	
				additive_arithmetic_p();		
				break;

			default:
				return;




			}
			gen_incode("PLATY: Additive arithmetic expression parsed");

		}

	}


/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions primary_arithmetic,multiplicative_arithmetic
Parameters none
Return Value: none
Algorithum:FIRST (<multiplicative arithmetic expression>) = { AVID_T, FPL_T, INL_T, LPR_T }

****************************************************************************************/
	void multiplicative_arithmetic() {//<multiplicative arithmetic expression> -><primary arithmetic expression><multiplicative arithmetic expression>


		primary_arithmetic();
		multiplicative_arithmetic();

	}



	/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions
Parameters none
Return Value: none
Algorithum:<multiplicative arithmetic expression’> ->
	*<primary arithmetic expression><multiplicative arithmetic expression>
	|/<primary arithmetic expression><multiplicative arithmetic expression>
	| e
****************************************************************************************/
	void  multiplicative_arithmetic_p() {//FIRST (<multiplicative arithmetic expression’>) = { ART_OP_T(MULT), ART_OP_T(DIV), e }



		if (lookahead.attribute.arr_op == DIV) {

			match(ART_OP_T, DIV);
			primary_arithmetic();
			multiplicative_arithmetic();

		}
		else if (lookahead.attribute.arr_op==MULT) {
			match(ART_OP_T, MULT);
			primary_arithmetic();
			multiplicative_arithmetic();
		}
		else {
			return;
		}


		gen_incode("PLATY: Multiplicative arithmetic expression parsed");

	}




	/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions syn_printe,gen_incode
Parameters none
Return Value: none
Algorithum: FIRST(<primary a_relational expression>) = { AVID_T, FPL_T, INL_T }	
****************************************************************************************/
	void primary_a_relational() {//<primary a_relational expression> ->AVID_T| FPL_T| INL_T

		switch (lookahead.code) {

		case AVID_T://AVID_T or 
			match(AVID_T, NO_ATTR);
			break;

		case FPL_T://fpl_T or 
			match(FPL_T, NO_ATTR);
			break;
		case INL_T://inl_t  or 
			match(INL_T, NO_ATTR);
			break;

		default:

			syn_printe();

		}

		gen_incode("PLATY: Primary a_relational expression parsed");
	}



/****************************************************************************************
Purpose:
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-04-90
Called Functions primary_string,gen_incode
Parameters none
Return Value: none
Algorithum:FIRST(<primary s_relational_expression>) = { FIRST(<primary string expression> } 
= { SVID_T, STR_T }

****************************************************************************************/
	void primary_s_relational() {//primary s_relational expression> -><primary string expression>

		primary_string();//primary string expression
		gen_incode("PLATY: Primary s_relational expression parsed");
	}