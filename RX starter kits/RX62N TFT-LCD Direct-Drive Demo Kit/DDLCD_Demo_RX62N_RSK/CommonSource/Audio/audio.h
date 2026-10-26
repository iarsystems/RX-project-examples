/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/

#ifndef AUDIO_H
#define AUDIO_H
/***********************************************************************************
FILE NAME    : Audio.h

DESCRIPTION  : Structure definitions and function prototypes for Renesas Audio API.

Copyright   : 2009 Renesas Electronics America
Copyright   : 2009 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
09.18.2009 RTA-JMB Rev 1.0
***********************************************************************************/

/* WAV header structure (little endian) */
typedef struct 
{
  uint8_t ChunckID[4];                           /* RIFF */
  uint8_t ChunckSize[4];
  uint8_t Format[4];                             /* WAVE */
  uint8_t SubChunk1ID[4];                        /* fmt  */
  uint8_t SubChunk1Size[4];                      /* 0x10 for PCM */
  uint8_t AudioFormat[2];                        /* PCM = 0x01 */
  uint8_t NumChannels[2];                        /* mono=1, stereo = 2 */
  uint8_t SampleRate[4];                         /* 8000, 44100, etc */
  uint8_t ByteRate[4];                           /* == SampleRate * NumChannels * BitsPerSample/8 */
  uint8_t BlockAlign[2];                         /* == NumChannels * BitsPerSample/8 */
  uint8_t BitsPerSample[2];                      /* 8 bits = 8, 16 bits = 16, etc */
} WAV_type;

typedef struct
{
  uint8_t SubChunk2ID[4];                        /* data */
  uint8_t SubChunk2Size[4];                      /* == NumSamples * NumChannels * BitsPerSample/8 (size of "data" array) */
  uint8_t Data[4];                               /* first four bytes of data array */
} WAV_DATA_type;

int16_t AUDWAVisPCM(WAV_type const *const s);
uint16_t AUDWAVNumChannels(WAV_type const *const s);
uint32_t AUDWAVSampleRate(WAV_type const *const s);
uint16_t AUDWAVBitsPerSample(WAV_type const *const s);
uint16_t AUDWAVBlockAlign(WAV_type const *const s);
uint8_t *AUDWAVpData(WAV_type const *const s);
uint32_t AUDWAVSize(WAV_type const *const s);

#endif
