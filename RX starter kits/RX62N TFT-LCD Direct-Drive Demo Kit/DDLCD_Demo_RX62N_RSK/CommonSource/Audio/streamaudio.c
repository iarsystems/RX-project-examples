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
#include "Global.h"
#include "LCD_demo.h"
//#include "da_dtc.h"
#include "Audio.h"

/***********************************************************************/
/*                                                                     */
/*  FILE        :StreamAudio.c                                         */
/*  DATE        :August 14, 2008                                       */
/*  DESCRIPTION :Streams requested audio out the D/A Channel           */
/*  CPU TYPE    :H8S/H8SX                                              */
/*                                                                     */
/*                                                                     */
/*                                                                     */
/***********************************************************************/

#define CHANNEL_SAMPLES 256                   /* Size of audio buffers (uncompressed data bytes) */         
typedef struct 
{
  uint8_t PCMBuffer[2][CHANNEL_SAMPLES][2];      /* stores decompressed data (double buffered) + left right channels*/
  uint8_t *location;                             /* starting address for audio in memory */
  uint32_t size;                                  /* size of PCM data file */
  uint32_t position;                              /* current position in file */
  uint32_t start_time;                            /* time of playback start */
  enum
  {
    IDLE = 0x00, STARTING, RUNNING
  } state;                                    /* identifies the current state */
  uint16_t frequency;                             /* audio sample frequency */
  uint8_t Left;                                  /* position of left channel MSByte in record */
  uint8_t Right;                                 /* position of right channel MSByte in record */
  uint8_t BlockAlign;                            /* record pitch */
}tAudioChannel;

/* stores all the audio channels */
tAudioChannel Audio;       /* store channel information */

static xTaskHandle AudioHnd;  /* RTOS_USAGE */
static xQueueHandle     AudioQueue; /* RTOS_USAGE */

void AudioLoadBuffer(uint8_t select)
{
  if (Audio.state != IDLE)
  {
    if (AudioOutput.Buffer[select].size == 0)
    {
      int16_t index, buffer_count;
      /* determine how much data to transfer to the buffer */
      if (Audio.size - Audio.position > CHANNEL_SAMPLES)
        buffer_count = CHANNEL_SAMPLES;
      else
        buffer_count = (int16_t)(Audio.size - Audio.position);

      /* read PCM data */
      {
        uint8_t *pDest = Audio.PCMBuffer[select][0];
        uint8_t *pSrc = &Audio.location[Audio.position];

        for (index = 0; index < buffer_count; index++, pDest+=2, pSrc+=Audio.BlockAlign)
        {
          pDest[0] = pSrc[Audio.Left];
          pDest[1] = pSrc[Audio.Right];
        }
        Audio.position += (buffer_count * Audio.BlockAlign);
      }

      AudioOutput.Buffer[select].ptr = Audio.PCMBuffer[select][0];
      AudioOutput.Buffer[select].size = buffer_count;

      if (buffer_count == 0)
        Audio.state = IDLE;
    }
  }
}

static void AudioTask(void *pvParameters)
{
  initAudio();
  for ( ; ; )
  {
    int16_t retVal;
    WAV_type *pWAV;
    /* check if new audio request */
    retVal = xQueueReceive(AudioQueue, &pWAV, 0); /* RTOS_USAGE */

    /* Let system know we're accessing External Memory */
    ExMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */

    if (retVal == pdPASS)
    {
      /* map audio request to hardware */
      AudioStop();
      Audio.location = AUDWAVpData(pWAV);
      Audio.size = AUDWAVSize(pWAV);
      Audio.position = 0;
      Audio.BlockAlign = AUDWAVBlockAlign(pWAV);

      if (AUDWAVBitsPerSample(pWAV) == 8)
      {
        /* 8 bit samples */
        if(AUDWAVNumChannels(pWAV) == 1)
        {
          /* mono */
          Audio.Left = 0; 
          Audio.Right = 0;
        }
        else
        {
          /* stereo */
          Audio.Left = 0; 
          Audio.Right = 1;
        }
      }
      else
      {
        /* 16 bit samples */
        if(AUDWAVNumChannels(pWAV) == 1)
        {
          /* mono */
          Audio.Left = 1; 
          Audio.Right = 1;
        }
        else
        {
          /* stereo */
          Audio.Left = 1; 
          Audio.Right = 3;
        }
      }

      Audio.frequency = AUDWAVSampleRate(pWAV);
      Audio.start_time = xTaskGetTickCount(); /* RTOS_USAGE */
      Audio.state = STARTING;
      AudioOutput.active = 0;
    }

    /* service buffer needs for each channel */
    AudioLoadBuffer(AudioOutput.active+1 & 0x01);
    if (Audio.state == STARTING)
    {
      /* start timer and resync buffers */
      AudioStart(Audio.frequency, AudioHnd);
      Audio.state = RUNNING;
    }

    ExMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
    // Suspend until service requested (queue or buffer fill).
    vTaskSuspend( NULL ); /* RTOS_USAGE */
  }
}

/**********************************************************************************
Function Name:   playAudio
Description:  locate and enqueue requested sound file
Arguments:   name: name of WAV file to play
Return value:   0=ok, not 0 = NG
***********************************************************************************/
int playAudio(char *name)
{
  static uint8_t once =0;

  if (once == 0)
  {
    /* allocate the task and queue */
    /* A  queue will be allocated for the audio...4 WAV files */
    AudioQueue = xQueueCreate(4, (unsigned portBASE_TYPE)sizeof(WAV_type *)); /* RTOS_USAGE */

    /* allocate task to manage audio */
    (void)xTaskCreate(AudioTask, "b:Audio", demotaskSTACK_SIZE, NULL, mainAUDIOTASKS_PRIORITY, &AudioHnd);  /* RTOS_USAGE */

    once = 0xFF;
  }

  {
    WAV_type const *pWAV = FileFind(pResources, name);
    if (pWAV == 0)
      return(-1);
    if (0 != AUDWAVisPCM(pWAV))
      return(-1);

    if(AUDWAVNumChannels(pWAV) > 2)
      return(-1);

    if (AUDWAVBitsPerSample(pWAV) > 16)
      return(-1);

    (void)xQueueSend(AudioQueue, (void *)&pWAV, 0UL);  /* RTOS_USAGE */
    (void)vTaskResume( AudioHnd );  /* RTOS_USAGE */
  }
  return(0);
}
