/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		UART API Header
* Filename:		dri_uart.h
* Author:		HS
* Origin Date:	09/29/2019
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_uart.h
 *  @brief:	This file contains API function declarations
 *  		for UART on USIC channel
 */
#ifndef DRI_UART_H_
#define DRI_UART_H_


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_uart_types.h>


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

/* Callback handler for user */
#if ( 1 == UART_CALLBACK_HANDLER_USED )
	extern void uart_callback_handler(eUART_Channel_& channel, unsigned char *data, eUART_Event_ event);
#endif


/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
 *
 * @brief	Class for UART Instance
 *
 ******************************************************************************/

/* Check UART Usage in system */
#if( UART_CHANNELS_USED > 0 )

class UART__
{
/* public members */
public:
	/** Constructors and Destructors */
	UART__() = delete;

	explicit UART__( const eUART_Channel_& channel_l ) : channel(channel_l)
	{
		/* Assign context */
		context.p_channel = ( eUART_Channel_::UART_CHANNEL_0 == this->channel ) ? XMC_USIC0_CH0 :
							( eUART_Channel_::UART_CHANNEL_1 == this->channel ) ? XMC_USIC0_CH1 :
							( eUART_Channel_::UART_CHANNEL_2 == this->channel ) ? XMC_USIC1_CH0 :
							XMC_USIC1_CH1;
		/* Assign channel context */
		config.p_channel_context = static_cast<tStUART_Context *>(&(this->context));
		/* Initialize control state */
		control.state.state_value = 0;
	}

	/* Destructor for class, not required but still present */
	~UART__()
	{
		/* Close UART channel */
		init( eUART_Init_::UART_INIT_CLOSE );
	}

	/******* Data Members *******/

	/******* API Member functions *******/

	/**
	 * @function	init
	 *
	 * @brief		Initialization function for UART channel
	 *
	 * @param[in]	init	-	initialization state
	 * 							eUART_Init_	-	UART_INIT_OPEN
	 * 											UART_INIT_CLOSE
	 *
	 * @param[out]	NA
	 *
	 * @return  	eUART_Status_
	 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
	 *          	UART_STATUS_FAILED:		Operation failed.<BR>
	 *
	 * \par<b>Description:</b><br>
	 * 				This function will initialize UART channel
	 * 				with required parameters for operation
	 *
	 * <i>Imp Note:</i>
	 *
	 */
	eUART_Status_ init( const eUART_Init_& init = eUART_Init_::UART_INIT_OPEN );

	/**
	 * @function	set_baudrate
	 *
	 * @brief		Set baudrate function
	 *
	 * @param[in]	speed	-	speed of operation of UART
	 *
	 * @param[out]	NA
	 *
	 * @return  	eUART_Status_
	 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
	 *          	UART_STATUS_FAILED:		Operation failed.<BR>
	 *
	 * \par<b>Description:</b><br>
	 * 				This function will set baud rate for a particular channel
	 *
	 * <i>Imp Note:</i>
	 *
	 */
	eUART_Status_ set_baudrate( const unsigned long& speed = 9600 );

	/**
	 * @function	transmit
	 *
	 * @brief		Registers as request for data transmission
	 *
	 * @param[in]	p_src	-	source data pointer
	 * 				length	-	length of source data
	 *
	 * @param[out]	NA
	 *
	 * @return  	eUART_Status
	 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
	 *          	UART_STATUS_FAILED:		Operation failed.<BR>
	 *
	 * \par<b>Description:</b><br>
	 * 				This function transmit data using interrupt functionality
	 *
	 * <i>Imp Note:</i>
	 * 				This function can have functionality of waiting as used
	 * 				in Polling of transmission data
	 *
	 */
	eUART_Status_ transmit( const unsigned char * p_src, const unsigned long& length = 1 );

	/**
	 * @function	receive
	 *
	 * @brief		Registers a request for data reception
	 *
	 * @param[in]	p_dest	-	destination data pointer
	 * 				length	-	length of data to be received
	 *
	 * @param[out]	p_dest	-	destination data
	 *
	 * @return		eUART_Status
	 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
	 *          	UART_STATUS_FAILED:		Operation failed.<BR>
	 *
	 * \par<b>Description:</b><br>
	 * 				This function will receive data from UART channel
	 *
	 * <i>Imp Note:</i>
	 * 				This function can have functionality of waiting as used in
	 * 				Polling for receive data
	 *
	 */
	eUART_Status_ receive( unsigned char * const p_dest, const unsigned long& length = 1 );

	/**
	 * @function	abort
	 *
	 * @brief		Data abort function
	 *
	 * @param[in]	tx_rx	-	abort state for Tx/Rx
	 * 							eUART_Abort_	-	UART_ABORT_TX
	 * 												UART_ABORT_RX
	 * 												UART_ABORT_TX_RX
	 *
	 * @param[out]	NA
	 *
	 * @return  	eUART_Status
	 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
	 *          	UART_STATUS_FAILED:		Operation failed.<BR>
	 *
	 * \par<b>Description:</b><br>
	 *
	 * <i>Imp Note:</i>
	 *
	 */
	eUART_Status_ abort( const eUART_Abort_& tx_rx );

	/******* Setters and Getters *******/
	/**
	 * @function	set
	 *
	 * @brief		setter function for UART__ class
	 *
	 * @param[in]	irq_config_l		-	interrupt configurations for UART object
	 * @param[in]	event_irq_config_l	-	event interrupt configuration for UART__ object
	 * @param[in]	protocol_event_l	-	event protocol configurations
	 * @param[in]	channel_cfg_l		-	UART channel configurations
	 *
	 * @param[out]	NA
	 *
	 * @return  	NA
	 *
	 * \par<b>Description:</b><br>
	 * 				This function will set the configuration parameters
	 * 				for UART__ class object
	 *
	 * <i>Imp Note:</i>
	 * 				Parameters are having default values, if not passed
	 * 				then default value will be considered
	 *
	 */
	void set( const tStUART_InterruptConfiguartion &irq_config_l
			#ifdef UART_PROTOCOL_EVENT_USED
			,const tStInterruptConfig &event_irq_config_l,
			const unsigned long protocol_event_l
			#endif
			, const tStUART_Configuartion_& channel_cfg_l
			)
	{
		/* Set parameters */
		irq_config = irq_config_l;
		#ifdef UART_PROTOCOL_EVENT_USED
		event_irq_config = event_irq_config_l;
		protocol_events = protocol_event_l;
		#endif
		config = channel_cfg_l;
		/* <i>Imp:</i> Need to write this statement otherwise object variables will be
		 * disturbed with default values of parameter */
		config.p_channel_context = static_cast<tStUART_Context *>(&(this->context));
	}

	/**
	 * @function	get
	 *
	 * @brief		getter function for UART__ class
	 *
	 * @param[in]	p_val	-	pointer to channel configurations
	 *
	 * @param[out]	p_val	-	data pointer to channel configurations
	 *
	 * @return  	NA
	 *
	 * \par<b>Description:</b><br>
	 * 				This function will get the channel configuration parameters
	 * 				for UART__ class object
	 *
	 * <i>Imp Note:</i>
	 *
	 */
	void get( tStUART_Configuartion * const p_val ) const
	{
		p_val->channel_cfg = config.channel_cfg;
		p_val->fifo_cfg = config.fifo_cfg;
		p_val->p_channel_context = config.p_channel_context;
	}

	#if (defined(UART_TX_INTERRUPT_USED) ||	\
		defined(UART_RX_INTERRUPT_USED) ||	\
		defined(UART_PROTOCOL_EVENT_USED))
	/* ISR entry function for UART__ class */
	static void isr_entry( UART__* p_object, const eUART_InterruptOperation_& type )
	{
		switch( type )
		{
			default:/* Considering all cases in default */
			#ifdef UART_TX_INTERRUPT_USED
			case eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_TX:
				p_object->tx_isr_entry( p_object );
				break;
			#endif
			#ifdef UART_RX_INTERRUPT_USED
			case eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_RX:
				p_object->rx_isr_entry( p_object );
				break;
			#endif
			#ifdef UART_PROTOCOL_EVENT_USED
			case eUART_InterruptOperation_::UART_INTERRUPT_OPERATION_ERROR:
				p_object->err_isr_entry( p_object );
				break;
			#endif
		}
	}
	#endif	/* ISR Handler functions. UART_TX_INTERRUPT_USED,
				UART_RX_INTERRUPT_USED, UART_PROTOCOL_EVENT_USED */

	/* Object initializer for UART class */
	static void initialize_uart_objects( void );

/* private members */
private:
	/** Data Members **/
	eUART_Channel_ channel;
	tStUART_Configuartion config;
	tStUART_Context context;
	tStUART_InterruptConfiguartion irq_config;
	tStUART_Control control;
	#ifdef UART_PROTOCOL_EVENT_USED
		tStInterruptConfig event_irq_config;
		unsigned long protocol_events;
	#endif	/* UART_PROTOCOL_EVENT_USED */

	/* Single callback for all channels */
	#if ( 1 == UART_CALLBACK_HANDLER_USED )
	static constexpr uart_callback_pointer fp_callback = uart_callback_handler;
	#else
	static constexpr uart_callback_pointer callback = nullptr;
	#endif


	/******* Private Member Functions *******/
	unsigned char find_uart_channel_input_source( const eUART_Channel_& channel ) const;
	void calc_fifo( const eUART_Channel_& channel, unsigned long (&fifo_size_arr)[2] );
	void reconfigure_rx_fifo( unsigned long data_size ) const;

	/* ISR Handler functions */
	#ifdef UART_TX_INTERRUPT_USED
	void tx_isr_entry( UART__* p_handle );
	#endif

	#ifdef UART_RX_INTERRUPT_USED
	void rx_isr_direct( UART__* p_handle );
	void rx_isr_entry( UART__* p_handle );
	#endif

	#ifdef UART_PROTOCOL_EVENT_USED
	void err_isr_entry( UART__* p_handle );
	#endif

/* protected members, if any */
protected:

};

#endif	/* UART_CHANNELS_USED */


#endif /* DRI_UART_H_ */

/********************************** End of File *******************************/
