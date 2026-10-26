/*
 * Copyright 2013 IAR Systems AB.
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
 * RSK+RX63N SDRAM example
 *
 * main.c
 *
 * Example that uses the SDRAM for some variables.
 *
 *          
 */

#include "intrinsics.h"

/* define a macro that can be used to specify the location of a variable */
#define _SDRAM _Pragma("location=\".SDRAM\"")

/* Variables will be placed in SDRAM*/
int myVariable @ ".SDRAM";
_SDRAM int anotherVar;

/* Main program.
 * Prints the Fibonacci sequence.
 */
void main(void)
{
  /* Infinite while loop */
  while(1)
  {
    myVariable++;
    anotherVar--;
  }
}
