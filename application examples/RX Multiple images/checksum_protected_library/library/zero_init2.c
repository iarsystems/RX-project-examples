/**************************************************
 *
 * Code that performs zero initialization of global data.
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
  DestAddr1
  Size2
  DestAddr2
  ...
  0
*/

uint32_t const *
__iar_zero_init2(uint32_t const * p);


uint32_t const *
__iar_zero_init2(uint32_t const * p)
{
  uint32_t size;
  while ((size = *p++) != 0)
  {
    uint32_t * dest;
    dest = (uint32_t *)*p++;
#if __RWPI__ == 1
    if (size & 1)
      dest = (uint32_t *)(__s_base() + (uint32_t)dest);
#endif
    for(uint32_t i = size >> 3; i; --i)
      *dest++ = 0;
  }
  return p;
}
