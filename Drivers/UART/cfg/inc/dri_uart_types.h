/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		UART data types Header
* Filename:		dri_uart_types.h
* Author:		HS
* Origin Date:	09/29/2019
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_uart_types.h
 *  @brief:	This file contains data types definitions for UART channel on USIC
 */
#ifndef DRI_UART_TYPES_H_
#define DRI_UART_TYPES_H_


/******************************************************************************
* Includes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

/* Include XMC Headers */
#include <xmc_uart.h>

#ifdef __cplusplus
}	/* extern "C" { */
#endif

/* Include private headers first */
#include <private/dri_uart_conf.h>

/* Include typedefs */
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
 * @brief	Enum for Status of UART
 * */
enum class eUART_Status_/*	:	unsigned char	*/
{
	UART_STATUS_SUCCESS = 0,
	UART_STATUS_FAILED,
	UART_STATUS_BUSY,
	UART_STATUS_INVALID_ARGUMENT,
	UART_STATUS_OUT_OF_BOUND
};

/*
 * @brief	Enum for Open or close
 * */
enum class eUART_Init_/*	:	unsigned char	*/
{
	UART_INIT_CLOSE = 0,
	UART_INIT_OPEN
};

/*
 * @brief	Enum for Operating Mode of UART
 * */
enum class eUART_Mode_/*	:	unsigned char	*/
{
	UART_MODE_FULL_DUPLEX = 0,
	UART_MODE_HALF_DUPLEX,
	UART_MODE_LOOPBACK
};

/*
 * @brief	Enum for Operation Events of UART
 * */
enum class eUART_Event_	:	unsigned char
{
	UART_EVENT_SYNC_BREAK_DETECT = 0,
	UART_EVENT_COLLISION_DETECT,
	UART_EVENT_RECEIVER_NOISE_DETECT,
	UART_EVENT_FORMAT_ERROR_0,
	UART_EVENT_FORMAT_ERROR_1,
	UART_EVENT_MAX,	/* Used as counter for Max error possibilities */
	UART_EVENT_RX_CHAR,
	UART_EVENT_TX_COMPLETE,
	UART_EVENT_RX_COMPLETE
};

/*
 * @brief	Enum for UART ISR operation
 * */
enum class eUART_InterruptOperation_	:	unsigned char
{
	UART_INTERRUPT_OPERATION_TX = 0,
	UART_INTERRUPT_OPERATION_RX,
	UART_INTERRUPT_OPERATION_ERROR
};

/*
 * @brief	Enum for Abort operation
 * */
enum class eUART_Abort_	:	unsigned char
{
	UART_ABORT_TX = 0,
	UART_ABORT_RX,
	UART_ABORT_TX_RX
};

/*
 * @brief	Enum for UART Channel Information
 * */
enum class eUART_Channel_	:	unsigned char
{
	UART_CHANNEL_0 = 0,
	UART_CHANNEL_1,
	UART_CHANNEL_2,
	UART_CHANNEL_3
};


/*
 * @brief	typedef for callback function pointer
 * */
typedef void (*uart_callback_pointer) ( eUART_Channel_& channel, unsigned char *data, eUART_Event_ event );


/******************************************************************************
 *
 * @brief	Data Types for UART instance
 *
 *****************************************************************************/

/*
 * @brief	typedef for status control of UART
 */
typedef struct tStUART_StatusControl_
{
	union
	{
		struct
		{
			unsigned char init		: 1;
			unsigned char tx_lock	: 1;
			unsigned char tx_busy	: 1;
			unsigned char rx_lock	: 1;
			unsigned char rx_busy	: 1;
			unsigned char RES		: 3;
		};
		unsigned char state_value = 0;
	};
} tStUART_StatusControl;

/*
 * @brief	Structure for UART control
 */
typedef struct tStUART_Control_
{
	/**< Pointer to the transmit data buffer */
	const unsigned char * p_tx_data = nullptr;
	/**< Pointer to the receive data buffer */
	unsigned char * p_rx_data = nullptr;
	unsigned long tx_data_count = 0U;			/**< Transmit data count */
	unsigned long tx_data_index = 0U;			/**< Transmit data index */
	unsigned long rx_data_count = 0U;			/**< Receive data count */
	unsigned long rx_data_index = 0U;			/**< Receive data index */
	volatile tStUART_StatusControl state;		/**< UART data control state */
} tStUART_Control;

/*
 * @brief Structure used for Channel Context
 */
typedef struct tStUART_Context_
{
	/**< channel information for current context */
	XMC_USIC_CH_t * p_channel;
} tStUART_Context;

/*
 * @brief Structure for UART channel fifo size configuration
 */
typedef struct tStUART_FIFO_Configuartion_
{
	/* fifo size configuration for tx and rx */
	XMC_USIC_CH_FIFO_SIZE_t tx_fifo_size = XMC_USIC_CH_FIFO_SIZE_32WORDS;
	XMC_USIC_CH_FIFO_SIZE_t rx_fifo_size = XMC_USIC_CH_FIFO_SIZE_32WORDS;
} tStUART_FIFO_Configuartion;

/*
 * @brief Structure for UART channel configuration
 */
typedef struct tStUART_Configuartion_
{
	/**< pointer to configuration structure */
	XMC_UART_CH_CONFIG_t channel_cfg = {9600U, 8U, 8U, 1U, 16U, XMC_USIC_CH_PARITY_MODE_NONE};
	/** Channel FIFO configuration */
	tStUART_FIFO_Configuartion fifo_cfg;
	/**< pointer to channel context */
	void * p_channel_context = nullptr;
} tStUART_Configuartion;

/*
 * @brief Structure for UART channel interrupt configuration
 */
typedef struct tStUART_InterruptConfiguartion_
{
	/**< Interrupt configuration for tx and rx */
	tStInterruptConfig tx_irq;
	tStInterruptConfig rx_irq;
	/**< Service request number assigned to transmit interrupt */
	unsigned long tx_sr;
} tStUART_InterruptConfiguartion;


#endif /* DRI_UART_TYPES_H_ */

/********************************** End of File *******************************/
