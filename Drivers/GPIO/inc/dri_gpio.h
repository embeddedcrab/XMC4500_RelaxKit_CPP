/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		GPIO Header API
* Filename:		dri_gpio.h
* Author:		HS
* Origin Date:	10/1/2019
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_gpio.h
 *  @brief:	This file contains GPIO API header
 */
#ifndef DRI_GPIO_H_
#define DRI_GPIO_H_


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_types.h>


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

/*
 * @brief	data types for function pointers of GPIO API
 */
typedef void (*init) ( XMC_GPIO_PORT_t *const port, const unsigned char pin,
						const XMC_GPIO_CONFIG_t *const config );

typedef void (*set_mode) ( XMC_GPIO_PORT_t *const port, const unsigned char pin,
							const XMC_GPIO_MODE_t mode );

typedef void (*control) ( XMC_GPIO_PORT_t *const port, const unsigned char pin,
							const XMC_GPIO_OUTPUT_LEVEL_t level );

typedef void (*toggle) ( XMC_GPIO_PORT_t *const port, const unsigned char pin );

typedef unsigned long (*get_input) ( XMC_GPIO_PORT_t *const port,
										const unsigned char pin );


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
 *
 * @brief	Class for GPIO API
 *
 ******************************************************************************/
class GPIO__
{
/* public members */
public:
	/* Constructors */
	GPIO__() = delete;
	GPIO__( GPIO__& ) = delete;

	/* Function pointers list */
	static constexpr init fp_init = XMC_GPIO_Init;
	static constexpr set_mode fp_set_mode = XMC_GPIO_SetMode;
	static constexpr control fp_ctrl = XMC_GPIO_SetOutputLevel;
	static constexpr toggle fp_toggle = XMC_GPIO_ToggleOutput;
	static constexpr get_input fp_get_input = XMC_GPIO_GetInput;

	/**
	 * @function	power_saver_mode
	 *
	 * @brief		Set/Reset power saver mode
	 *
	 * @param[in]	port	-	gpio port information
	 * @param[in]	pin		-	gpio pin information
	 * @param[in]	enable	-	enable/disable
	 * 							0	-	disable
	 * 							1	-	enable
	 *
	 * @param[out]	NA
	 *
	 * @return  	NA
	 *
	 * \par<b>Description:</b><br>
	 * 				This function set or reset power saver mode for pin
	 *
	 * <i>Imp Note:</i>
	 *
	 */
	static inline void power_saver_mode( XMC_GPIO_PORT_t *const p_port,
								const unsigned char pin, eEnableDisable_ enable
								);

	/**
	 * @function	digital_input
	 *
	 * @brief		Set/Reset digital input for pin
	 *
	 * @param[in]	port	-	gpio port information
	 * @param[in]	pin		-	gpio pin information
	 * @param[in]	enable	-	enable/disable
	 * 							0	-	disable
	 * 							1	-	enable
	 *
	 * @param[out]	NA
	 *
	 * @return  	NA
	 *
	 * \par<b>Description:</b><br>
	 * 				This function set or reset digital input on analog pin
	 *
	 * <i>Imp Note:</i>
	 *
	 */
	static inline void digital_input( XMC_GPIO_PORT_t *const p_port,
								const unsigned char pin, eEnableDisable_ enable
								);

	/**
	 * @function	set_hw_ctrl
	 *
	 * @brief		Set hardware control
	 *
	 * @param[in]	port	-	gpio port information
	 * @param[in]	pin		-	gpio pin information
	 * @param[in]	hwctrl	-	hardware control info
	 *
	 * @param[out]	NA
	 *
	 * @return  	NA
	 *
	 * \par<b>Description:</b><br>
	 * 				This function set hardware control for pin
	 *
	 * <i>Imp Note:</i>
	 *
	 */
	static inline void set_hw_ctrl( XMC_GPIO_PORT_t *const p_port,
						const unsigned char pin, const XMC_GPIO_HWCTRL_t hwctrl
						);
};


#endif /* DRI_GPIO_H_ */

/********************************** End of File *******************************/
