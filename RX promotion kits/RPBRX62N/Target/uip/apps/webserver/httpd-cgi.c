/* Adapted for use with IAR Embedded Workbench */
/**
 * \addtogroup httpd
 * @{
 */

/**
 * \file
 *         Web server script interface
 * \author
 *         Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2001-2006, Adam Dunkels.
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
 * 3. The name of the author may not be used to endorse or promote
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

#include "uip.h"
#include "psock.h"
#include "httpd.h"
#include "httpd-cgi.h"
#include "httpd-fs.h"

#include "usb_hid_app.h"
#include "usbdescriptors.h"

#include <stdio.h>
#include <string.h>

extern char          g_szDhrystone_msg[128];
extern uint8_t       g_uiIpAddr[IPADDR_STRING_SIZE];
extern int           g_iLeds;
extern unsigned char g_ucRunHid;

HTTPD_CGI_CALL(file   , "file-stats"     , file_stats);
HTTPD_CGI_CALL(tcp    , "tcp-connections", tcp_stats);
HTTPD_CGI_CALL(net    , "net-stats"      , net_stats);
HTTPD_CGI_CALL(addr   , "ip-address"     , ip_address);
HTTPD_CGI_CALL(dhry   , "dhry-string"    , dhry_string);
HTTPD_CGI_CALL(led0on , "led0-status-on" , led0_status_on);
HTTPD_CGI_CALL(led0off, "led0-status-off", led0_status_off);
HTTPD_CGI_CALL(led1on , "led1-status-on" , led1_status_on);
HTTPD_CGI_CALL(led1off, "led1-status-off", led1_status_off);
HTTPD_CGI_CALL(led2on , "led2-status-on" , led2_status_on);
HTTPD_CGI_CALL(led2off, "led2-status-off", led2_status_off);
HTTPD_CGI_CALL(hidon  , "hid-status-on"  , hid_status_on);
HTTPD_CGI_CALL(hidoff , "hid-status-off" , hid_status_off);

static const struct httpd_cgi_call *calls[] = { &file, &tcp, &net, &addr, &dhry, &led0on, &led0off, &led1on, &led1off, &led2on, &led2off, &hidon, &hidoff, NULL };

/*---------------------------------------------------------------------------*/
static
PT_THREAD(nullfunction(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
httpd_cgifunction
httpd_cgi(char *name)
{
  const struct httpd_cgi_call **f;

  /* Find the matching name in the table, return the function. */
  for(f = calls; *f != NULL; ++f) {
    if(strncmp((*f)->name, name, strlen((*f)->name)) == 0) {
      return (*f)->function;
    }
  }
  return nullfunction;
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_file_stats(void *arg)
{
  char *f = (char *)arg;
  // Renesas -- return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE, "%5u", httpd_fs_count(f));
  return sprintf((char *)uip_appdata, "%5u", httpd_fs_count(f));
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(file_stats(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_file_stats, strchr(ptr, ' ') + 1);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static const char closed[] =   /*  "CLOSED",*/
{0x43, 0x4c, 0x4f, 0x53, 0x45, 0x44, 0};
static const char syn_rcvd[] = /*  "SYN-RCVD",*/
{0x53, 0x59, 0x4e, 0x2d, 0x52, 0x43, 0x56,
 0x44,  0};
static const char syn_sent[] = /*  "SYN-SENT",*/
{0x53, 0x59, 0x4e, 0x2d, 0x53, 0x45, 0x4e,
 0x54,  0};
static const char established[] = /*  "ESTABLISHED",*/
{0x45, 0x53, 0x54, 0x41, 0x42, 0x4c, 0x49, 0x53, 0x48,
 0x45, 0x44, 0};
static const char fin_wait_1[] = /*  "FIN-WAIT-1",*/
{0x46, 0x49, 0x4e, 0x2d, 0x57, 0x41, 0x49,
 0x54, 0x2d, 0x31, 0};
static const char fin_wait_2[] = /*  "FIN-WAIT-2",*/
{0x46, 0x49, 0x4e, 0x2d, 0x57, 0x41, 0x49,
 0x54, 0x2d, 0x32, 0};
static const char closing[] = /*  "CLOSING",*/
{0x43, 0x4c, 0x4f, 0x53, 0x49,
 0x4e, 0x47, 0};
static const char time_wait[] = /*  "TIME-WAIT,"*/
{0x54, 0x49, 0x4d, 0x45, 0x2d, 0x57, 0x41,
 0x49, 0x54, 0};
static const char last_ack[] = /*  "LAST-ACK"*/
{0x4c, 0x41, 0x53, 0x54, 0x2d, 0x41, 0x43,
 0x4b, 0};

static const char *states[] = {
  closed,
  syn_rcvd,
  syn_sent,
  established,
  fin_wait_1,
  fin_wait_2,
  closing,
  time_wait,
  last_ack};
  

static unsigned short
generate_tcp_stats(void *arg)
{
  struct uip_conn *conn;
  struct httpd_state *s = (struct httpd_state *)arg;
    
  conn = &uip_conns[s->count];
  /*
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE,
     "<tr><td>%d</td><td>%u.%u.%u.%u:%u</td><td>%s</td><td>%u</td><td>%u</td><td>%c %c</td></tr>\r\n",
     htons(conn->lport),
     htons(conn->ripaddr[0]) >> 8,
     htons(conn->ripaddr[0]) & 0xff,
     htons(conn->ripaddr[1]) >> 8,
     htons(conn->ripaddr[1]) & 0xff,
     htons(conn->rport),
     states[conn->tcpstateflags & UIP_TS_MASK],
     conn->nrtx,
     conn->timer,
     (uip_outstanding(conn))? '*':' ',
     (uip_stopped(conn))? '!':' ');
  */   
  return sprintf((char *)uip_appdata,
     "<tr align=center><td>%d</td><td>%u.%u.%u.%u:%u</td><td>%s</td><td>%u</td><td>%u</td><td>%c %c</td></tr>\r\n",
     htons(conn->lport),
     htons(conn->ripaddr[0]) >> 8,
     htons(conn->ripaddr[0]) & 0xff,
     htons(conn->ripaddr[1]) >> 8,
     htons(conn->ripaddr[1]) & 0xff,
     htons(conn->rport),
     states[conn->tcpstateflags & UIP_TS_MASK],
     conn->nrtx,
     conn->timer,
     (uip_outstanding(conn))? '*':' ',
     (uip_stopped(conn))? '!':' ');      
  
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(tcp_stats(struct httpd_state *s, char *ptr))
{
  
  PSOCK_BEGIN(&s->sout);

  for(s->count = 0; s->count < UIP_CONNS; ++s->count) {
    if((uip_conns[s->count].tcpstateflags & UIP_TS_MASK) != UIP_CLOSED) {
      PSOCK_GENERATOR_SEND(&s->sout, generate_tcp_stats, s);
    }
  }

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_net_stats(void *arg)
{
  struct httpd_state *s = (struct httpd_state *)arg;
  /*
  return snprintf((char *)uip_appdata, UIP_APPDATA_SIZE,
      "%5u\n", ((uip_stats_t *)&uip_stat)[s->count]);
  */
  return sprintf((char *)uip_appdata,
      "%5u\n", ((uip_stats_t *)&uip_stat)[s->count]);

}

static
PT_THREAD(net_stats(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

#if UIP_STATISTICS

  for(s->count = 0; s->count < sizeof(uip_stat) / sizeof(uip_stats_t);
      ++s->count) {
    PSOCK_GENERATOR_SEND(&s->sout, generate_net_stats, s);
  }
  
#endif /* UIP_STATISTICS */
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_ip_address(void *arg)
{
  char ucIpAddr[IPADDR_STRING_SIZE/2+1];
  int  i;
  for (i=0; i<IPADDR_STRING_SIZE/2; i++)
  {
    /* The g_uiIpAddr is UNICODE, e.g. two bytes per letter.         */
    /* This copy operation copies only the first byte of each letter. */
    ucIpAddr[i] = g_uiIpAddr[i*2];
  }
  ucIpAddr[IPADDR_STRING_SIZE/2] = 0;
  return sprintf((char *)uip_appdata, "%s", ucIpAddr);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(ip_address(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_ip_address, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static unsigned short
generate_dhry_string(void *arg)
{
  return sprintf((char *)uip_appdata, "%s", g_szDhrystone_msg);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(dhry_string(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_dhry_string, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_led0_status_on(void *arg)
{
  if (g_iLeds & 1) 
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDA\" VALUE=\"On\" CHECKED>");
  }
  else
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDA\" VALUE=\"On\">");
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(led0_status_on(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_led0_status_on, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_led0_status_off(void *arg)
{
  if (g_iLeds & 1) 
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDA\" VALUE=\"Off\">");
  }
  else
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDA\" VALUE=\"Off\" CHECKED>");
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(led0_status_off(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_led0_status_off, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_led1_status_on(void *arg)
{
  if (g_iLeds & 2) 
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDB\" VALUE=\"On\" CHECKED>");
  }
  else
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDB\" VALUE=\"On\">");
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(led1_status_on(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_led1_status_on, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_led1_status_off(void *arg)
{
  if (g_iLeds & 2) 
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDB\" VALUE=\"Off\">");
  }
  else
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDB\" VALUE=\"Off\" CHECKED>");
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(led1_status_off(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_led1_status_off, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_led2_status_on(void *arg)
{
  if (g_iLeds & 4) 
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDC\" VALUE=\"On\" CHECKED>");
  }
  else
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDC\" VALUE=\"On\">");
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(led2_status_on(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_led2_status_on, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_led2_status_off(void *arg)
{
  if (g_iLeds & 4) 
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDC\" VALUE=\"Off\">");
  }
  else
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"LEDC\" VALUE=\"Off\" CHECKED>");
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(led2_status_off(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_led2_status_off, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_hid_status_on(void *arg)
{
  if (g_ucRunHid &= 1) 
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"HIDMOUSE\" VALUE=\"On\" CHECKED>");
  }
  else
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"HIDMOUSE\" VALUE=\"On\">");
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(hid_status_on(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_hid_status_on, NULL);
  
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static unsigned short
generate_hid_status_off(void *arg)
{
  if (g_ucRunHid &= 1) 
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"HIDMOUSE\" VALUE=\"Off\">");
  }
  else
  {
    return sprintf((char *)uip_appdata, "<INPUT TYPE=\"RADIO\" NAME=\"HIDMOUSE\" VALUE=\"Off\" CHECKED>");
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(hid_status_off(struct httpd_state *s, char *ptr))
{
  PSOCK_BEGIN(&s->sout);

  PSOCK_GENERATOR_SEND(&s->sout, generate_hid_status_off, NULL);
  
  PSOCK_END(&s->sout);
}
/** @} */
