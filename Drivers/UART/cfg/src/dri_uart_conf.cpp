/*****************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		UART Configuration Source
* Filename:		dri_uart_conf.cpp
* Author:		HS
* Origin Date:	09/29/2019
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_uart_conf.cpp
 *  @brief:	This source file contains configuration function definitions for
 *  		UART on USIC channel
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_uart.h>
#include <dri_mem.h>


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
 * @brief	Objects for UART Class
 *
 * <i>Imp Note:</i>
 * 			Single class object will take 72 bytes of memory + 4 for object pointer
 */
static UART__ sg_uart_obj_ch0 (eUART_Channel_::UART_CHANNEL_0 );
/* Not using because of pins unavailability, if need to use then add configurations */
//static UART__ sg_uart_obj_ch1 (eUART_Channel_::UART_CHANNEL_1 );
static UART__ sg_uart_obj_ch2 (eUART_Channel_::UART_CHANNEL_2 );
static UART__ sg_uart_obj_ch3 (eUART_Channel_::UART_CHANNEL_3 );

UART__ * g_p_uart_obj[UART_CHANNELS_USED] =
{ &sg_uart_obj_ch0, &sg_uart_obj_ch2, &sg_uart_obj_ch3 };



/******************************************************************************
* Function Definitions
*******************************************************************************/

/* Check UART Usage in system */
#if( UART_CHANNELS_USED > 0 )

/**
 * @function	initialize_uart_objects
 *
 * @brief		Initialize UART Objects with specific properties
 *
 * <i>Imp Note:</i>
 * 				Do not call this function more than once!
 */
void UART__::initialize_uart_objects( void )
{
	/* Local Variables */
	tStUART_Configuartion_ config;
	tStUART_InterruptConfiguartion irq_config = {
			.tx_irq = { .irq_num = 86, .irq_priority = 60 },
			.rx_irq = { .irq_num = 85, .irq_priority = 59 },
			.tx_sr = UART_0_TX_SR
	};

	#ifdef UART_PROTOCOL_EVENT_USED
	tStInterruptConfig event_config = { .irq_num = 84, .irq_priority = 58 };
	unsigned long events = ((unsigned long)XMC_UART_CH_EVENT_RECEIVER_NOISE |
							(unsigned long)XMC_UART_CH_EVENT_COLLISION |
							(unsigned long)XMC_UART_CH_EVENT_FORMAT_ERROR |
							(unsigned long)XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK
							);
	#endif

	#ifdef UART_CHANNEL_0_USED
	config.channel_cfg.baudrate = 115200;
	/* Initialize UART__ Object channel 0 parameters */
	g_p_uart_obj[0]->set( irq_config
						#ifdef UART_PROTOCOL_EVENT_USED
						,event_config,
						events
						#endif
						, config
						);

	__asm("NOP");
	#endif	/* UART_CHANNEL_0_USED */

	#ifdef UART_CHANNEL_1_USED
	/* Initialize UART__ Object channel 1 parameters */
	irq_config.tx_irq.irq_num = 88;
	irq_config.rx_irq.irq_num = 87;
	irq_config.tx_irq.irq_priority = 62;
	irq_config.rx_irq.irq_priority = 61;
	irq_config.tx_sr = UART_1_TX_SR;
	events = 0;
	event_config.irq_num = 87;	/* Makes no sense */

	/* Initialize UART__ Object channel 1 parameters */
	g_p_uart_obj[1]->set( irq_config
						#ifdef UART_PROTOCOL_EVENT_USED
						,event_config,
						events
						#endif
						, config
						);

	__asm("NOP");
	#endif	/* UART_CHANNEL_1_USED */

	#ifdef UART_CHANNEL_2_USED
	/* Initialize UART__ Object channel 2 parameters */
	irq_config.tx_irq.irq_num = 91;
	irq_config.rx_irq.irq_num = 90;
	irq_config.tx_irq.irq_priority = 62;
	irq_config.rx_irq.irq_priority = 61;
	irq_config.tx_sr = UART_2_TX_SR;

	#ifdef UART_PROTOCOL_EVENT_USED
	events = 0;
	event_config.irq_num = 90;/* Makes no sense */
	#endif

	/* Initialize UART__ Object channel 2 parameters */
	g_p_uart_obj[1]->set( irq_config
						#ifdef UART_PROTOCOL_EVENT_USED
						,event_config,
						events
						#endif
						, config
						);

	__asm("NOP");
	#endif	/* UART_CHANNEL_2_USED */

	#ifdef UART_CHANNEL_3_USED
	/* Initialize UART__ Object channel 3 parameters */
	irq_config.tx_irq.irq_num = 93;
	irq_config.rx_irq.irq_num = 92;
	irq_config.tx_irq.irq_priority = 62;
	irq_config.rx_irq.irq_priority = 61;
	irq_config.tx_sr = UART_3_TX_SR;

	#ifdef UART_PROTOCOL_EVENT_USED
	events = 0;
	event_config.irq_num = 91;/* Makes no sense */
	#endif

	/* Initialize UART__ Object channel 3 parameters */
	g_p_uart_obj[2]->set( irq_config
						#ifdef UART_PROTOCOL_EVENT_USED
						,event_config,
						events
						#endif
						, config
						);

	__asm("NOP");
	#endif	/* UART_CHANNEL_3_USED */
}
#endif	/* UART_CHANNELS_USED */


/******* ISR Handlers for UART__ 0 channel *******/

extern "C"
{

#ifdef UART_0_TX_HANDLER
/**
 * @function	UART_0_TX_HANDLER
 *
 * @brief		UART0 tx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_0_TX_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch0, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_TX );
}
#endif	/* UART_0_TX_HANDLER */


#ifdef UART_0_RX_HANDLER
/**
 * @function	UART_0_RX_HANDLER
 *
 * @brief		UART0 rx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_0_RX_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch0, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_RX );
}
#endif	/* UART_0_RX_HANDLER */


#ifdef UART_0_ERR_HANDLER
/**
 * @function	UART_0_ERR_HANDLER
 *
 * @brief		UART0 err handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_0_ERR_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch0, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_ERROR );
}
#endif	/* UART_0_ERR_HANDLER */


/******* ISR Handlers for UART__ 1 channel *******/

#ifdef UART_1_TX_HANDLER
/**
 * @function	UART_1_TX_HANDLER
 *
 * @brief		UART1 tx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_1_TX_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch1, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_TX );
}
#endif	/* UART_1_TX_HANDLER */


#ifdef UART_1_RX_HANDLER
/**
 * @function	UART_1_RX_HANDLER
 *
 * @brief		UART1 rx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_1_RX_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch1, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_RX );
}
#endif	/* UART_1_RX_HANDLER */


#ifdef UART_1_ERR_HANDLER
/**
 * @function	UART_1_ERR_HANDLER
 *
 * @brief		UART1 err handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_1_ERR_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch1, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_ERROR );
}
#endif	/* UART_1_ERR_HANDLER */


/******* ISR Handlers for UART__ 2 channel *******/
#ifdef UART_2_TX_HANDLER
/**
 * @function	UART_2_TX_HANDLER
 *
 * @brief		UART2 tx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_2_TX_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch2, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_TX );
}
#endif	/* UART_2_TX_HANDLER */


#ifdef UART_2_RX_HANDLER
/**
 * @function	UART_2_RX_HANDLER
 *
 * @brief		UART2 rx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_2_RX_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch2, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_RX );
}
#endif	/* UART_2_RX_HANDLER */


#ifdef UART_2_ERR_HANDLER
/**
 * @function	UART_2_ERR_HANDLER
 *
 * @brief		UART2 err handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_2_ERR_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch2, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_ERROR );
}
#endif	/* UART_2_ERR_HANDLER */


/******* ISR Handlers for UART__ 3 channel *******/
#ifdef UART_3_TX_HANDLER
/**
 * @function	UART_3_TX_HANDLER
 *
 * @brief		UART3 tx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_3_TX_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch3, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_TX );
}
#endif	/* UART_3_TX_HANDLER */


#ifdef UART_3_RX_HANDLER
/**
 * @function	UART_3_RX_HANDLER
 *
 * @brief		UART3 rx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_3_RX_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch3, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_RX );
}
#endif	/* UART_3_RX_HANDLER */


#ifdef UART_3_ERR_HANDLER
/**
 * @function	UART_3_ERR_HANDLER
 *
 * @brief		UART3 err handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_3_ERR_HANDLER( void )
{
	/* Call ISR entry function */
	UART__::isr_entry( &sg_uart_obj_ch3, eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_ERROR );
}
#endif	/* UART_3_ERR_HANDLER */


}	/* extern "C"{ */


/**
 * @function		initialize_uart_objects_c
 *
 * @brief			UART objects initialization function
 *
 * <i>Imp Note:</i> To be called before main entry
 *
 */
extern "C" void initialize_uart_objects_c( void )
		{
			UART__::initialize_uart_objects();
		}


/*********************************** End of File ******************************/
