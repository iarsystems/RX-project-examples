#include <machine.h>

// Override the default interrupt handler for __interrupt_0
// BRK
__interrupt void __interrupt_0()
{
    wait();
}