/****************************************************************************************
 *  File name: Buffer.c
 *  Compiler Visual Studio 2019
 *  Author: Jonathan Slaunwhite , 040939090
 *  Course:CST 8152 –Compilers, Lab Section:013
 *  Assignment 1
 *  Date: 30 January 2020
 *  Professor: Sv.Ranev
 *  Purpose:This is the main program for Assignment #1, CST8152
 *  Version: 1.20.1
 *  Function list: b_allocate,b_addc,b_clear, b_free, b_isfull
 *	 b_limit, b_capacity, b_markc, b_mode, b_incfactor
 *	 b_load, b_isempty, b_getc, b_eob, b_print, b_compact
 *	 b_rflag, b_retract, b_reset, b_getcoffset, b_rewind
 *
 ****************************************************************************************/

#include <stdio.h>
#include "buffer.h"

 /****************************************************************************************
 Purpose: The purpose of this function is to create a new buffer
 Author: Jonathan Slaunwhite
 Histroy/Versions 1.0  2020-01-30
 Called Functions free, malloc, calloc , sizeof
 Parameters short init_capacity, char inc_factor, char o_mode
 Return Value: buffer, if fail NULL
 Algorithum: Allocate memory for one buffer structure. Then alocate memory for a dynamic character buffer.
			 Then proceeds to set inc factor and mode to corrensponding values based on its o_mode and or include inc_facotr
			 Then proceeds to return pointer to the buffer structure
 ****************************************************************************************/
Buffer* b_allocate(short init_capacity, char inc_factor, char o_mode) {


	Buffer* ptr;//buffer 
	ptr = NULL;//initilize pointer to null

	ptr = (Buffer*)calloc(1, sizeof(Buffer));//calloc memory for the buffer pointer


	//check for invalid states in init_cap and ptr

	if (init_capacity < 0 || init_capacity > Max_Value) {//if capacity is within given range
		return NULL;
	}

	if (ptr == NULL) {//if pointer is equal to null

		free(ptr);
		return NULL;
	}


	if (init_capacity == 0) {//if int_cap is equal to 0

		ptr->cb_head = (char*)malloc(sizeof(char) * DEFAULT_INIT_CAPACITY);


		if (ptr->cb_head == NULL) {//check if ptr is equal to null
			free(ptr->cb_head);
			free(ptr);
			return NULL;

		}


		/*
		Check if the 0_mode matched any of the case statements below
		and set the mode and inc factor accordingly
		*/


		if (o_mode == 'f') {//if user puts in f mode
			ptr->inc_factor = 0;
			ptr->mode = f_mode;
		}

		if (o_mode == 'm') {//if user puts in m mode
			ptr->inc_factor = DEFAULT_INC_FACTOR;
			ptr->mode = m_mode;
		}

		if (o_mode == 'a') {//if user puts in a mode
			ptr->inc_factor = DEFAULT_INC_FACTOR;
			ptr->mode = a_mode;
		}


		ptr->capacity = DEFAULT_INIT_CAPACITY;//set the capaicty 
	}


	else {//if its not equal to 0 proceed here

		ptr->cb_head = (char*)malloc(sizeof(char) * init_capacity);//molloc the ptr to head

		if (ptr->cb_head == NULL) {//if allocation has failed
			free(ptr->cb_head);
			free(ptr);
			return NULL;
		}

		if ((unsigned char)inc_factor == 0) {//if in fixed size mode proceed below
			ptr->mode = f_mode;
			ptr->inc_factor = 0;
		}



		/*
		Check the 0_mode character, along with inc factor value for conditional assignment of mode and inc_factor
		*/

		if (o_mode == 'a' && (unsigned char)inc_factor >= 1 && (unsigned char)inc_factor <= 255) {//a mode with parameters
			ptr->mode = a_mode;
			ptr->inc_factor = (unsigned char)inc_factor;
		}


		if (o_mode == 'm' && (unsigned char)inc_factor >= 1 && (unsigned char)inc_factor <= 100) {//m mode in paremters
			ptr->mode = m_mode;
			ptr->inc_factor = (unsigned char)inc_factor;
		}

		if (o_mode == 'f') {//if in f mode
			ptr->mode = f_mode;
			ptr->inc_factor = 0;
		}

		ptr->capacity = init_capacity;//set the capacity
	}


	ptr->flags |= DEFAULT_FLAGS;//set the flags accordingly

	return ptr;
}

/****************************************************************************************
Purpose: The purpose of this function is to set flags accordingly and add size and symbol to buffer
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters pBuffer const pBD, char symbol
Return Value: pBD, if error NULL
Algorithum: set flag to 0, proceed to check mode and make a new capactiy with 3 step formula,
			Then return pointer the buffer.
****************************************************************************************/
pBuffer b_addc(pBuffer const pBD, char symbol) {

	char* temp_ptr;//a temp pointer 

	char* current_used_buffer;//current buffer

	short inc_factor_n;//the new inc factor

	short new_cap;//the new capacity to be used


	short avalible_memory;//avalible memory


	pBD->flags &= RESET_R_FLAG;//set the flags reset

	new_cap = 0;//set new cap to initilized 0 value

	if (pBD->addc_offset == pBD->capacity) {//if addc_offset is equal to capacity


		if (pBD->mode == f_mode) {//if the mode is equal to fixed mode reutrn null
			return NULL;
		}


		if (pBD->mode == a_mode) {//if add mode 

			new_cap = pBD->capacity + (unsigned char)pBD->inc_factor;//set new capacity

			if (new_cap > 0 && new_cap > Max_Value) {//check for new capacity if it is within specifed paremeters
				new_cap = Max_Value;
			}

			if (new_cap < 0) {//check if the new capacity is less then zero
				return NULL;
			}




		}

		if (pBD->mode == m_mode) {//if in m mode


			if (pBD->capacity == Max_Value) {//if new cap if equal to max value

				return NULL;

			}


			/*THree step algorithum provided processed here*/
			avalible_memory = Max_Value - pBD->capacity;
			inc_factor_n = avalible_memory * pBD->inc_factor / 100;//increment facotr is set 
			new_cap = pBD->capacity + inc_factor_n;//new cap is set


			if (inc_factor_n == 0 && pBD->capacity < Max_Value) {//check if int factor and capacity are within specified range
				new_cap = Max_Value;
			}

			if (new_cap < 0 || new_cap > Max_Value) {//if new cap is not within range return null
				return NULL;
			}
		}



		current_used_buffer = pBD->cb_head;

		temp_ptr = (char*)realloc(pBD->cb_head, sizeof(char) * new_cap);


		if (temp_ptr == NULL) {
			return NULL;
		}



		if (current_used_buffer != temp_ptr) {
			pBD->cb_head = temp_ptr;
			pBD->flags |= SET_R_FLAG;
		}

		pBD->capacity = new_cap;//set capacity 
	}


	pBD->cb_head[pBD->addc_offset++] = symbol;//add symbol 

	return pBD;
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to re-initalizes data members of buffer structure
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: 0, if fail -1
*****************************************************************************************************************/
int b_clear(Buffer* const pBD) {

	if (!pBD) {

		return RT_FAIL_1;
	}
	/*set flags to 0 though bit masking*/
	pBD->flags = pBD->flags & DEFAULTZEROS;
	pBD->flags = pBD->flags | DEFAULT_FLAGS;

	/*reset contents of the buffer descriptors*/
	pBD->addc_offset = reset;
	pBD->getc_offset = reset;
	pBD->markc_offset = reset;

	return 1;/*1 =true who knows?*/
}

/*****************************************************************************************************************
Purpose: The purpose of this function is tofree memory occupied by character buffer
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions free()
Parameters Buffer* const pBD
Return Value: void
*****************************************************************************************************************/
void b_free(Buffer* const pBD) {

	/*free character buffer */
	free(pBD->cb_head);
	pBD->cb_head = NULL;/*make is point to null */
	free(pBD);/*free buffer*/
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to return 1 if character buffer is full
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: int 0 if fail, -1 or 1
*****************************************************************************************************************/
int b_isfull(Buffer* const pBD) {

	/*If run time error return -1*/
	if (!pBD) {
		return RT_FAIL_1;
	}
	/*The buffer is ful wehn addc_offset measured in bytes is equals to CAPACITY*/
	if (pBD->addc_offset == pBD->capacity) {

		return 1;
	}
	return 0; /*If character buffer is not full then return 0*/
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to return current limit of character buffer
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: addc_offset, if fail -1
*****************************************************************************************************************/
short b_limit(Buffer* const pBD) {

	if (pBD == NULL) {
		return RT_FAIL_1;
	}

	/*capacity is the current total size (measured in bytes) of the memory allocated for the character array*/
	return pBD->addc_offset;
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to return the current limit of character buffer
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: capacity, if error then -1
*****************************************************************************************************************/
short b_capacity(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_1;
	}

	/* Capactiy -In the text below it is referred also as current capacity*/
	return pBD->capacity;
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to set marc)offset to mark
Author: Jonathan Slaunwhite
HistroyVersions 1.0  2020-01-30
Called Functions
Parameters pBuffer const pBD, short mark
Return Value:markc_offset, if fail NULL
Algorithum:
*****************************************************************************************************************/
char* b_markc(pBuffer const pBD, short mark) {

	if (!pBD) {
		return NULL;
	}

	/*if mark is not within the range of 0 and addc_offset return null*/
	if (mark < 0 || mark > pBD->addc_offset) {

		return NULL;
	}
	//if mark is not within the limits of the 0 to addc_offset

	/*set marc_offset equal to the mark*/
	pBD->markc_offset = mark;

	/*return a pointer to marc_offset */
	return (char*)pBD->cb_head + pBD->markc_offset;

}

/*****************************************************************************************************************
Purpose: The purpose of this function is to return mode to calling funciton
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: mode, if fail NULL
*****************************************************************************************************************/
int b_mode(Buffer* const pBD) {

	if (!pBD) {
		return 0;
	}
	return pBD->mode;/*Returns the value of mode*/
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to return non negative value of inc_factor
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: inc_factor, if fail 0x100
Algorithum:
*****************************************************************************************************************/
size_t b_incfactor(Buffer* const pBD) {

	if (!pBD) {
		return  b_incFactor_return;/*returns 0x100 for run time error*/
	}

	return (unsigned char)pBD->inc_factor;/*return non negative inc_factor*/
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters  FILE* const fi, Buffer* const pBD
Return Value: if fail -1,else,   else counter
*****************************************************************************************************************/
int b_load(FILE* const fi, Buffer* const pBD) {

	int counter = 0;/*will be used to count how many times a character is read*/
	char buffer;

	if (!pBD || fi == NULL) {
		return  RT_FAIL_1;/*returns -1 for run time error*/
	}
	while (!feof(fi)) {

		buffer = (char)fgetc(fi);/*read one character at a time*/


		if (feof(fi)) {/*detect end of line then break from loop*/
			break;
		}
		if (b_addc(pBD, buffer) == NULL) {

			ungetc(buffer, fi);/*inputting back into file buffer */
			return RT_FAIL_2;
		}
		counter++;
	}
	return counter;
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to return 1 if addc_offset is 0 otherwise return 0
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: -1 if fail, 1 if pass, else 0:
*****************************************************************************************************************/
int b_isempty(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_1;
	}

	if (pBD->addc_offset == 0) {

		return 1;
	}
	else {
		return 0;
	}
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to read the buffer
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: character at getc_offset
*****************************************************************************************************************/
char b_getc(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_2;
	}

	if (pBD->getc_offset == pBD->addc_offset) {/*if adcdc_offset is equal to addc_*/

		pBD->flags = pBD->flags | SET_EOB;/*set eob bit to 1  */

		return 0;
	}

	pBD->flags = pBD->flags & RESET_EOB;/*change eob bit back to 0*/

	//pBD->getc_offset++;

	return pBD->cb_head[pBD->getc_offset++];;
}
/*****************************************************************************************************************
Purpose: The purpose of this function is to return value of flags detaermined only by eob bit
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: flags, if fail -1
*****************************************************************************************************************/
int b_eob(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_1;
	}
	return pBD->flags & CHECK_EOB;

}
/*****************************************************************************************************************
Purpose: The purpose of this function is to print character by character contents of character buffer
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD, char nl
Return Value: count, if fail -1
*****************************************************************************************************************/
int b_print(Buffer* const pBD, char nl) {
	int count = 0;/*will count how many times a character is read*/
	char character;/*will hold contents of character buffer character by character*/

	if (!pBD) {
		return RT_FAIL_1;
	}

	character = b_getc(pBD);

	while (!b_eob(pBD)) {

		if (b_eob(pBD)) {/*reached end of buffer */
			break;
		}
		printf("%c", (char)character);
		count++;
		character = b_getc(pBD);
	}



	if (nl != 0) {/*if nl is not 0 print new line character */
		printf("\n");
	}
	return count;
}
/*****************************************************************************************************************
Purpose: The purpose of this function is to
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions relloc, sizeof
Parameters Buffer* const pBD, char symbol
Return Value:pBD, if error then NULL
Algorithum:
*****************************************************************************************************************/
Buffer* b_compact(Buffer* const pBD, char symbol) {

	short newCapacity;/*this will be the new capacity plus 1*/
	char* temp;/*temp pointer used for holding the character buffers new capacity*/

	if (!pBD) {
		return NULL;
	}
	/*make new capacity equal to next avalbie space in buffer */
	newCapacity = pBD->addc_offset + 1;

	if (newCapacity <= 0) {
		return NULL;
	}
	/* reallocating character buffer to new capacity */
	temp = realloc(pBD->cb_head, sizeof(char) * newCapacity);

	if (!temp) {
		return NULL;
	}

	if (temp != pBD->cb_head) {
		/*completly reollcatd buffer to new size*/
		pBD->flags = pBD->flags | SET_R_FLAG;

	}
	/*setting cb_head = to the temp pointer*/
	pBD->cb_head = temp;
	/*set capactiy to the new capactiy */
	pBD->capacity = newCapacity;
	/*putting symbol into next avalible space */
	pBD->cb_head[pBD->addc_offset] = symbol;
	/*increment addc_offset by one for next avalble symbol */
	pBD->addc_offset++;

	return pBD;
}

/*****************************************************************************************************************
Purpose: The purpose of this function is toreturn the value of the glags determined only by the r_flag bit
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: flags
Algorithum:
*****************************************************************************************************************/
char b_rflag(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_1;
	}
	/*return flags r flag bit only*/
	return pBD->flags & CHECK_R_FLAG;
}
/*****************************************************************************************************************
Purpose: The purpose of this function is to decrements getc_offset by 1
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: getc_offset, if fail -1
*****************************************************************************************************************/
short b_retract(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_1;
	}
	if (pBD->getc_offset == 0) {
		return RT_FAIL_1;
	}
	pBD->getc_offset--;

	return pBD->getc_offset;
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to set getc_offset to value of current markc_offset
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: getc_offset , if fail -1
*****************************************************************************************************************/
short b_reset(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_1;
	}
	/*make getc_offset equal to marc_offset*/
	pBD->getc_offset = pBD->markc_offset;

	return pBD->getc_offset;
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to return getc_offset to calling fuction
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: get_offset if fail then -1
*****************************************************************************************************************/
short b_getcoffset(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_1;
	}

	return pBD->getc_offset;
}

/*****************************************************************************************************************
Purpose: The purpose of this function is to set getc_offset to 0 so bugger can  be re read again
Author: Jonathan Slaunwhite
Histroy/Versions 1.0  2020-01-30
Called Functions none
Parameters Buffer* const pBD
Return Value: 0 if error then return -1
*****************************************************************************************************************/
int b_rewind(Buffer* const pBD) {

	if (!pBD) {
		return RT_FAIL_1;
	}
	/*set parameters to 0 so buffer can be re read*/
	pBD->getc_offset = 0;
	pBD->markc_offset = 0;

	return 0;
}
