/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		UART API Source
* Filename:		dri_uart.cpp
* Author:		HS
* Origin Date:	09/29/2019
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_uart.cpp
 *  @brief:	This header file contains API function definitions for
 *  		UART on USIC channel. It contains Interrupt handlers for UART.
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_uart.h>


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

/* Declare static data member
 *
 * Note: Tell linker to take variable from this source file
 * */


#ifdef UART_PROTOCOL_EVENT_USED

/* Protocol events configurations and flags data */
constexpr XMC_UART_CH_STATUS_FLAG_t uart_event_status_flags[static_cast<unsigned char>(eUART_Event_::UART_EVENT_MAX)] =
{
  XMC_UART_CH_STATUS_FLAG_SYNCHRONIZATION_BREAK_DETECTED,
  XMC_UART_CH_STATUS_FLAG_RECEIVER_NOISE_DETECTED,
  XMC_UART_CH_STATUS_FLAG_FORMAT_ERROR_IN_STOP_BIT_0,
  XMC_UART_CH_STATUS_FLAG_FORMAT_ERROR_IN_STOP_BIT_1,
  XMC_UART_CH_STATUS_FLAG_COLLISION_DETECTED
};


constexpr XMC_UART_CH_EVENT_t uart_event_conf_flags[static_cast<unsigned char>(eUART_Event_::UART_EVENT_MAX)] =
{
  XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK,
  XMC_UART_CH_EVENT_RECEIVER_NOISE,
  XMC_UART_CH_EVENT_FORMAT_ERROR,
  XMC_UART_CH_EVENT_FORMAT_ERROR,
  XMC_UART_CH_EVENT_COLLISION
};

#endif	/* UART_PROTOCOL_EVENT_USED */


/******************************************************************************
* Function Definitions
*******************************************************************************/

/* Check UART Usage in system */
#if( UART_CHANNELS_USED > 0 )


/******* Public Member function for UART__ class *******/

/**
 * @function	init
 *
 * @brief		Initialization function for UART channel
 *
 * <i>Imp Note:</i>
 *
 */
eUART_Status_ UART__::init( const eUART_Init_& init )
{
	/* Local Variables */
	eUART_Status_ ret_status = eUART_Status_::UART_STATUS_FAILED;
	unsigned long fifo_size[2] = {0U};

	/* Calculate FIFO size */
	calc_fifo( channel, fifo_size );

	/* Check input argument */
	switch( init )
	{	/* Open UART Channel */
		case eUART_Init_::UART_INIT_OPEN:
			/* Check for initialization */
			if( control.state.state_value > 0 ){ break;	}
			else{	/* Not required */	}

			/* Initialize UART channel for communication */
			XMC_UART_CH_Init( context.p_channel, &(config.channel_cfg) );
			/* Set input source path, decides the mode of communication as well */
			XMC_USIC_CH_SetInputSource( context.p_channel,
										XMC_USIC_CH_INPUT_DX0,
										find_uart_channel_input_source( channel )
										);
			/* Configure transmit FIFO */
			XMC_USIC_CH_TXFIFO_Configure( context.p_channel, fifo_size[0],
											config.fifo_cfg.tx_fifo_size, 1U
											);
			/* Configure receive FIFO */
			XMC_USIC_CH_RXFIFO_Configure( context.p_channel, fifo_size[1],
											config.fifo_cfg.rx_fifo_size, 0U
											);
			/* Start UART channel */
			XMC_UART_CH_Start( context.p_channel );

			/* Set service request for UART protocol events */
			XMC_USIC_CH_SetInterruptNodePointer( context.p_channel,
								XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL
								#ifdef UART_PROTOCOL_EVENT_USED
								,
								( protocol_events > 0 ? 5U : 2U )
								#else
								,2U
								#endif
								);

			XMC_USIC_CH_TXFIFO_SetInterruptNodePointer( context.p_channel,
						XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
						irq_config.tx_sr
						);

			/* Set service request for rx FIFO receive interrupt */
			XMC_USIC_CH_RXFIFO_SetInterruptNodePointer( context.p_channel,
							XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
							(irq_config.tx_sr - 1)
							);
			XMC_USIC_CH_RXFIFO_SetInterruptNodePointer( context.p_channel,
						XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE,
						(irq_config.tx_sr - 1)
						);

			#ifdef UART_PROTOCOL_EVENT_USED
			/* Check for event handling functionality */
			if( protocol_events > 0 )
			{
				XMC_UART_CH_EnableEvent( context.p_channel, protocol_events );
				/* Initialize ISR for events */
				/* Set priority and enable NVIC node for event interrupts */
				NVIC_SetPriority( static_cast<IRQn_Type>(event_irq_config.irq_num),
									NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
									event_irq_config.irq_priority, 0U)
								);
				NVIC_EnableIRQ( static_cast<IRQn_Type>(event_irq_config.irq_num) );
			} else{ /* Not required */ }
			#endif	/* UART_PROTOCOL_EVENT_USED */

			/* Set priority and enable NVIC node for transmit interrupt */
			NVIC_SetPriority( static_cast<IRQn_Type>(irq_config.tx_irq.irq_num),
								NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
								irq_config.tx_irq.irq_priority,
								0U)
							);
			NVIC_EnableIRQ( static_cast<IRQn_Type>(irq_config.tx_irq.irq_num) );
			/* Set priority and enable NVIC node for receive interrupt */
			NVIC_SetPriority( static_cast<IRQn_Type>(irq_config.rx_irq.irq_num),
								NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
								irq_config.tx_irq.irq_priority,
								0U)
							);
			NVIC_EnableIRQ( static_cast<IRQn_Type>(irq_config.rx_irq.irq_num) );

			/* Set initialize state */
			control.state.state_value = 1;
			/* Update return status */
			ret_status = eUART_Status_::UART_STATUS_SUCCESS;
			break;
		/* Close UART Channel */
		case eUART_Init_::UART_INIT_CLOSE:
			if( XMC_UART_CH_STATUS_BUSY == XMC_UART_CH_Stop( context.p_channel )
				)
			{
				/* Update return status */
				ret_status = eUART_Status_::UART_STATUS_BUSY;
			}
			else
			{
				/* Reset control state */
				control.state.state_value = 0;
				/* Update return status */
				ret_status = eUART_Status_::UART_STATUS_SUCCESS;
			}
			break;
		/* Default parameters, invalid argument */
		default:
			/* Update return status */
			ret_status = eUART_Status_::UART_STATUS_INVALID_ARGUMENT;
			break;
	}

	/* Return status */
	return ret_status;
}


/**
 * @function	set_baudrate
 *
 * @brief		Set baud rate for UART channel
 *
 * <i>Imp Note:</i>
 *
 */
eUART_Status_ UART__::set_baudrate( const unsigned long& speed )
{
	/* Local Arguments */
	unsigned char status = 1;
	eUART_Status_ ret_status = eUART_Status_::UART_STATUS_FAILED;

	/* Check input argument validity */
	switch( speed )
	{
		/* Valid arguments, can add more valid baud rates */
		case 1200:
		case 4800:
		case 9600:
		case 19200:
		case 38400:
		case 115200:
			/* Good to go for baud rate update */
			break;

		/* Invalid arguments */
		default:
			ret_status = eUART_Status_::UART_STATUS_INVALID_ARGUMENT;
			return ret_status;
	}

	/* Check state of Tx and Rx locks */
	if( (1 == control.state.tx_lock) || (1 == control.state.rx_lock) ||
			(1 == control.state.tx_busy) || (1 == control.state.rx_busy)
			)
	{
		ret_status = eUART_Status_::UART_STATUS_BUSY;
	}
	else
	{
		/* Update speed */
		status = XMC_UART_CH_SetBaudrate( context.p_channel, speed,
											config.channel_cfg.oversampling
										);
		/* Check operation status */
		if( XMC_UART_CH_STATUS_OK == status )
		{
			config.channel_cfg.baudrate = speed;
			ret_status = eUART_Status_::UART_STATUS_SUCCESS;
		}
		else
		{
			ret_status = eUART_Status_::UART_STATUS_FAILED;
		}
	}

	/* Return status */
	return ret_status;
}


/**
 * @function	transmit
 *
 * @brief		Registers a request for data transmission
 *
 * <i>Imp Note:</i>
 *
 */
eUART_Status_ UART__::transmit( const unsigned char * p_src, const unsigned long& length )
{
	/* Input argument validity */
	DRIVER_ASSERT( nullptr == p_src,
					eUART_Status_::UART_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( 0U == length,
					eUART_Status_::UART_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( 0U == control.state.init,
						eUART_Status_::UART_STATUS_FAILED );

	/* Local Arguments */
	eUART_Status_ ret_status = eUART_Status_::UART_STATUS_FAILED;

	/* Check transmit busy status and perform operation */
	if( (0 == control.state.tx_busy) && (0 == control.state.tx_lock) )
	{
		/* Update required parameters for transmission interrupt */
		control.p_tx_data = p_src;
		control.tx_data_count = length;
		control.tx_data_index = 0;
		control.state.tx_busy = 1;
		control.state.tx_lock = 1;

		/* Trigger transmission interrupt */
		if( config.fifo_cfg.tx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
		{
			/* Clear transmit FIFO */
			XMC_USIC_CH_TXFIFO_Flush( context.p_channel );
			/* Enable transmit buffer event */
			XMC_USIC_CH_TXFIFO_EnableEvent( context.p_channel, (unsigned long)
										XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD
										);
		}
		else
		{
			/* Enable transmit buffer event */
			XMC_USIC_CH_EnableEvent( context.p_channel, (unsigned long)
										XMC_USIC_CH_EVENT_TRANSMIT_BUFFER
										);
		}
		/* Update status */
		ret_status = eUART_Status_::UART_STATUS_SUCCESS;
		/* Trigger the transmit buffer interrupt */
		XMC_USIC_CH_TriggerServiceRequest( context.p_channel, (unsigned long)irq_config.tx_sr );
	}
	else
	{
		ret_status = eUART_Status_::UART_STATUS_BUSY;
	}

	return ret_status;
}


/**
 * @function	receive
 *
 * @brief		Registers a request for data reception
 *
 * <i>Imp Note:</i>
 *
 */
eUART_Status_ UART__::receive( unsigned char * const p_dest, const unsigned long& length )
{
	/* Input argument validity */
	DRIVER_ASSERT( nullptr == p_dest,
					eUART_Status_::UART_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( 0U == length,
					eUART_Status_::UART_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( 0U == control.state.init,
						eUART_Status_::UART_STATUS_FAILED );

	/* Local Arguments */
	eUART_Status_ ret_status = eUART_Status_::UART_STATUS_FAILED;

	/* Check receive busy status and perform operation */
	if( (0 == control.state.rx_busy) && (0 == control.state.rx_lock) )/* 0x18 == (control.state.state_value & 0x18) */
	{
		/* Update receiver parameters */
		control.p_rx_data = p_dest;
		control.rx_data_count = length;
		control.rx_data_index = 0;
		control.state.rx_busy = 1;
		control.state.rx_lock = 1;

		/* Check FIFO configuration and trigger event */
		if( config.fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
		{
			/* Clear transmit FIFO */
			XMC_USIC_CH_RXFIFO_Flush( context.p_channel );

			/* Reconfigure FIFO for current data size */
			reconfigure_rx_fifo( control.rx_data_count );

			/* Enable receive buffer event */
			XMC_USIC_CH_RXFIFO_EnableEvent( context.p_channel, (unsigned long)
						((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
						(unsigned long)XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE)
						);
		}
		else {/* Enable receive buffer event */
			XMC_USIC_CH_RXFIFO_EnableEvent(context.p_channel, (unsigned long)
						((unsigned long) XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
						(unsigned long) XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE)
						);
		}
		/* Update status */
		ret_status = eUART_Status_::UART_STATUS_SUCCESS;
	}
	else
	{
		ret_status = eUART_Status_::UART_STATUS_BUSY;
	}

	return ret_status;
}


/**
 * @function	abort
 *
 * @brief		Abort transmission or reception
 *
 * <i>Imp Note:</i>
 *
 */
eUART_Status_ UART__::abort( const eUART_Abort_& tx_rx )
{
	/* Local Arguments */
	eUART_Status_ ret_status = eUART_Status_::UART_STATUS_FAILED;

	/* Check the abort request */
	switch( tx_rx )
	{
		default:
		case eUART_Abort_::UART_ABORT_TX:
			/* Reset Transmitter Parameters */
			control.state.tx_busy = 0;
			control.p_tx_data = nullptr;

			/* Disable the transmitter events */
			if( config.fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
			      /* Disable the transmit FIFO event */
			      XMC_USIC_CH_TXFIFO_DisableEvent( context.p_channel, (unsigned long)
			    		  	  	  	  	  XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD
						  	  	  	  	  );
			      XMC_USIC_CH_TXFIFO_Flush( context.p_channel );
			}
			else
			{
			      /*Disable the standard transmit event*/
			      XMC_USIC_CH_DisableEvent( context.p_channel, (unsigned long)
			    		  	  	  	  	  	  XMC_USIC_CH_EVENT_TRANSMIT_BUFFER
											  );
			}
			XMC_USIC_CH_SetTransmitBufferStatus( context.p_channel, XMC_USIC_CH_TBUF_STATUS_SET_IDLE );
		break;

		case eUART_Abort_::UART_ABORT_RX:
			/* Reset Receiver Parameters */
			control.state.rx_busy = 0;
			control.p_rx_data = nullptr;
			/* Disable the transmitter events */
			if( config.fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
				/* Disable the receiver event */
				XMC_USIC_CH_TXFIFO_DisableEvent( context.p_channel, (unsigned long)
									((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
									(unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE)
									);
			}
			else
			{
				XMC_UART_CH_DisableEvent( context.p_channel, (unsigned long)
									((unsigned long)XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
				            		(unsigned long)XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE)
									);
			}
		break;

		case eUART_Abort_::UART_ABORT_TX_RX:
			/* Reset Transmitter and Receiver parameters */
			control.state.tx_busy = 0;
			control.p_tx_data = nullptr;
			control.state.rx_busy = 0;
			control.p_rx_data = nullptr;

			/* Disable the transmitter events */
			if( config.fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
				/* Disable the transmit FIFO event */
				XMC_USIC_CH_TXFIFO_DisableEvent( context.p_channel, (unsigned long)
											XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD
											);
				XMC_USIC_CH_TXFIFO_Flush( context.p_channel );
			}
			else
			{
				/*Disable the standard transmit event*/
				XMC_USIC_CH_DisableEvent( context.p_channel, (unsigned long)
											XMC_USIC_CH_EVENT_TRANSMIT_BUFFER
											);
			}
			XMC_USIC_CH_SetTransmitBufferStatus( context.p_channel, XMC_USIC_CH_TBUF_STATUS_SET_IDLE );

			/* Disable the transmitter events */
			if( config.fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
				/* Disable the receiver event */
				XMC_USIC_CH_TXFIFO_DisableEvent( context.p_channel, (unsigned long)
									((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
									(unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE)
									);
			}
			else
			{
				XMC_UART_CH_DisableEvent( context.p_channel, (unsigned long)
									((unsigned long)XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
				            		(unsigned long)XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE)
									);
			}
		break;
	}

	return ret_status;
}


/******* Private Member function for UART__ class *******/

/**
 * @function	find_uart_channel_input_source
 *
 * @brief		find channel input source
 *
 * @param[in]	channel	-	UART channel number
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
unsigned char UART__::find_uart_channel_input_source( const eUART_Channel_& channel ) const
{
	/* Local Variables */
	unsigned char channel_value = 0x0;
	switch( channel )
	{
		default:/* Considering all in default */
		case eUART_Channel_::UART_CHANNEL_0: channel_value = 0x3; break; /* Using P5.0 for RX */
		case eUART_Channel_::UART_CHANNEL_1: /* return initialized value */
		case eUART_Channel_::UART_CHANNEL_2: break;
		case eUART_Channel_::UART_CHANNEL_3: channel_value = 0x3; break;
	}
	/* Return channel */
	return channel_value;
}


/**
 * @function	calc_fifo
 *
 * @brief		calculate FIFO size according to channel
 *
 * @param[in]	channel		-	UART channel number
 * @param[in]	fifo_size	-	fifo size array data reference
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
void UART__::calc_fifo( const eUART_Channel_& channel, unsigned long (&fifo_size_arr)[2] )
{
	/* Update fifo_size */
	fifo_size_arr[0] = 48U;
	fifo_size_arr[1] = 32U;
	switch( channel )
	{
		default:/* Considering all in default */
		case eUART_Channel_::UART_CHANNEL_0: /* return initialized value */
		case eUART_Channel_::UART_CHANNEL_2: break;
		case eUART_Channel_::UART_CHANNEL_1:
		case eUART_Channel_::UART_CHANNEL_3: fifo_size_arr[0] = 16U;
												fifo_size_arr[1] = 0U;
												break;
	}
}


/**
 * @function	reconfigure_rx_fifo
 *
 * @brief		Reconfigure fifo limits for receiver
 *
 * @param[in]	data_size	-	required size of data
 *
 * @param[out]	NA
 *
 * \par<b>Description:</b><br>
 * 				This function is used to reconfigure fifo size according to
 * 				required length of data
 *
 * <i>Imp Note:</i>
 * 				A local function to reconfigure Receive FIFO with the given
 * 				size and trigger limit. Size is needed because the FIFO should
 * 				be disabled before changing the trigger limit by clearing the FIFO size.
 *
 */
void UART__::reconfigure_rx_fifo( unsigned long data_size ) const
{
	/* Local Variables */
	unsigned long fifo_size = 0U;
	unsigned long ret_limit_val = 0U;

	/* Get FIFO size in bytes */
	fifo_size = (unsigned long) ( 0x01UL << (unsigned char) (config.fifo_cfg.rx_fifo_size) );
	/* If data size is more than FIFO size, configure the limit to the FIFO size */
	ret_limit_val = ( data_size < fifo_size ) ? data_size - 1U : fifo_size - 1U;
	/* Set the limit value */
	XMC_USIC_CH_RXFIFO_SetSizeTriggerLimit( context.p_channel,
											config.fifo_cfg.rx_fifo_size,
											ret_limit_val
											);
}


#endif	/* UART_CHANNELS_USED */


/******* ISR entries for UART__ class objects *******/

/**
 * @function	tx_isr_entry
 *
 * @brief		transmit handler function for UART transmission interrupt
 *
 * @param[in]	UART object handle
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
void UART__::tx_isr_entry( UART__* p_handle )
{
	/* Make sure that user has asked to transmit data */
	if( (1 == p_handle->control.state.tx_lock) && (1 == p_handle->control.state.tx_busy) )
	{
		/* Check data sent */
		if( p_handle->control.tx_data_index < p_handle->control.tx_data_count )
		{
			/* Check FIFO used or not */
			if( p_handle->config.fifo_cfg.tx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
				/* Fill FIFO and send data */
				while( 0 == XMC_USIC_CH_TXFIFO_IsFull( p_handle->context.p_channel ) )
				{
					if( p_handle->control.tx_data_index < p_handle->control.tx_data_count )
					{
						/* Load the FIFO byte by byte till either FIFO is full or all data is loaded */
						XMC_UART_CH_Transmit( p_handle->context.p_channel, (unsigned short)
								p_handle->control.p_tx_data[p_handle->control.tx_data_index] );
						/* Increment data index */
						++(p_handle->control.tx_data_index);
					}
					else
					{
						/* FIFO has been filled completely or data has been loaded into it */
						break;
					}
				}
			}
			else/* Send directly */
			{
				XMC_UART_CH_Transmit( p_handle->context.p_channel,
						p_handle->control.p_tx_data[p_handle->control.tx_data_index] );
				++(p_handle->control.tx_data_index);
			}
		}
		else/* Data is fully sent, ensure it and complete the transfer */
		{
			/* Check FIFO empty status now */
			if( 1 == XMC_USIC_CH_TXFIFO_IsEmpty( p_handle->context.p_channel ) )
			{
				if( p_handle->config.fifo_cfg.tx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
				{
					/* Disable fifo transmit event event */
					XMC_USIC_CH_TXFIFO_DisableEvent( p_handle->context.p_channel,
							(unsigned long) XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD
							);
				}
				else
				{
					/* Disable standard transmit event */
					XMC_USIC_CH_DisableEvent( p_handle->context.p_channel, (unsigned long)
												XMC_USIC_CH_EVENT_TRANSMIT_BUFFER
												);
				}

				/* Wait for transmit buffer to get free to ensure data has been sent */
				while( XMC_USIC_CH_TBUF_STATUS_BUSY == XMC_USIC_CH_GetTransmitBufferStatus( p_handle->context.p_channel ) );


				/* Call callback if available */
				if(
					#if( 0 == UART_CALLBACK_HANDLER_USED )
						p_handle->callback != nullptr
					#else
						1
					#endif
					)
				{
					p_handle->fp_callback( p_handle->channel, (unsigned char *)(p_handle->control.p_tx_data),
							eUART_Event_::UART_EVENT_TX_COMPLETE );
				}

				/* Release resources for transmission */
				p_handle->control.p_tx_data = nullptr;
				p_handle->control.state.tx_busy = 0;
				p_handle->control.state.tx_lock = 0;
			}
		}
	}
}


/**
 * @function	rx_isr_direct
 *
 * @brief		Receive handler function for UART receiver handler for
 * 				direct ISR requests
 *
 * @param[in]	UART object handle
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
void UART__::rx_isr_direct( UART__* p_handle )
{
	/* Receive data directly and pass it to callback,
	 * for this operation events for receiver should always be enabled
	 *
	 * TODO: If need this functionality then enable receiver events for direct use
	 *  */
	unsigned char data_l = 255;

	/* Check for FIFO usage */
	if( p_handle->config.fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
	{
		/* Check if FIFO is empty or not */
		if( 0 == XMC_USIC_CH_RXFIFO_IsEmpty( p_handle->context.p_channel ) )
		{
			data_l = (unsigned char)XMC_UART_CH_GetReceivedData( p_handle->context.p_channel );
		}
	}
	else
	{
		/* Receive data */
		data_l = (unsigned char)XMC_UART_CH_GetReceivedData( p_handle->context.p_channel );
	}

	/* Call callback */
	if(
		#if( 0 == UART_CALLBACK_HANDLER_USED )
			p_handle->callback != nullptr
		#else
			1/* true case if callback handler is defined, no need to check */
		#endif
		)
	{
		p_handle->fp_callback( p_handle->channel, &data_l, eUART_Event_::UART_EVENT_RX_CHAR );
	}
}


/**
 * @function	rx_isr_entry
 *
 * @brief		Receive handler function for UART receiver handler
 *
 * @param[in]	UART object handle
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
void UART__::rx_isr_entry( UART__* p_handle )
{
	if( (1 == p_handle->control.state.rx_lock) && (1 == p_handle->control.state.rx_busy) )
	{
		/* Check for FIFO usage */
		if( p_handle->config.fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
		{
			/* Check FIFO empty status */
			while( 0 == XMC_USIC_CH_RXFIFO_IsEmpty( p_handle->context.p_channel ) )
			{
				/* Receive data */
				if( p_handle->control.rx_data_index < p_handle->control.rx_data_count )
				{
					p_handle->control.p_rx_data[p_handle->control.rx_data_index] =
						(unsigned char)XMC_UART_CH_GetReceivedData( p_handle->context.p_channel );
					/* Increment data index */
					++(p_handle->control.rx_data_index);
				}
				/* Check for data count and index equality */
				if( p_handle->control.rx_data_index == p_handle->control.rx_data_count )
				{
					/* Call callback function if available */
					if(
						#if( 0 == UART_CALLBACK_HANDLER_USED )
							p_handle->callback != nullptr
						#else
							1/* true case if callback handler is defined, no need to check */
						#endif
						)
					{
						p_handle->fp_callback( p_handle->channel, static_cast<unsigned char *>(p_handle->control.p_rx_data),
								eUART_Event_::UART_EVENT_RX_COMPLETE );
					}

					/* Release FIFO and reset parameters*/
					p_handle->control.state.rx_busy = 0;
					p_handle->control.state.rx_lock = 0;
					XMC_USIC_CH_RXFIFO_DisableEvent( p_handle->context.p_channel, (unsigned long)
									((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
									(unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE)
									);
					/* Break operation */
					break;
				}
			}
			/* Dynamically setup the trigger limit for FIFO */
			if( p_handle->control.rx_data_index < p_handle->control.rx_data_count )
			{
				p_handle->reconfigure_rx_fifo( p_handle->control.rx_data_count - p_handle->control.rx_data_index );
			}
		}
		else
		{
			/* Check data index */
			if( p_handle->control.rx_data_index < p_handle->control.rx_data_count )
			{
				/* receive data and put it in buffer */
				p_handle->control.p_rx_data[p_handle->control.rx_data_index] =
						(unsigned char)XMC_UART_CH_GetReceivedData( p_handle->context.p_channel );
				/* Update data index */
				++(p_handle->control.rx_data_index);
			}
			else
			{
				/* Call callback function if available */
				if(
					#if( 0 == UART_CALLBACK_HANDLER_USED )
						p_handle->callback != nullptr
					#else
						1/* true case if callback handler is defined, no need to check */
					#endif
					)
				{
					p_handle->fp_callback( p_handle->channel, static_cast<unsigned char *>(p_handle->control.p_rx_data),
							eUART_Event_::UART_EVENT_RX_COMPLETE );
				}

				/* Release rx control */
				p_handle->control.state.rx_busy = 0;
				p_handle->control.state.rx_lock = 0;
				/* Disable events */
				XMC_USIC_CH_DisableEvent( p_handle->context.p_channel, (unsigned long)
							((unsigned long) XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
							(unsigned long) XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE)
							);
			}
		}
	}
	else
	{
		/* Receive data directly from ISR without explicitly asking for it */
		rx_isr_direct( p_handle );
	}
}


#ifdef UART_PROTOCOL_EVENT_USED
/**
 * @function	err_isr_entry
 *
 * @brief		Protocol handler function for UART protocol event handling
 *
 * @param[in]	UART object handle
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
void UART__::err_isr_entry( UART__* p_handle )
{
	/* Local variables */
	unsigned long count_l = 0;
	unsigned long max_val_l = static_cast<unsigned long>(eUART_Event_::UART_EVENT_MAX);
	/* Get event status */
	unsigned long psr_status_l = XMC_UART_CH_GetStatusFlag( p_handle->context.p_channel );
	unsigned long event_cfg_l = p_handle->context.p_channel->PSR_ASCMode;

	/* Check events, if event available then call particular callback one by one */
	for( count_l = 0; count_l < max_val_l; ++count_l )
	{
		if( (event_cfg_l & (unsigned long) uart_event_conf_flags[count_l]) &&
				(psr_status_l & (unsigned long) uart_event_status_flags[count_l]) )
		{
			/* Clear status flag */
			XMC_UART_CH_ClearStatusFlag( p_handle->context.p_channel,
					(unsigned long) uart_event_status_flags[count_l] );
			/* Call Callback */
			if(
				#if( 0 == UART_CALLBACK_HANDLER_USED )
					p_handle->callback != nullptr
				#else
					1/* true case if callback handler is defined, no need to check */
				#endif
				)
			{
				p_handle->fp_callback( p_handle->channel, nullptr, static_cast<eUART_Event_>(count_l) );
			}
			break;
		}
	}
}
#endif

/********************************** End of File *******************************/
