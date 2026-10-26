/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************************
FILE NAME    : Audio_WAV.c

DESCRIPTION  : WAV Information Support for Renesas Audio API.

Copyright   : 2009 Renesas Electronics America
Copyright   : 2009 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
09.09.2009 RTA-JMB First Release
***********************************************************************************/
#include "Global.H"
#include "Audio.H"

static const WAV_type WAV_header=
{
  {'R', 'I', 'F', 'F'},                          /* ChunckID[4];        RIFF                                                                 */ 
  {0x00, 0x00, 0x00, 0x00},                      /* ChunckSize[4];                                                                           */
  {'W', 'A', 'V', 'E'},                          /* Format[4];          WAVE                                                                 */ 
  {'f', 'm', 't', ' '},                          /* SubChunk1ID[4];     fmt                                                                  */ 
  {0x10, 0x00, 0x00, 0x00},                      /* SubChunk1Size[4];   0x10 for PCM                                                         */ 
  {0x01, 0x00},                                  /* AudioFormat[2];     PCM = 0x01                                                           */ 
  {0x01, 0x00},                                  /* NumChannels[2];     mono=1, stereo = 2                                                   */ 
  {0x11, 0x2B, 0x00, 0x00},                      /* SampleRate[4];      8000, 44100, etc                                                     */ 
  {0x11, 0x2B, 0x00, 0x00},                      /* ByteRate[4];        == SampleRate * NumChannels * BitsPerSample/8                        */ 
  {0x01, 0x00},                                  /* BlockAlign[2];      == NumChannels * BitsPerSample/8                                     */ 
  {0x08, 0x00},                                  /* BitsPerSample[2];   8 bits = 8, 16 bits = 16, etc                                        */ 
};

/**********************************************************************************
Function Name:   AUDWAVisPCM
Description:  Check if this data structure is a PCM WAV file
Arguments:   s: pointer to source WAV
Return value:   0:OK, Not 0: NG
***********************************************************************************/
int16_t AUDWAVisPCM(WAV_type const *const s)
{
  int16_t index;
  /* check ChunckID, Format, SubChunk1ID & AudioFormat fields */
  for ( index = 0; index < 3; index++ )
    if ( s->ChunckID[index] != WAV_header.ChunckID[index] )
      return(-1);
  for ( index = 0; index < 3; index++ )
    if ( s->Format[index] != WAV_header.Format[index] )
      return(-1);
  for ( index = 0; index < 3; index++ )
    if ( s->SubChunk1ID[index] != WAV_header.SubChunk1ID[index] )
      return(-1);
  for ( index = 0; index < 3; index++ )
    if ( s->AudioFormat[index] != WAV_header.AudioFormat[index] )
      return(-1);
  return(0);
}

/**********************************************************************************
Function Name:   AUDWAVNumChannels
Description:  obtain number of channels in PCM WAV file
Arguments:   s: pointer to source WAV
Return value:   number of channels
***********************************************************************************/
uint16_t AUDWAVNumChannels(WAV_type const *const s)
{
  return(((uint16_t)s->NumChannels[1] << 8) + ((uint16_t)s->NumChannels[0] << 0));
}

/**********************************************************************************
Function Name:   AUDWAVSampleRate
Description:  obtain sample rate of PCM WAV file
Arguments:   s: pointer to source WAV
Return value:   sample frequency
***********************************************************************************/
uint32_t AUDWAVSampleRate(WAV_type const *const s)
{
  return(((uint32_t)s->SampleRate[3] << 24) + ((uint32_t)s->SampleRate[2] << 16) + ((uint32_t)s->SampleRate[1] << 8) + ((uint32_t)s->SampleRate[0] << 0));
}

/**********************************************************************************
Function Name:   AUDWAVBitsPerSample
Description:  obtain sample width PCM WAV file
Arguments:   s: pointer to source WAV
Return value:   bit per sample
***********************************************************************************/
uint16_t AUDWAVBitsPerSample(WAV_type const *const s)
{
  return(((uint16_t)s->BitsPerSample[1] << 8) + ((uint16_t)s->BitsPerSample[0] << 0));
}

/**********************************************************************************
Function Name:   BlockAlign
Description:  obtain pitch of samples
Arguments:   s: pointer to source WAV
Return value:   bytes per record
***********************************************************************************/
uint16_t AUDWAVBlockAlign(WAV_type const *const s)
{
  return(((uint16_t)s->BlockAlign[1] << 8) + ((uint16_t)s->BlockAlign[0] << 0));
}
/**********************************************************************************
Function Name:   AUDWAVpChunk2
Description:  obtain pointer to chunk 2 data record
Arguments:   s: pointer to source WAV
Return value:   pointer to data record
***********************************************************************************/
WAV_DATA_type *AUDWAVpChunk2(WAV_type const *const s)
{
  /* first find SubChunk1Size so we can locate SubChunk2 */
  uint32_t size = ((uint32_t)s->SubChunk1Size[3] << 24) + ((uint32_t)s->SubChunk1Size[2] << 16) + ((uint32_t)s->SubChunk1Size[1] << 8) + ((uint32_t)s->SubChunk1Size[0] << 0);
  return((WAV_DATA_type *)&s->AudioFormat[size]);
}

/**********************************************************************************
Function Name:   AUDWAVpData
Description:  obtain pointer to PCM Data
Arguments:   s: pointer to source WAV
Return value:   0:OK, Not 0: NG
***********************************************************************************/
uint8_t *AUDWAVpData(WAV_type const *const s)
{
  /* first find SubChunk2 so we can locate data */
  return(AUDWAVpChunk2(s)->Data);
}

/**********************************************************************************
Function Name:   AUDWAVSize
Description:  obtain number of samples in PCM WAV file
Arguments:   s: pointer to source WAV
Return value:   0:OK, Not 0: NG
***********************************************************************************/
uint32_t AUDWAVSize(WAV_type const *const s)
{
  /* first find SubChunk2 so we can locate size */
  WAV_DATA_type *SubChunk2 =AUDWAVpChunk2(s);
  return(((uint32_t)SubChunk2->SubChunk2Size[3] << 24) + ((uint32_t)SubChunk2->SubChunk2Size[2] << 16) + ((uint32_t)SubChunk2->SubChunk2Size[1] << 8) + ((uint32_t)SubChunk2->SubChunk2Size[0] << 0));
}




