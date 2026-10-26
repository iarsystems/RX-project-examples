/**
 * \file  io_cpu_define.h
 * \brief Header file to include the correct cpu io define file
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

#ifndef  IO_CPU_DEFINE_H_
#define  IO_CPU_DEFINE_H_

#ifdef __CPU_RX62N__
  #include "iorx62n.h"
  #include "io_rx62n_port_define.h"

  #define CS2_BASE_ADDRESS ((volatile uint8_t *)0x06000000)
  #define CS3_BASE_ADDRESS ((volatile uint8_t *)0x05000000)
  #define CS4_BASE_ADDRESS ((volatile uint8_t *)0x04000000)
  #define CS5_BASE_ADDRESS ((volatile uint8_t *)0x03000000)
  #define CS6_BASE_ADDRESS ((volatile uint8_t *)0x02000000)
  #define CS7_BASE_ADDRESS ((volatile uint8_t *)0x01000000)
#else
  #error CPU type not supported
#endif

#endif //IO_CPU_DEFINE_H_