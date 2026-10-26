/* Adapted for use with IAR Embedded Workbench */
/****** Header File for support of STFL-I based Serial Flash Memory Driver *****

   Filename:    Serialize.h
   Description: Header file for Serialize.c
                     Consult also the C file for more details.

   Version:    0.1
   Date:        Sept. 2009
   Authors:    Numonyx S.r.l. Arzano (Napoli)

   THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
   CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A
   RESULT, NUMONYX S.r.l. SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
   CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH
   SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN
   IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************

   Version History.
   Ver.   Date      Comments

   0.1   June 2009  Alpha version

*******************************************************************************/

#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include "P5Q.h"

#define ptrNull 0   // a null pointer

#define True  1
#define False 0
typedef unsigned char Bool;
/* Status register masks */

#define SPI_SR1_WIP        (1 << 0)
#define SPI_SR1_WEL        (1 << 1)
#define SPI_SR1_BP0        (1 << 2)
#define SPI_SR1_BP1        (1 << 3)
#define SPI_SR1_BP2        (1 << 4)
#define SPI_SR1_E_FAIL      (1 << 5)
#define SPI_SR1_P_FAIL      (1 << 6)
#define SPI_SR1_SRWD      (1 << 7)


// mask bit definitions for SPI master side configuration
enum
{
    MaskBit_Trans                          = 0x01,  // mask bit for Transfer enable/disable
    MaskBit_Recv                           = 0x02,  // mask bit for Receive enable/disable
    MaskBit_Trans_Relevant                 = 0x04,  // check whether MaskBit_Trans is necessary
    MaskBit_Recv_Relevant                  = 0x08,  // check whether MaskBit_Recv is necessary

    MaskBit_SlaveSelect                    = 0x10,  // mask bit for Slave Select/Deselect
    MaskBit_SelectSlave_Relevant           = 0x20,  // check whether MaskBit_SelectSlave is necessary

};

// Acceptable values for SPI master side configuration
typedef enum _SpiMasterConfigOptions
{
    enumNull                               = 0,     // do nothing
    enumEnableTransOnly                    = 0x05,  // enable transfer
    enumEnableRecvOnly                     = 0x0A,  // enable receive
    enumEnableTansRecv                     = 0x0F,  // enable transfer & receive

    enumEnableTransOnly_SelectSlave        = 0x35,  // enable transfer and select slave
    enumEnableRecvOnly_SelectSlave         = 0x3A,  // enable receive and select slave
    enumEnableTansRecv_SelectSlave         = 0x3F,  // enable transfer & receive and select slave

    enumDisableTransOnly                   = 0x04,  // disable transfer and deselect slave
    enumDisableRecvOnly                    = 0x08,  // disable receive
    enumDisableTransRecv                   = 0x0C,  // disable transfer & receive

    enumDisableTransOnly_DeSelectSlave     = 0x24,  // disable transfer and deselect slave
    enumDisableRecvOnly_DeSelectSlave      = 0x28,  // disable receive and deselect slave
    enumDisableTansRecv_DeSelectSlave      = 0x2C   // disable transfer & receive and deselect slave

}SpiMasterConfigOptions;

// char stream definition for 
typedef struct _structCharStream
{
    NMX_uint8* pChar;                                // buffer address that holds the streams
    NMX_uint32 length;                               // length of the stream in bytes
}CharStream;

void InitSPIMaster();                               // one-time setting to work in SPI mode
void ConfigureSpiMaster(
                        SpiMasterConfigOptions opt  // configuration options
                        ); 

NMX_uint8 Serialize_SPI(
               const CharStream* char_stream_send,   // char stream to be sent to the memory(incl. instruction, address etc)
               CharStream* char_stream_recv,         // char stream to be received from the memory   
               SpiMasterConfigOptions optBefore,     // Pre-Configurations on the SPI master side
               SpiMasterConfigOptions optAfter       // Post-Configurations on the SPI master side
              );

#endif //end of file 


