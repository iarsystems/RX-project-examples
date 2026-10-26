#include "lib.h"
/*
 * This library project exports its symbols:
 * Options -> Build actions -> Post-build command line:
 * $TOOLKIT_DIR$\bin\isymexport.exe 
 *    "$TARGET_PATH$" "$PROJ_DIR$\protected_lib.symbols"
 *  --edit "$PROJ_DIR$\sym_export.txt"
 *
*/

static int lib_test_var = 20;

__root int lib_test_func(int a)
{
    return lib_test_var + a;
}

extern void __iar_data_init2(void);

__root int lib_init(void)
{
    /* Perform zero initializion and 
    *  copy initialized data from ROM to RAM */

    __iar_data_init2();

    return 0;
}
