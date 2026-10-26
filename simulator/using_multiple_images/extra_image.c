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
* extra_image.c
*
* Module that is loaded by the main application ("static_part") via the 
* C-SPY Multiple Images feature.
*
* The entry symbol for this project is set to "_extra_image" to avoid 
* including cstartup.
*
* As a post-build action in this project, the tool isymexport is executed to 
* create a symbol table with functions that shall be possible to call from 
* the main application.
*
*          
*/

int calculate_something(int a)
{
  return ++a;
}

int extra_image( int input )
{
  int result=calculate_something(input);
  return result;
}
