/*
* Copyright (C) 2014 IAR Systems AB.
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
* IAR Embedded Workbench example project
*
* main.c
*
* ID code protection, on-chip debugger ID code protection, 
* and ROM code protection
*
*          
*/

// Setting the ID code
#pragma public_equ="__ID_BYTES_1_4",0xFFFFFFFF
#pragma public_equ="__ID_BYTES_5_8",0xFFFFFFFF
#pragma public_equ="__ID_BYTES_9_12",0xFFFFFFFF
#pragma public_equ="__ID_BYTES_13_16",0xFFFFFFFF

// Setting the ROM code protection register:
#pragma public_equ="__ROM_CODE",0xFFFFFFFF

volatile int foo;

int main( void )
{
  return foo;
}
