/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		Memory Header file
* Filename:		dri_mem.h
* Author:		HS
* Origin Date:	10/04/2019
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_mem.h
 *  @brief:	This file contains memory allocation operator
 *  		Memory operation functions to replace standard library functions
 */
#ifndef DRI_MEM_H_
#define DRI_MEM_H_


/******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
 *******************************************************************************/

/**
 * @function	Operator overloading for new
 *
 * @brief		This function is a C++ wrapper
 *
 * @param[in]	Size of data
 *
 * @param[out]	Memory allocated address
 *
 * @return  	Memory allocated for variables address
 *
 * \par<b>Description:</b><br>
 * 				These kind of wrappers can be used for any RTOS which provide
 * 				memory allocation in C standard
 *
 * <i>Imp Note:</i>
 * 				Should not be used for Bare-metal Targets
 *
 */
inline void * operator new( size_t size )
{
	(void) size;
	return nullptr;
}


/**
 * @function	Operator overloading for new with array of elements
 *
 * @brief		This function is a C++ wrapper
 *
 * @param[in]	Size of data and array
 *
 * @param[out]	Memory allocated address
 *
 * @return  	Memory allocated for variables address
 *
 * \par<b>Description:</b><br>
 * 				These kind of wrappers can be used for any RTOS which provide
 * 				memory allocation in C standard
 *
 * <i>Imp Note:</i>
 * 				Should not be used for Bare-metal Targets
 *
 */
inline void * operator new[]( size_t size )
{
	(void) size;
	return nullptr;
}


/**
 * @function	operator overloading for delete
 *
 * @brief		This function deallocates previously allocated memory from heap
 *
 * @param[in]	Memory address
 *
 * @param[out]	NA
 *
 * @return  	NA
 *
 * \par<b>Description:</b><br>
 * 				These kind of wrappers can be used for any RTOS which provide
 * 				memory allocation in C standard
 *
 * <i>Imp Note:</i>
 *
 */
inline void operator delete( void * p_arg )
{
	(void) p_arg;
}


/**
 * @function	operator overloading for delete with array
 *
 * @brief		This function deallocates previously allocated memory from heap
 *
 * @param[in]	Memory address
 *
 * @param[out]	NA
 *
 * @return  	NA
 *
 * \par<b>Description:</b><br>
 * 				These kind of wrappers can be used for any RTOS which provide
 * 				memory allocation in C standard
 *
 * <i>Imp Note:</i>
 *
 */
inline void operator delete[]( void * p_arg )
{
	(void) p_arg;
}


#endif /* DRI_MEM_H_ */

/********************************** End of File *******************************/
