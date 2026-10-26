/*
* Copyright (C) 2011 IAR Systems AB.
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
* static_main.c
*
* This application loads an extra image, built in the "extra_image" project,
* via the C-SPY Multiple Images feature.
* A symbol table created with isymexport is included when linking to get 
* knowledge about the extra image.
*
*          
*/

#include "stdio.h"
extern int extra_image();

typedef int (*fp_t)(int);

#define IMAGE_OFFSET 0x10000

int main( void )
{
  fp_t fp;
  int result;
  
  fp = (fp_t)((unsigned int)extra_image + IMAGE_OFFSET);
  
  result = fp(1);
  
  printf("The result, calculated in the extra image, is %d.", result);
  
  return result;
}
