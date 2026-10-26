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
* IAR Embedded Workbench example project
*
* main.c
*
* C-SPY macro example that shows how to set breakpoints and perform actions
* at breakpoint hits without stopping execution. View the macro file 
* setBreak.mac and the result output in the Debug Log window! 
*
*          
*/

#define TWO_POWER 13

int result;
void multiplyByTwo(void);

int main(int i)
{
  for (i = 0, result = 1; i < TWO_POWER; i++)
  {
    multiplyByTwo();
  }
  return result;
}

void multiplyByTwo(void)
{
  result *= 2;
}
