/* Adapted for use with IAR Embedded Workbench */
/*
 * Copyright (c) 2001, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Adam Dunkels.
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 *
 *
 */
 
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "iodefine.h"
#include "typedefine.h"


#include "uip.h"
#include "uip_arp.h"
// Renesas -- #include "network-device.h"
#include "hwEthernet.h"
#include "httpd.h"
#include "timer.h"
#include "uip_app.h"
#include "lcd.h"

#include "main.h"
#include "mt.h"

#include "ball.h"
#include "pong.h"
#include "mandel.h"
#include "vnc-server.h"

#include "i2cprom.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern unsigned char LEDflag;

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
uint8_t g_uiMACAddr[48];

/******************************************************************************
Private variables and functions
******************************************************************************/
// MAC address configuration
struct uip_eth_addr my_mac;

void init_dry(void);

/******************************************************************************
Mandelbrot thread private global struct
******************************************************************************/
static struct mt_thread mandelbrot_thread;

/******************************************************************************
* Function Name: ResumeMandelbrot
* Description  : Resumes (starts/runs) the Mandelbrot demo thread
* Arguments    : none
* Return Value : int
******************************************************************************/
void ResumeMandelbrot(void)
{
   mt_resume(&mandelbrot_thread);
}

/******************************************************************************
* Function Name: ResumeMandelbrot
* Description  : Resumes (starts/runs) the Mandelbrot demo thread
* Arguments    : none
* Return Value : int
******************************************************************************/
void SuspendMandelbrot(void)
{
   mt_suspend(&mandelbrot_thread);
}

/******************************************************************************
Balls demo thread private global structs
******************************************************************************/
static struct mt_thread fpuBall_thread;
static struct mt_thread theBall_thread;
static struct mt_thread drawBall_thread;

/******************************************************************************
* Function Name: ResumeBalls
* Description  : Resumes (starts/runs) the Balls demo threads
* Arguments    : none
* Return Value : int
******************************************************************************/
void ResumeBalls(void)
{
   mt_resume(&theBall_thread);
   mt_resume(&fpuBall_thread);
   mt_resume(&drawBall_thread);
}

/******************************************************************************
* Function Name: SuspendBalls
* Description  : Suspends (stops) the Balls demo threads
* Arguments    : none
* Return Value : int
******************************************************************************/
void SuspendBalls(void)
{
   mt_suspend(&drawBall_thread);
   mt_suspend(&fpuBall_thread);
   mt_suspend(&theBall_thread);
}

/******************************************************************************
Pong demo thread private global structs
******************************************************************************/
static struct mt_thread pong_thread;

/******************************************************************************
* Function Name: ResumePong
* Description  : Resumes (starts/runs) the Pong demo threads
* Arguments    : none
* Return Value : int
******************************************************************************/
void ResumePong(void)
{
   mt_resume(&pong_thread);
}

/******************************************************************************
* Function Name: SuspendPong
* Description  : Suspends (stops) the Pong demo threads
* Arguments    : none
* Return Value : int
******************************************************************************/
void SuspendPong(void)
{
   mt_suspend(&pong_thread);
}

/******************************************************************************
* Function Name: SuspendAllDemos
* Description  : Suspends (stops) all demo threads
* Arguments    : none
* Return Value : int
******************************************************************************/
void SuspendAllDemos(void)
{
  SuspendMandelbrot();
  SuspendBalls();
  SuspendPong();
}

/******************************************************************************
* Function Name: amend_serialnum_descriptor
* Description  : amends serial number descriptor
* Arguments    : none
* Return Value : none
******************************************************************************/
void amend_serialnum_descriptor(void)
{
   char buf[32];
   sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", (my_mac.addr[0])&0xff,
                                     (my_mac.addr[1])&0xff,
                                     (my_mac.addr[2])&0xff,
                         (my_mac.addr[3])&0xff,
                                     (my_mac.addr[4])&0xff,
                         (my_mac.addr[5])&0xff);
   for (unsigned i=0; i<strlen(buf); i++)
   {
   g_uiMACAddr[2*i] = buf[i];
   }
}

/******************************************************************************
* Function Name: ReadMacAddressFromI2CEEPROM
* Description  : Retrieves MAC Address from IIC EEPROM
* Arguments    : none
* Return Value : int
******************************************************************************/
void ReadMacAddressFromI2CEEPROM(void)
{
  int iErr   = 0;
  int iTries = 0;
  
  /* Reset I2C addressing, just to be sure */
  i2c_eeprom_reset();
  
  /* Read from I2C EEPROM address 0 */
  iErr = i2c_eeprom_read(0, &my_mac.addr[0], 6);
  while ((iErr)&&(iTries<5))
  {
    /* Upon error, just try a couple of more times */
    
    /* Reset I2C addressing, just to be sure */
    i2c_eeprom_reset();
  
    /* Read from I2C EEPROM address 0 */
    iErr = i2c_eeprom_read(0, &my_mac.addr[0], 6);
    iTries++;
  }

  if (iErr)
  {
    /* Try to tell user there's a problem */
    SetLed2(1);
    my_mac.addr[0] = 0x00;
    my_mac.addr[1] = 0x66;
    my_mac.addr[2] = 0x77;
    my_mac.addr[3] = 0xaa;
    my_mac.addr[4] = 0x44;
    my_mac.addr[5] = 0x55;
  }
  
  amend_serialnum_descriptor();

}

/*----------------------------------------------------------------------------*/ 
/******************************************************************************
* Function Name: uip_main
* Description  : 
* Arguments    : none
* Return Value : int
******************************************************************************/
int uip_main(void)
{
  int i;
  // Renesas -- uip_ipaddr_t ipaddr;
  struct timer periodic_timer, arp_timer;
  uint32_t ch = 0;

  // Renesas ++
//  DisplayuIPDemo();
  ReadMacAddressFromI2CEEPROM();

  init_dry();  /* Initialises string variable */
  
  timer_init();
  timer_set(&periodic_timer, CLOCK_SECOND / 20);
  timer_set(&arp_timer, CLOCK_SECOND * 10);

  // Renesas -- network_device_init();
  /* Wait until Ether device initializes succesfully. 
     Make sure Ethernet cable is plugged in. */ 
  while (R_ETHER_ERROR == R_Ether_Open(ch, (uint8_t*)&my_mac.addr[0]));

  // Renesas ++ set Ethernet address
  uip_setethaddr(my_mac);
  uip_init();

  // Renesas -- 
  //uip_ipaddr(ipaddr, 192,168,0,2);
  //uip_sethostaddr(ipaddr);
  dhcpc_init(&my_mac.addr[0], 6);
  httpd_init();
  
  /* Start Multi-Tasking Framework. Threads are initially suspended. */
  mt_init();
  mt_start(&theBall_thread   , bounce_theBall, NULL);
  mt_start(&fpuBall_thread   , bounce_fpuBall, NULL);
  mt_start(&drawBall_thread  , drawBalls     , NULL);
  mt_start(&pong_thread      , pong          , NULL);
  mt_start(&mandelbrot_thread, mandelbrot    , NULL);  

  
  /* We choose Mandelbrot as the initial demo to run with VNC, so that */
  /* when connecting with a VNC client something is running.           */
  g_ucVncLastDemo = DEMO_MANDELBROT;
  ResumeMandelbrot();
  
  /* Seed random number generation with timer ticks */
  randomSeed();
  
  /* OK, ready to start listening to VNC port 5900 */
  vncd_init();

  while (1)
  {
    // Renesas -- uip_len = network_device_read();
    uip_len = R_Ether_Read(ch, (void *)uip_buf);
    if (uip_len > 0)
    {
      if (BUF->type == htons(UIP_ETHTYPE_IP))
      {
        uip_arp_ipin();
        uip_input();
    
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          uip_arp_out();
          // Renesas -- network_device_send();
          R_Ether_Write(ch, (void *)uip_buf, (uint32_t)uip_len);
        }
      }
      else if (BUF->type == htons(UIP_ETHTYPE_ARP))
      {
        uip_arp_arpin();
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          // Renesas -- network_device_send();
          R_Ether_Write(ch, (void *)uip_buf, (uint32_t)uip_len);
        }
      }

    }
    else if (timer_expired(&periodic_timer))
    {
      timer_reset(&periodic_timer);
      for (i = 0; i < UIP_CONNS; i++)
      {
        uip_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          uip_arp_out();
          // Renesas -- network_device_send();
          R_Ether_Write(ch, (void *)uip_buf, (uint32_t)uip_len);
        }
      }

#if UIP_UDP
      for (i = 0; i < UIP_UDP_CONNS; i++)
      {
        uip_udp_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if (uip_len > 0)
        {
          uip_arp_out();
          // Renesas -- network_device_send();
          R_Ether_Write(ch, (void *)uip_buf, (uint32_t)uip_len);
        }
      }
#endif /* UIP_UDP */

      /* Call the ARP timer function every 10 seconds. */
      if (timer_expired(&arp_timer))
      {
        timer_reset(&arp_timer);
        uip_arp_timer();
      }
    }
    // Insert user aplications here.
    // Call WEB application that controls LEDs on the target board.
    user_app();
  }
  return 0;
}
/*---------------------------------------------------------------------------*/

