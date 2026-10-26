/**************************************************
 *
 * Global data initialization for use with ilink.
 * New style, where each init table routine defines
 * its own format.
 *
 * Copyright 2008 IAR Systems. All rights reserved.
 *
 *          
 *
 **************************************************/

#include "data_init2.h"

#pragma language = extended

#pragma section = "Region$$Table" const
uint32_t const *__iar_copy_init2(uint32_t const * p);
uint32_t const *__iar_zero_init2(uint32_t const * p);

void __iar_data_init2(void)
{
   uint32_t const * p = __section_begin("Region$$Table");
   uint32_t const * pe = __section_end("Region$$Table");
   while (p != pe)
   {
     init_fun_t * fun = (init_fun_t *)*p++;
#pragma calls=__iar_zero_init2
#pragma calls=__iar_copy_init2
     p = fun(p);
   }
}
