#include <stdio.h>

#include <iorx610.h>
#include "fixmath.h"

void main()
{
    float r_flt, a;
    FIX16 a16, b16, r16;

    // Calculate square root.
    a16 = FIX16_fromfloat(9.0f);
    r16 = FIX16_sqrt(a16);
    r_flt = FIX16_tofloat(r16);
    printf("sqrt: %.3f\n", r_flt);

    // Calculate cos for pi/4.
    a = 3.14159265359f / 4.0f;
    a16 = FIX16_fromfloat(a);
    r16 = FIX16_cos(a16);
    r_flt = FIX16_tofloat(r16);
    printf("cos: %.3f, ", r_flt);

    // Calculate cos for pi.
    a = 3.14159265359f;
    a16 = FIX16_fromfloat(a);
    r16 = FIX16_cos(a16);
    r_flt = FIX16_tofloat(r16);
    printf("cos: %.3f\n", r_flt);

    // Calculate sin for pi/4.
    a = 3.14159265359f / 4.0f;
    a16 = FIX16_fromfloat(a);
    r16 = FIX16_sin(a16);
    r_flt = FIX16_tofloat(r16);
    printf("sin: %.3f, ", r_flt);

    // Calculate sin for pi.
    a = 3.14159265359f;
    a16 = FIX16_fromfloat(a);
    r16 = FIX16_sin(a16);
    r_flt = FIX16_tofloat(r16);
    printf("sin: %.3f\n", r_flt);

    // Division
    a16 = FIX16_fromfloat(10.0f);
    b16 = FIX16_fromfloat(3.0f);
    r16 = FIX16_div(a16, b16);
    r_flt = FIX16_tofloat(r16);
    printf("div: %.3f, ", r_flt);

    // Multiplication
    a16 = FIX16_fromfloat(2.0f);
    b16 = FIX16_fromfloat(3.972f);
    r16 = FIX16_mul(a16, b16);
    r_flt = FIX16_tofloat(r16);
    printf("mul: %.3f\n", r_flt);

    while(1);
}
