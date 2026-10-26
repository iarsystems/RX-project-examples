/**
 * \file  io_baord_define.h
 * \brief Header file to include board specific definations
 *
 * This file includes the correct cpu specific io define file.
 *
 * \internal Author, version, and date are provided by SVN.  Do not edit \endinternal
 * $Author$
 *          
 * $Date$
 *
 * (C)2011 INOVA Geophysical Equipment
 * All Rights Reserved
 *
 */

#ifndef  IO_BOARD_DEFINE_H_
#define  IO_BOARD_DEFINE_H_

#ifdef __BOARD_VIBCOM_PROTOTYPE__
  #define  __CPU_RX62N__
  #include "io_cpu_define.h"

  #define UART_FIFO_SIZE           64u
  #define UART_TX_TRIGGER          32u //Must be div by 4
  #define UART_RX_TRIGGER          32u //Must be div by 4

  #define UART_BAUD_RATE           115200ul
  #define UART_INPUT_CLK           26000000ul //MHZ

  enum Interrupt_priorities
  {
    UART_INT_PRIORITY = 3u,
  };

  #define EXT_FLASH1_BASE_ADDRESS  CS2_BASE_ADDRESS
  #define EXT_FLASH2_BASE_ADDRESS  CS3_BASE_ADDRESS
  #define EXT_SRAM_BASE_ADDRESS    CS4_BASE_ADDRESS
  #define EXT_NVRAM_BASE_ADDRESS   CS5_BASE_ADDRESS
  #define ETHERNET_BASE_ADDRESS    CS6_BASE_ADDRESS
  #define UART_BASE_ADDRESS        CS7_BASE_ADDRESS
#else
  #error CPU type not supported
#endif

#endif //IO_CPU_DEFINE_H_