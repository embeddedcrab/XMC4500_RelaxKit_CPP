/******************************************************************************
 * Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		GPIO API Source
* Filename:		dri_gpio_pins.cpp
* Author:		HS
* Origin Date:	10/01/2019
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_gpio_pins.cpp
 *  @brief:	This header file contains API function definitions for
 *  		GPIO pins information.
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_gpio_pins.h>


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

/*
 * @brief Array size used for Pin configuration
 */
static constexpr auto sg_pins_array_size (8);

/*
 * @brief Structure array used for Pin configuration
 */
static const tStPinConfig sg_gpio_pins[::sg_pins_array_size] =
{
		{/**< Port information for GPIO Pin 0 on port 1 */
				XMC_GPIO_PORT1,
				{
						XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
						XMC_GPIO_OUTPUT_LEVEL_LOW,
						XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
				},
				0
		},
		{/**< Port information for GPIO Pin 1 on port 1 */
				XMC_GPIO_PORT1,
				{
						XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
						XMC_GPIO_OUTPUT_LEVEL_LOW,
						XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
				},
				1
		},
		{/**< Port information for UART RX0 Pin 0 on port 5, UART Channel 0 */
				XMC_GPIO_PORT5,
				{
						XMC_GPIO_MODE_INPUT_TRISTATE,
						XMC_GPIO_OUTPUT_LEVEL_HIGH,
						XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
				},
				0
		},
		{/**< Port information for UART TX0 Pin 1 on port 5, UART Channel 0 */
				XMC_GPIO_PORT5,
				{
						XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
						XMC_GPIO_OUTPUT_LEVEL_HIGH,
						XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
				},
				1
		},
		{/**< Port information for UART RX1 Pin 4 on port 0, UART Channel 2 */
				XMC_GPIO_PORT0,
				{
						XMC_GPIO_MODE_INPUT_TRISTATE,
						XMC_GPIO_OUTPUT_LEVEL_HIGH,
						XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
				},
				4
		},
		{/**< Port information for UART TX0 Pin 5 on port 0, UART Channel 2 */
				XMC_GPIO_PORT0,
				{
						XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
						XMC_GPIO_OUTPUT_LEVEL_HIGH,
						XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
				},
				5
		},
		{/**< Port information for UART RX2 Pin 0 on port 0, UART Channel 3 */
				XMC_GPIO_PORT0,
				{
						XMC_GPIO_MODE_INPUT_TRISTATE,
						XMC_GPIO_OUTPUT_LEVEL_HIGH,
						XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
				},
				0
		},
		{/**< Port information for UART TX2 Pin 1 on port 0, UART Channel 3 */
				XMC_GPIO_PORT0,
				{
						XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
						XMC_GPIO_OUTPUT_LEVEL_HIGH,
						XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
				},
				1
		}
};



/******************************************************************************
* Function Definitions
*******************************************************************************/

/**
 * @function	init
 *
 * @brief		Pins initialization function
 *
 * <i>Imp Note:</i>
 * 				This function has to be called first in main or before main!
 *
 */
void GPIO_Pins__::init( void )
{
	/* Local Variables */
	unsigned char pins = 0;

	for( pins = 0; pins < ::sg_pins_array_size; ++pins )
	{
		XMC_GPIO_Init( ::sg_gpio_pins[pins].p_port,
						::sg_gpio_pins[pins].pin,
						 &::sg_gpio_pins[pins].config
						 );
	}
}


/* Providing call to pins initialization for C */
extern "C" void gpio_init_c( void )
{
	/* Initialize Hardware pins functionality options */
	GPIO_Pins__::init();
}

/********************************** End of File *******************************/
