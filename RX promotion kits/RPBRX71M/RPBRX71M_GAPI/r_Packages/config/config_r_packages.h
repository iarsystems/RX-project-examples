/**
 * This file configures the Renesas Package Libraries.
 *  Definitions Global to all libraries are defined here.
 */
#ifndef CONFIG_R_PACKAGES_H
#define CONFIG_R_PACKAGES_H

/* These MACRO's are parsed by the library make utility...only change as necessary*/
/* MCU family for compiler selection */
#define COMPILER_FAMILY RX
/* Compiler version, change as necessary */
#define COMPILER_VERSION 2_3_0
/* Directory to build libraries in...typically no reason to change */
#define LIB_DIRECTORY release
/* Common tool options...only change as necessary */
#define FLAGS_COMMON -cpu=rx600 -debug -nologo -endian=LITTLE
/* C Compiler options */
#define CFLAGS -lang=c99 -utf8
/* C++ compiler flags */
#define CXXFLAGS -lang=cpp
/* Assembler options */
#define ASFLAGS 
/* Linker options */
#define LNK_FLAGS -form=library -list -show=all -nologo

#endif
