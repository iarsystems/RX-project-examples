/**************************************************
 *
 * Code that performs copy initialization of global data.
 * New style init table.
 *
 * Copyright 2008-2018 IAR Systems. All rights reserved.
 *
 *          
 *
 **************************************************/

#include <stdint.h>
#include "intrinsics.h"

#pragma language = extended

/* Format:
  Size1
  SrcAddr1
  DestAddr1
  Size2
  SrcAddr2
  DestAddr2
  ...
  0
*/

uint32_t const *
__iar_copy_init2(uint32_t const * p);

uint32_t const *
__iar_copy_init2(uint32_t const * p)
{
  uint32_t size; 
  while ((size = *p++) != 0)
  {
    char const * src = (char const *)((char const *)p + (uint32_t)*p);
    p++;
    char * dest;
    dest = (char *)*p++;
#if __RWPI__ == 1
    if (size & 1)
      dest = (char *)(__s_base() + (uint32_t)dest);
#endif
    for(uint32_t i = size >> 1; i; --i)
      *dest++ = *src++;
  }
  return p;
}
