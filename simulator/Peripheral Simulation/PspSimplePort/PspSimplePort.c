/*
 * Copyright (C) 2009 IAR Systems AB.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * IAR Embedded Workbench peripheral simulation example
 *
 * PspSimplePort.c
 *
 * Simple example using a button, a led and a simple Port.
 *
 *          
 */

#include <stdio.h>
#include "intrinsics.h"

#include <iorx610.h>

void main()
{
  printf( "start\n" );
  
  // initialise PortD to have bit0-3 as input and 4-7 as output
  // if you nicely step this code you can see changes in the Port window
  PORTD.DDR.BYTE = 0xF0;

  // never-ending loop
  while(1)
  {
    __no_operation();
    
    // move input2 to output4 for PD
    // when button is pressed the LED will light up
    char button  = PORTD.PORT.BIT.B2;
    PORTD.DR.BIT.B4 = button;
  }
}
