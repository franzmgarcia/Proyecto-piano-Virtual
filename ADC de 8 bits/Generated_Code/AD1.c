/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : AD1.c
**     Project     : ADC de 8 bits
**     Processor   : MC9S08QE128CLK
**     Component   : ADC
**     Version     : Component 01.690, Driver 01.30, CPU db: 3.00.067
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2018-04-05, 16:07, # CodeGen: 37
**     Abstract    :
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
**     Settings    :
**          Component name                                 : AD1
**          A/D converter                                  : ADC
**          Sharing                                        : Disabled
**          Interrupt service/event                        : Enabled
**            A/D interrupt                                : Vadc
**            A/D interrupt priority                       : medium priority
**          A/D channels                                   : 10
**            Channel0                                     : 
**              A/D channel (pin)                          : PTA0_KBI1P0_TPM1CH0_ADP0_ACMP1PLUS
**              A/D channel (pin) signal                   : 
**            Channel1                                     : 
**              A/D channel (pin)                          : PTA1_KBI1P1_TPM2CH0_ADP1_ACMP1MINUS
**              A/D channel (pin) signal                   : 
**            Channel2                                     : 
**              A/D channel (pin)                          : PTA2_KBI1P2_SDA1_ADP2
**              A/D channel (pin) signal                   : 
**            Channel3                                     : 
**              A/D channel (pin)                          : PTA6_TPM1CH2_ADP8
**              A/D channel (pin) signal                   : 
**            Channel4                                     : 
**              A/D channel (pin)                          : PTA7_TPM2CH2_ADP9
**              A/D channel (pin) signal                   : 
**            Channel5                                     : 
**              A/D channel (pin)                          : PTF0_ADP10
**              A/D channel (pin) signal                   : 
**            Channel6                                     : 
**              A/D channel (pin)                          : PTF1_ADP11
**              A/D channel (pin) signal                   : 
**            Channel7                                     : 
**              A/D channel (pin)                          : PTB2_KBI1P6_SPSCK1_ADP6
**              A/D channel (pin) signal                   : 
**            Channel8                                     : 
**              A/D channel (pin)                          : PTB3_KBI1P7_MOSI1_ADP7
**              A/D channel (pin) signal                   : 
**            Channel9                                     : 
**              A/D channel (pin)                          : PTA3_KBI1P3_SCL1_ADP3
**              A/D channel (pin) signal                   : 
**          A/D resolution                                 : 12 bits
**          Conversion time                                : 5.483627 �s
**          Low-power mode                                 : Disabled
**          Sample time                                    : short
**          Internal trigger                               : Disabled
**          Number of conversions                          : 10
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Events enabled in init.                      : yes
**          CPU clock/speed selection                      : 
**            High speed mode                              : This component enabled
**            Low speed mode                               : This component disabled
**            Slow speed mode                              : This component disabled
**          High input limit                               : 1
**          Low input limit                                : 0
**          Get value directly                             : yes
**          Wait for result                                : yes
**     Contents    :
**         Start          - byte AD1_Start(void);
**         Measure        - byte AD1_Measure(bool WaitForResult);
**         MeasureChan    - byte AD1_MeasureChan(bool WaitForResult, byte Channel);
**         GetValue16     - byte AD1_GetValue16(word *Values);
**         GetChanValue16 - byte AD1_GetChanValue16(byte Channel, word *Value);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file AD1.c
** @version 01.30
** @brief
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
*/         
/*!
**  @addtogroup AD1_module AD1 module documentation
**  @{
*/         


/* MODULE AD1. */

#include "Events.h"
#include "AD1.h"



static void ClrSumV(void);
/*
** ===================================================================
**     Method      :  ClrSumV (component ADC)
**
**     Description :
**         The method clears the internal buffers used to store sum of a 
**         number of last conversions.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define STOP            0x00U          /* STOP state           */
#define MEASURE         0x01U          /* MESURE state         */
#define CONTINUOUS      0x02U          /* CONTINUOS state      */
#define SINGLE          0x03U          /* SINGLE state         */

static const  word Table[10] = {0x01U,0x02U,0x04U,0x08U,0x10U,0x20U,0x40U,0x80U,0x0100U,0x0200U};  /* Table of mask constants */

static const  byte Channels[10] = {0x40U,0x41U,0x42U,0x48U,0x49U,0x4AU,0x4BU,0x46U,0x47U,0x43U
};  /* Contents for the device control register */

static volatile word OutFlg;           /* Measurement finish flag */
static volatile byte SumChan;          /* Number of measured channels */
static volatile byte ModeFlg;          /* Current state of device */
static volatile byte SumCnt;           /* Number of measured channels */

volatile word AD1_SumV[10];            /* Temporary sum of measured values */
volatile word AD1_OutV[10];            /* Sum of measured values */





/*
** ===================================================================
**     Method      :  AD1_Interrupt (component ADC)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(AD1_Interrupt)
{
  TWREG tmpTwreg;

  if (ModeFlg != SINGLE) {
    /*lint -save  -e740 -e931 Disable MISRA rule (1.2) checking. */
    tmpTwreg.b.high = ADCRH;
    tmpTwreg.b.low = ADCRL;
    AD1_SumV[SumChan] += tmpTwreg.w;   /* Save measured value */
    /*lint -restore Enable MISRA rule (1.2) checking. */
    SumChan++;                         /* Number of measurement */
    if (SumChan == 10U) {              /* Is number of measurement equal to the number of conversions? */
      SumChan = 0U;                    /* If yes then set the number of measurement to 0 */
      SumCnt++;
      if (SumCnt == 10U) {
        OutFlg = 0x03FFU;              /* Measured values are available */
        AD1_OutV[0] = AD1_SumV[0];     /* Save measured value to the output buffer */
        AD1_OutV[1] = AD1_SumV[1];     /* Save measured value to the output buffer */
        AD1_OutV[2] = AD1_SumV[2];     /* Save measured value to the output buffer */
        AD1_OutV[3] = AD1_SumV[3];     /* Save measured value to the output buffer */
        AD1_OutV[4] = AD1_SumV[4];     /* Save measured value to the output buffer */
        AD1_OutV[5] = AD1_SumV[5];     /* Save measured value to the output buffer */
        AD1_OutV[6] = AD1_SumV[6];     /* Save measured value to the output buffer */
        AD1_OutV[7] = AD1_SumV[7];     /* Save measured value to the output buffer */
        AD1_OutV[8] = AD1_SumV[8];     /* Save measured value to the output buffer */
        AD1_OutV[9] = AD1_SumV[9];     /* Save measured value to the output buffer */
        if (ModeFlg != MEASURE) {      /* Is the device in measure state? */
          AD1_SumV[0] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[1] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[2] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[3] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[4] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[5] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[6] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[7] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[8] = 0U;            /* Set mesured values to 0 */
          AD1_SumV[9] = 0U;            /* Set mesured values to 0 */
          SumCnt = 0U;                 /* Set number of conversions to 0 */
        }
        AD1_OnEnd();                   /* Invoke user event */
        if (ModeFlg == MEASURE) {      /* Is the device in the measure state? */
          ModeFlg = STOP;              /* Set the device to the stop mode */
          return;                      /* Return from interrupt */
        }
      }
    }
    ADCSC1 = Channels[SumChan];        /* Start measurement of next channel */
  }
  else {
    /*lint -save  -e740 -e931 Disable MISRA rule (1.2) checking. */
    tmpTwreg.b.high = ADCRH;
    tmpTwreg.b.low = ADCRL;
    AD1_SumV[SumChan] += tmpTwreg.w;   /* Save measured value */
    /*lint -restore Enable MISRA rule (1.2) checking. */
    SumCnt++;
    if (SumCnt == 10U) {
      /*lint -save  -e740 -e931 Disable MISRA rule (1.2) checking. */
      OutFlg |= Table[SumChan];        /* Value of measured channel is available */
      /*lint -restore Enable MISRA rule (1.2) checking. */
      AD1_OutV[SumChan] = AD1_SumV[SumChan]; /* Save measured value to the output buffer */
      AD1_OnEnd();                     /* Invoke user event */
      ModeFlg = STOP;                  /* Set the device to the stop mode */
      return;                          /* Return from interrupt */
    }
    ADCSC1 = Channels[SumChan];        /* Start measurement of next channel */
  }
}

/*
** ===================================================================
**     Method      :  ClrSumV (component ADC)
**
**     Description :
**         The method clears the internal buffers used to store sum of a 
**         number of last conversions.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void ClrSumV(void)
{
  AD1_SumV[0] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[1] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[2] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[3] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[4] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[5] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[6] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[7] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[8] = 0U;                    /* Set variable for storing measured values to 0 */
  AD1_SumV[9] = 0U;                    /* Set variable for storing measured values to 0 */
}

/*
** ===================================================================
**     Method      :  AD1_HWEnDi (component ADC)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AD1_HWEnDi(void)
{
  if (ModeFlg) {                       /* Start or stop measurement? */
    SumCnt = 0U;                       /* Set the number of conversions */
    if (ModeFlg != SINGLE) {
      OutFlg = 0U;                     /* Output values aren't available */
      SumChan = 0U;                    /* Set the number of measured channels to 0 */
      ClrSumV();                       /* Clear measured values */
    }
    else {
      /*lint -save  -e740 -e931 Disable MISRA rule (1.2) checking. */
      OutFlg &= (word)(~(word)Table[SumChan]); /* Output value isn't available */
      /*lint -restore Enable MISRA rule (1.2) checking. */
      AD1_SumV[SumChan] = 0U;          /* Set variable for storing measured values to 0 */
    }
    ADCSC1 = Channels[SumChan];        /* If yes then start the conversion */
  }
}

/*
** ===================================================================
**     Method      :  AD1_Start (component ADC)
*/
/*!
**     @brief
**         This method starts continuous conversion on all channels
**         that are set in the component inspector. When each
**         measurement on all channels has finished the [OnEnd ] event
**         may be invoked. This method is not available if the
**         [interrupt service] is disabled and the device doesn't
**         support the continuous mode. Note: If time of measurement is
**         too short and the instruction clock is too slow then the
**         conversion complete interrupt and its handler may cause a
**         system overflow.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
*/
/* ===================================================================*/
byte AD1_Start(void)
{
  if (ModeFlg != STOP) {               /* Is the device in running mode? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = CONTINUOUS;                /* Set state of device to the continuos mode */
  AD1_HWEnDi();                        /* Enable the device */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_Measure (component ADC)
*/
/*!
**     @brief
**         This method performs one measurement on all channels that
**         are set in the component inspector. (Note: If the [number of
**         conversions] is more than one the conversion of A/D channels
**         is performed specified number of times.)
**     @param
**         WaitForResult   - Wait for a result of a
**                           conversion. If [interrupt service] is
**                           disabled, A/D peripheral doesn't support
**                           measuring all channels at once or Autoscan
**                           mode property isn't enabled and at the same
**                           time the [number of channels] is greater
**                           than 1, then the WaitForResult parameter is
**                           ignored and the method waits for each
**                           result every time. If the [interrupt
**                           service] is disabled and a [number of
**                           conversions] is greater than 1, the
**                           parameter is ignored and the method also
**                           waits for each result every time.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
*/
/* ===================================================================*/
#pragma MESSAGE DISABLE C5703 /* WARNING C5703: Parameter declared but not referenced */
byte AD1_Measure(bool WaitForResult)
{
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = MEASURE;                   /* Set state of device to the measure mode */
  AD1_HWEnDi();                        /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ModeFlg == MEASURE) {}      /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_MeasureChan (component ADC)
*/
/*!
**     @brief
**         This method performs measurement on one channel. (Note: If
**         the [number of conversions] is more than one the conversion
**         of the A/D channel is performed specified number of times.)
**     @param
**         WaitForResult   - Wait for a result of
**                           conversion. If the [interrupt service] is
**                           disabled and at the same time a [number of
**                           conversions] is greater than 1, the
**                           WaitForResult parameter is ignored and the
**                           method waits for each result every time.
**     @param
**         Channel         - Channel number. If only one
**                           channel in the component is set this
**                           parameter is ignored, because the parameter
**                           is set inside this method.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
**                           ERR_RANGE - Parameter "Channel" out of range
*/
/* ===================================================================*/
byte AD1_MeasureChan(bool WaitForResult,byte Channel)
{
  if (Channel >= 10U) {                /* Is channel number greater than or equal to 10 */
    return ERR_RANGE;                  /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = SINGLE;                    /* Set state of device to the measure mode */
  SumChan = Channel;                   /* Set required channel */
  AD1_HWEnDi();                        /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ModeFlg == SINGLE) {}       /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_GetValue16 (component ADC)
*/
/*!
**     @brief
**         This method returns the last measured values of all channels.
**         Compared with [GetValue] method this method returns more
**         accurate result if the [number of conversions] is greater
**         than 1 and [AD resolution] is less than 16 bits. In addition,
**         the user code dependency on [AD resolution] is eliminated.
**     @param
**         Values          - Pointer to the array that contains
**                           the measured data.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
byte AD1_GetValue16(word *Values)
{
  if (OutFlg != 0x03FFU) {             /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  Values[0] = (word)((((dword)(AD1_OutV[0])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[1] = (word)((((dword)(AD1_OutV[1])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[2] = (word)((((dword)(AD1_OutV[2])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[3] = (word)((((dword)(AD1_OutV[3])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[4] = (word)((((dword)(AD1_OutV[4])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[5] = (word)((((dword)(AD1_OutV[5])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[6] = (word)((((dword)(AD1_OutV[6])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[7] = (word)((((dword)(AD1_OutV[7])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[8] = (word)((((dword)(AD1_OutV[8])) << 4) / 10U); /* Save measured values to the output buffer */
  Values[9] = (word)((((dword)(AD1_OutV[9])) << 4) / 10U); /* Save measured values to the output buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_GetChanValue16 (component ADC)
*/
/*!
**     @brief
**         This method returns the last measured value of the required
**         channel. Compared with [GetChanValue] method this method
**         returns more accurate result if the [number of conversions]
**         is greater than 1 and [AD resolution] is less than 16 bits.
**         In addition, the user code dependency on [AD resolution] is
**         eliminated.
**     @param
**         Channel         - Channel number. If only one
**                           channel in the component is set then this
**                           parameter is ignored.
**     @param
**         Value           - Pointer to the measured value.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_RANGE - Parameter "Channel" out of
**                           range
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
*/
/* ===================================================================*/
byte AD1_GetChanValue16(byte Channel,word *Value)
{
  if (Channel >= 10U) {                /* Is channel number greater than or equal to 10 */
    return ERR_RANGE;                  /* If yes then error */
  }
  if ((OutFlg & Table[Channel]) == 0U) { /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  *Value = (word)((((dword)(AD1_OutV[Channel])) << 4) / 10U); /* Save measured values to the output buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_Init (component ADC)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AD1_Init(void)
{
  /* ADCSC1: COCO=0,AIEN=0,ADCO=0,ADCH4=1,ADCH3=1,ADCH2=1,ADCH1=1,ADCH0=1 */
  setReg8(ADCSC1, 0x1FU);              /* Disable the module */ 
  /* ADCSC2: ADACT=0,ADTRG=0,ACFE=0,ACFGT=0,??=0,??=0,??=0,??=0 */
  setReg8(ADCSC2, 0x00U);              /* Disable HW trigger and autocompare */ 
  OutFlg = 0U;                         /* No measured value */
  ModeFlg = STOP;                      /* Device isn't running */
  /* ADCCFG: ADLPC=0,ADIV1=1,ADIV0=0,ADLSMP=0,MODE1=0,MODE0=1,ADICLK1=0,ADICLK0=0 */
  setReg8(ADCCFG, 0x44U);              /* Set prescaler bits */ 
}


/* END AD1. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
