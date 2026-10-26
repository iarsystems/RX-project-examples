/* Adapted for use with IAR Embedded Workbench */
/*
 * Copyright (c) 2001, Adam Dunkels
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

#include <stdio.h>
#include <string.h>

#include "screen.h"

#include "uip.h"
#include "vnc-server.h"
#include "vnc-out.h"

#include "ball.h"
#include "main.h"
#include "mt.h"
#include "pong.h"
#include "mandel.h"
#include "ball.h"

/* Objects to be sent to VNC client - the bouncing balls */
DRAW_OBJECT vncBall[DRAWBALLS];
int g_iYLine = 0;
int g_iAckLine = 0;
int g_iAgain = 0;

#if CTK_CONF_MOUSE_SUPPORT
static int mouse_x, mouse_y, mouse_button;
#endif

/*-----------------------------------------------------------------------------------*/
void
vnc_out_init(void)
{
  g_iYLine   = 0;
  g_iAckLine = 0;
  g_iAgain   = 0;

  if (g_ucVncLastDemo)
  {
    switch (g_ucDemoSel)
    {
      case DEMO_BALLS:
      drawBallInitialScreen();
    break;
    case DEMO_GAME:
        initPong();
      break;
    case DEMO_MANDELBROT:
      mandelbrot_init();
      break;
    }
  g_ucDemoSel = 0;
  }
}

/*-----------------------------------------------------------------------------------*/
static void
init_send_screen(vnc_server_state *vs)
{
  vs->sendmsg = SEND_SCREEN;
}
/*-----------------------------------------------------------------------------------*/
static void
check_updates(vnc_server_state *vs)
{
  vs->sendmsg = SEND_UPDATE;
}
/*-----------------------------------------------------------------------------------*/
void
vnc_out_new(vnc_server_state *vs)
{
}
/*-----------------------------------------------------------------------------------*/
void
vnc_out_send_blank(vnc_server_state *vs)
{
  vs->sendmsg = SENT_BLANK;
}
/*-----------------------------------------------------------------------------------*/
void
vnc_out_send_screen(vnc_server_state *vs)
{
  vnc_out_send_update(vs);
}
/*-----------------------------------------------------------------------------------*/
#define VNCLINES  4
void
vnc_out_send_update(vnc_server_state *vs)
{
  if (g_iYLine<Y_SIZE)
  {
  unsigned nLines = 0;
  unsigned dy = 0;
  
  disable_tasktimertick();
  
  /* RealVNC seems to only accept at most 168 raw pixels in one VNC pixel block. */
  /* We pack these in an 4 block VNC message. We need to calculate how many      */
  /* lines we need to get the maximum of out 4x168 pixels:                       */
  /*     n * X_SIZE = 4 * 168                                                    */
  /* so  n          = 4 * 168 / X_SIZE                                           */
  nLines = (VNCLINES * VNC_MAXPIXELS) / X_SIZE;

  if (g_iYLine+nLines>Y_SIZE) dy = Y_SIZE - g_iYLine;
  else                        dy = nLines;
  
  for (unsigned l=0; l<VNCLINES; l++)
  {
    vncBall[l].x      = (l*X_SIZE)/VNCLINES;
      vncBall[l].y      = g_iYLine;
      vncBall[l].dx     = X_SIZE/VNCLINES;
      vncBall[l].dy     = dy;
    vncBall[l].valid  = 1;
  }
  // uIP requires us to resend a packet if it's lost. We wait for an
  // ack after sending a (VNC) packet. If ack-ed, g_iYLine gets
  // assigned g_iAckLine.
  g_iAckLine          = g_iYLine + nLines;
    
    vnc_draw_objects(vncBall, VNCLINES);

    enable_tasktimertick();
  }
  else
  {
    vnc_draw_objects(vncBall, 8);
  }
}

int g_iBlockSent = 0;

void vnc_draw_objects(DRAW_OBJECT *_pDrawObject, int _nNumObjects)
{
  u8_t msglen;
  u16_t len, n;
  u8_t *ptr;
  struct rfb_fb_update *umsg;
  register struct rfb_fb_update_rect_hdr *recthdr;
  u8_t *pixeldata;
  int   i,x,y;

  disable_tasktimertick();

  /* First, check if we need to feed the update function with a new
     pending update. */
//  check_updates(vs);

  disable_tasktimertick();
  
  umsg = (struct rfb_fb_update *)uip_appdata;

  umsg->type = RFB_FB_UPDATE;

  n = 0;
  msglen = 0;
  ptr = (u8_t *)umsg + sizeof(struct rfb_fb_update);
  len = sizeof(struct rfb_fb_update);

  i = 0;  
  while (i<_nNumObjects)
  {
     if (_pDrawObject[i].valid)
   {
     
       /* First we must make sure that there is 
          enough space in the outgoing TCP segment. */

       msglen = sizeof(struct rfb_fb_update_rect_hdr) + _pDrawObject[i].dx*_pDrawObject[i].dy;
       if (msglen >= uip_mss() - len)
       {
         /* ... and we break out of the loop. */
         goto drawobjects_end;    
       }

       recthdr = (struct rfb_fb_update_rect_hdr *)ptr;

       recthdr->rect.x      = htons(_pDrawObject[i].x);
       recthdr->rect.y      = htons(_pDrawObject[i].y);
       recthdr->rect.w      = HTONS(_pDrawObject[i].dx);
       recthdr->rect.h      = HTONS(_pDrawObject[i].dy);
       recthdr->encoding[0] = 0;
       recthdr->encoding[1] = 0;
       recthdr->encoding[2] = 0;
       recthdr->encoding[3] = RFB_ENC_RAW;

       pixeldata = (u8_t *)recthdr + sizeof(struct rfb_fb_update_rect_hdr);
     for (y=_pDrawObject[i].y; y<(_pDrawObject[i].y+_pDrawObject[i].dy); y++)
     {
       for (x=_pDrawObject[i].x; x<(_pDrawObject[i].x+_pDrawObject[i].dx); x++)
         {
           *pixeldata++ = g_ucFrameBuf[y][x];
         }
     }
  
       len += msglen;
       ptr += msglen;
       n++;
     /* Indicate we have finished with this rect */
     _pDrawObject[i].valid = 0;
   }
   i++;
  }

drawobjects_end:

  if (n)
  {
    umsg->rects = htons(n);
  
    uip_send(uip_appdata, len);
  }
  
  g_iBlockSent = 1;
  
  enable_tasktimertick();
}

/*-----------------------------------------------------------------------------------*/

char
vnc_out_keyavail(void)
{
  return 0;
}

char
vnc_out_getkey(void)
{
 
  return 0;
}

void
vnc_out_key_event(vnc_server_state *vs)
{
  register struct rfb_key_event *ev;
  
  ev = (struct rfb_key_event *)uip_appdata;

  if(ev->down != 0) {
    if(vs->sendmsg == SEND_NONE) 
  {
      vs->sendmsg = SEND_UPDATE;
    }
  }

  check_updates(vs);
}
/*-----------------------------------------------------------------------------------*/
int g_iSpeed = 0;
void
vnc_out_pointer_event(vnc_server_state *vs)
{
  struct rfb_pointer_event *ev;
  u16_t evx, evy;
  
  ev = (struct rfb_pointer_event *)uip_appdata;

  evx = htons(ev->x);
  evy = htons(ev->y);
  
  if (ev->buttonmask != 0)
  {
  // If mouse clicked in right-bottom corner, increment speed
  if ((evx>=X_SIZE-10)&&(evx<=X_SIZE)&&
      (evy>=Y_SIZE-10)&&(evy<=Y_SIZE)  )
  {
    g_iSpeed++;
    switch (g_ucVncLastDemo)
    {
    case DEMO_BALLS:
      adjust_speed(+1);
      break;
    case DEMO_GAME:
    break;
    default:
    break;
    }
    }
  // If mouse clicked in left-bottom corner, decrement speed
  if (/*(evx>=0        )&&*/(evx<=10    )&&
        (evy>=Y_SIZE-10)&&  (evy<=Y_SIZE)  )
  {
    g_iSpeed--;
    switch (g_ucVncLastDemo)
    {
    case DEMO_BALLS:
      adjust_speed(-1);
      break;
    case DEMO_GAME:
    break;
    default:
    break;
    }
  }
  }
    
//    check_updates(vs);    
}
/*-----------------------------------------------------------------------------------*/
void
vnc_out_acked(vnc_server_state *vs)
{
  if(vs->state != VNC_RUNNING)
  {
    return;
  }

  // If still sending initial screen, increment to next block.  
  g_iYLine = g_iAckLine;
  
  if (vs->sendmsg == SENT_BLANK)
  {
    init_send_screen(vs);
  } 
  else
  if (vs->sendmsg == SEND_BLANK)
  {
    /* Do nothing until sendmsg == SENT_BLANK. */
  }
  else
  if (vs->sendmsg == SEND_SCREEN)
  {
    /* When the screen has been fully drawn, ->x2 and ->y2 are both
       set to 0 to indicate this.*/
  }
  else
  if (vs->sendmsg == SEND_UPDATE) 
  {
      check_updates(vs);
  }
  else
  {
    vs->sendmsg = SEND_NONE;
  }
}
/*-----------------------------------------------------------------------------------*/
void
vnc_out_poll(vnc_server_state *vs)
{
  if(vs->state == VNC_RUNNING &&
     vs->sendmsg == SEND_NONE) {
    check_updates(vs);
    vnc_server_send_data(vs);
  }
  else
  if(vs->state == VNC_RUNNING)
  {
    vnc_server_send_data(vs);
  }
}
/*-----------------------------------------------------------------------------------*/
#if CTK_CONF_MOUSE_SUPPORT
void
ctk_mouse_init(void)
{

}
/*-----------------------------------------------------------------------------------*/
unsigned short
ctk_mouse_x(void)
{
  return mouse_x;
}
/*-----------------------------------------------------------------------------------*/
unsigned short
ctk_mouse_y(void)
{
  return mouse_y;
}
/*-----------------------------------------------------------------------------------*/
unsigned char
ctk_mouse_button(void)
{
  return mouse_button;
}
/*-----------------------------------------------------------------------------------*/
void
ctk_mouse_hide(void)
{
}
/*-----------------------------------------------------------------------------------*/
void
ctk_mouse_show(void)
{
}
/*-----------------------------------------------------------------------------------*/
#endif /* CTK_CONF_MOUSE_SUPPORT */
