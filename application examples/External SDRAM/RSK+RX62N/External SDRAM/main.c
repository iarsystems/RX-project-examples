/*
 * Copyright (c) 20010 IAR Systems AB.
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
 * IAR Embedded Workbench tutorial
 *
 * Example that show how a function and variables can be placed in the
 * external RAM on the RSKRX62N.
 *
 *          
 */

/* Create a section */
#pragma section = "EXTERNAL_RAM"

/* Place a variable in that section. */
int myVariable @ "EXTERNAL_RAM";
int anotherVar @ "EXTERNAL_RAM";

/* Place following function in the EXTERNAL_RAM section */
#pragma location = "EXTERNAL_RAM"
void MyFunctionInExternalRAM(void)
{
  anotherVar ++;
}

/* main will be placed in ROM */
int main(void)
{
  while(1)
  {
    myVariable++;
    
    MyFunctionInExternalRAM();
  }
}