#include "lib.h"

/*
 * This application project imports the library symbols:
 * Options -> Linker -> Additional libraries 
 * ($PROJ_DIR$\..\protected_lib\protected_lib.symbols)
 *
*/

extern const unsigned short _checksum;
extern const unsigned int   _checksum_begin;
extern const unsigned int   _checksum_end;

__root int app_var1 = 2;
__root int app_var2 = 0;

extern unsigned short slow_crc16(unsigned short sum, 
                                 unsigned char *p, 
                                 unsigned int len);

int main(void)
{
    int a = 0;
    unsigned short calc = 0;
    unsigned char zeros[2] = {0, 0};

    /* Calculate the ROM library checksum */

    calc = slow_crc16(calc,(unsigned char *)&_checksum_begin, 
                     (unsigned int)&_checksum_end - 
                     (unsigned int)&_checksum_begin + 1);
    
    calc = slow_crc16(calc, zeros, 2);
    
    /* Compare the calculated checksum with the stored */
    if (_checksum != calc)
    {
        /* Checksum did not match! */
        return -1;
    }

    /* Initialize the library */
    lib_init();

    /* Call a library function */
    a = lib_test_func(2);
    
    /* Use library data */
    a += lib_data_arr_rom[1];
    a += lib_data_arr_ram[1];
    a += lib_data_zero;
    
    return a;
}
