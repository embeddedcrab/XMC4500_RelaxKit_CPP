/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		GPIO API Source
* Filename:		dri_gpio.cpp
* Author:		HS
* Origin Date:	10/01/2019
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_gpio.cpp
 *  @brief:	This header file contains API function definitions for
 *  		GPIO.
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_gpio.h>


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
 * @brief
 */



/******************************************************************************
* Function Definitions
*******************************************************************************/

/**
 * @function	power_saver_mode
 *
 * @brief		enable or disable power saver mode
 *
 * <i>Imp Note:</i>
 *
 */
void GPIO__::power_saver_mode( XMC_GPIO_PORT_t *const p_port, const unsigned char pin,
								eEnableDisable_ enable
								)
{
	/* Check input argument */
	switch( enable )
	{
		default:/* Considering enable or disable as default case */
			case eEnableDisable_::DISABLE:
				XMC_GPIO_DisablePowerSaveMode( p_port, pin );
			break;

			case eEnableDisable_::ENABLE:
				XMC_GPIO_EnablePowerSaveMode( p_port, pin );
			break;
	}
}


/**
 * @function	digital_input
 *
 * @brief		enable or disable digital input for analog pin
 *
 * <i>Imp Note:</i>
 *
 */
void GPIO__::digital_input( XMC_GPIO_PORT_t *const p_port, const unsigned char pin,
							eEnableDisable_ enable
							)
{
	/* Check input argument */
	switch( enable )
	{
		default:/* Considering enable or disable as default case */
			case eEnableDisable_::DISABLE:
				XMC_GPIO_DisableDigitalInput( p_port, pin );
			break;

			case eEnableDisable_::ENABLE:
				XMC_GPIO_EnableDigitalInput( p_port, pin );
			break;
	}
}


/**
 * @function	set_hw_ctrl
 *
 * @brief		set hw control for pins
 *
 * <i>Imp Note:</i>
 *
 */
void GPIO__::set_hw_ctrl( XMC_GPIO_PORT_t *const p_port, const unsigned char pin,
							const XMC_GPIO_HWCTRL_t hwctrl
							)
{
	XMC_GPIO_SetHardwareControl( p_port, pin, hwctrl);
}


/********************************** End of File *******************************/
