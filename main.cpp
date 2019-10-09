/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		Main Source
* Filename:		main.cpp
* Author:		HS
* Origin Date:	09/10/2019
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	main.cpp
 *  @brief:	This source file contains main function entry with
 *  		Exception handlers and callback functions
 */


/******************************************************************************
* Includes
*******************************************************************************/

/* Include peripheral device files */
#include <dri_gpio.h>
#include <dri_uart_extern.h>


/******************************************************************************
* Variables
*******************************************************************************/
unsigned char uart_data[13] = {0};


/******************************************************************************
* Function Declarations
*******************************************************************************/


/* Extern "C" block for C variables
 *
 * C Linkage in CPP Application
 * */
#ifdef __cplusplus
extern "C"
{

#define EXCEPTION_HANDLER_DEBUG_ON
//#undef EXCEPTION_HANDLER_DEBUG_ON

/******* User defined initialization functions *******/
void init_user_before_main( void );

}
#endif	/* __cplusplus */


/******************************************************************************
* Function Definitions
*******************************************************************************/

/**
 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point.
 * It is invoked by the device startup code.
 */
int main(void)
{
	/* Local Variables */

	/******* Initialize Peripherals *******/
	g_p_uart_obj[0]->init();
	g_p_uart_obj[1]->init();
	g_p_uart_obj[2]->init();

	/* Configure SysTick
	 * System core clock after PLL would be 120 MHz
	 *
	 * We need tick interrupt for every 1000 ticks i.e., 1ms
	 *  */
	if( 1 == SysTick_Config(SystemCoreClock / 1000UL) )
	{
		for( ; ; )
		{
			/* We are stuck!! */
		}
	}
	else
	{
		g_p_uart_obj[0]->receive( uart_data, 13 );
		g_p_uart_obj[0]->transmit( (const unsigned char *)"Hello World\r\n", 13 );
		g_p_uart_obj[0]->receive( uart_data, 13 );
		g_p_uart_obj[0]->transmit( (const unsigned char *)"Pello Porld\r\n", 13 );
		g_p_uart_obj[0]->receive( uart_data, 13 );
		g_p_uart_obj[0]->transmit( (const unsigned char *)"Tello Torld\r\n", 13 );
		g_p_uart_obj[0]->receive( uart_data, 13 );

		/* Connect tx and rx pins using a jumper i.e., P5.0 and P5.1
		 *
		 * uart_data should have "Hello World\r\n" and "tello Torld\r\n" after that
		 * To check place data in expressions and check its value
		 *  */

		/* Placeholder for user application code.
		 * The while loop below can be replaced with user application code. */
		while(1U)
		{
			__asm("NOP");
		}
	}

	return 0;
}

/******************************************************************************/


/* Exception Handler functions for debugging and tracing */
extern "C"{

void SysTick_Handler(void)
{
	/* Variables for SysTick interrupt */
	static unsigned long ticks = 0;

	/****** Operation in SysTick Interrupt Handler *******/
	/* Check tick counts */
	if( 500 == ticks )
	{
		/* Perform timer interrupt operations */
		GPIO__::fp_toggle( XMC_GPIO_PORT1, 0 );
		GPIO__::fp_toggle( XMC_GPIO_PORT1, 1 );

		/* Reset ticks */
		ticks = 0;
	}
	else
	{
		/* Increment ticks */
		ticks += 1;
	}
}


#ifdef EXCEPTION_HANDLER_DEBUG_ON

void get_registers_from_stack( unsigned long *pulFaultStackAddress )
{
	/* These are volatile to try and prevent the compiler/linker optimizing them
	 away as the variables never actually get used.  If the debugger won't show the
	 values of the variables, make them global my moving their declaration outside
	 of this function. */
	volatile uint32_t r0;
	volatile uint32_t r1;
	volatile uint32_t r2;
	volatile uint32_t r3;
	volatile uint32_t r12;
	volatile uint32_t lr;	/**< Link register. */
	volatile uint32_t pc;	/**< Program counter. */
	volatile uint32_t psr;	/**< Program status register. */

	/* Assign Register values into local variables */
	r0 = pulFaultStackAddress[0];
	r1 = pulFaultStackAddress[1];
	r2 = pulFaultStackAddress[2];
	r3 = pulFaultStackAddress[3];

	r12 = pulFaultStackAddress[4];
	lr = pulFaultStackAddress[5];
	pc = pulFaultStackAddress[6];
	psr = pulFaultStackAddress[7];

	/* When the following line is hit, the variables contain the register values. */
	for( ; ; );
}

#endif	/* EXCEPTION_HANDLER_DEBUG_ON */

void BusFault_Handler(void)
{
#ifdef EXCEPTION_HANDLER_DEBUG_ON
	__asm volatile
	(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" ldr r1, [r0, #24]                                         \n"
			" ldr r2, bus_fault_handler_address_const                   \n"
			" bx r2                                                     \n"
			" bus_fault_handler_address_const: .word get_registers_from_stack    \n"
	);
#endif
	for( ; ; );
}

void HardFault_Handler(void)
{
#ifdef EXCEPTION_HANDLER_DEBUG_ON
	__asm volatile
	(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" ldr r1, [r0, #24]                                         \n"
			" ldr r2, hard_fault_handler_address_const                  \n"
			" bx r2                                                     \n"
			" hard_fault_handler_address_const: .word get_registers_from_stack    \n"
	);
	for( ; ; );
#endif	/* EXCEPTION_HANDLER_DEBUG_ON */
}

void MemManage_Handler(void)
{
#ifdef EXCEPTION_HANDLER_DEBUG_ON
	/* Exception tracing */
#endif
	for( ; ; );
}

void UsageFault_Handler(void)
{
#ifdef EXCEPTION_HANDLER_DEBUG_ON
	/* Exception tracing */
#endif
	for( ; ; );
}

}/* } end */


/* Extern "C" block for C Functions */
/** User defined functions **/
extern "C" void init_user_before_main( void )
		{
			extern void gpio_init_c( void );
			gpio_init_c();
			extern void initialize_uart_objects_c( void );
			initialize_uart_objects_c();
		}


/* UART__ class callback handler function for User */
void uart_callback_handler(eUART_Channel_& channel, unsigned char *p_data, eUART_Event_ event)
{
	/* Can check the events for callback and do processing */
	(void) p_data;

	/* Check events */
	switch( event )
	{
		default:
		case eUART_Event_::UART_EVENT_TX_COMPLETE:
			if( eUART_Channel_::UART_CHANNEL_0 == channel )
			{
				/* Transmission successful! */
				__asm("NOP");
			}
			break;

		case eUART_Event_::UART_EVENT_RX_COMPLETE:
			if( eUART_Channel_::UART_CHANNEL_0 == channel )
			{
				/* Reception successful! */
				__asm("NOP");
			}
			break;
	}
}

/********************************** End of File *******************************/
