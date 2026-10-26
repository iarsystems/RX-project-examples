/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* File Name    : uart.h
* Version     : 1
* Device     : Renesas RX62N MCU.
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : YRDKRX62N
* Description   : header file for UART driver using RX62N SCI peripheral.
*******************************************************************************/
/*******************************************************************************
* History     : 09.14.2011 Ver. 1   
*******************************************************************************/
#ifndef UART_H // multiple inclusion guard
#define UART_H

#define NEWLINE_CHAR    '\n'
#define RETURN_CHAR     '\r'

void     sci_uart_init(void);
void     sci_put_char(uint8_t write_data);
uint8_t  sci_get_char(void);
void     sci_put_str(uint8_t *source_string);
uint32_t sci_read_count_get  (void);
void     sci_tx_interrupt_enable (void);
void     sci_rx_interrupt_enable (void);
void     sci_tx_interrupt_disable(void);
void     sci_rx_interrupt_disable(void);

#endif
