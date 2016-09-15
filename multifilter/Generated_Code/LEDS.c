/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : LEDS.c
**     Project     : multifilter
**     Processor   : MK22FN512VLH12
**     Component   : 74HC595
**     Version     : Component 01.022, Driver 01.00, CPU db: 3.00.000
**     Repository  : Legacy User Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-02-19, 02:32, # CodeGen: 0
**     Abstract    :
**
Driver for the 74HC595 8bit serial shift register.
**     Settings    :
**          Component name                                 : LEDS
**          Latch (STCP)                                   : STCP
**          Data (DS)                                      : DS
**          Clock (SHCP)                                   : SHCP
**          Clock/Latch delay (ns)                         : 19
**          Wait                                           : WAIT1
**          MSB first                                      : yes
**          Enable Pin (OE)                                : Disabled
**     Contents    :
**         Disable   - void LEDS_Disable(void);
**         ShiftByte - void LEDS_ShiftByte(byte val);
**         Latch     - void LEDS_Latch(void);
**         ReadByte  - byte LEDS_ReadByte(void);
**         WriteByte - void LEDS_WriteByte(byte val);
**         Deinit    - void LEDS_Deinit(void);
**         Init      - void LEDS_Init(void);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2013, all rights reserved.
**     Web: http://mcuoneclipse.com
**     This an open source software implementing a driver using Processor Expert.
**     This is a free software and is opened for education, research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file LEDS.c
** @version 01.00
** @brief
**
Driver for the 74HC595 8bit serial shift register.
*/         
/*!
**  @addtogroup LEDS_module LEDS module documentation
**  @{
*/         

/* MODULE LEDS. */

#include "LEDS.h"

#define LEDS_STCP_LATCH_DELAY_NS  19 /* delay in nano seconds */
#define LEDS_SHCP_CLOCK_DELAY_NS  19 /* delay in nano seconds */

static uint8_t LEDS_regVal8; /* cached value of the shift register */

#define LEDS_SHIFT_MSB_FIRST  1  /* MSB is shifted first */
/*
** ===================================================================
**     Method      :  LEDS_Latch (component 74HC595)
**     Description :
**         Sends a latch pulse to move the bits from the internal
**         memory to the output pins.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LEDS_Latch(void)
{
  /* send a latch pulse to show the data on the output pins */
  STCP1_SetVal(); /* set latch to high */
#if LEDS_STCP_LATCH_DELAY_NS != 0
  WAIT1_Waitns(LEDS_STCP_LATCH_DELAY_NS); /* wait according to data sheet */
#endif
  STCP1_ClrVal(); /* set latch to low */
#if LEDS_STCP_LATCH_DELAY_NS != 0
  WAIT1_Waitns(LEDS_STCP_LATCH_DELAY_NS); /* wait according to data sheet */
#endif
}

/*
** ===================================================================
**     Method      :  LEDS_ShiftByte (component 74HC595)
**     Description :
**         Shift a byte to the shift register. The most significant bit
**         will be on output pin 0. Use Latch() to show the data on the
**         output pins.
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - 8bit value to be shifted
**     Returns     : Nothing
** ===================================================================
*/
void LEDS_ShiftByte(byte val)
{
  uint8_t i;

  /* precondition: latch pin, data pin and clock pin are all low */
  LEDS_regVal8 = val;
  for(i=0;i<8;i++) { /* shift all the 8 bits */
    /* put data bit */
#if LEDS_SHIFT_MSB_FIRST
    if (val&0x80) { /* LSB bit first */
#else /* LSB first */
    if (val&1) { /* LSB bit first */
#endif
      DS1_SetVal();
    } else {
      DS1_ClrVal();
    }
#if LEDS_SHCP_CLOCK_DELAY_NS != 0
    WAIT1_Waitns(LEDS_SHCP_CLOCK_DELAY_NS); /* wait according to data sheet */
#endif
    SHCP1_SetVal(); /* CLK high: data gets transfered into memory */
#if LEDS_SHCP_CLOCK_DELAY_NS != 0
    WAIT1_Waitns(LEDS_SHCP_CLOCK_DELAY_NS); /* wait according to data sheet */
#endif
    DS1_ClrVal(); /* data line low */
#if LEDS_SHCP_CLOCK_DELAY_NS != 0
    WAIT1_Waitns(LEDS_SHCP_CLOCK_DELAY_NS); /* wait according to data sheet */
#endif
    SHCP1_ClrVal(); /* CLK high: data gets transfered into memory */
#if LEDS_SHIFT_MSB_FIRST
    val <<= 1; /* next bit */
#else
    val >>= 1; /* next bit */
#endif
  }
}

/*
** ===================================================================
**     Method      :  LEDS_Disable (component 74HC595)
**     Description :
**         Disables the device
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LEDS_Disable(void)
{
  /* no OE pin available */
}

/*
** ===================================================================
**     Method      :  LEDS_Deinit (component 74HC595)
**     Description :
**         Deinitializes the driver
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LEDS_Deinit(void)
{
  /* nothing needed here */
}

/*
** ===================================================================
**     Method      :  LEDS_Init (component 74HC595)
**     Description :
**         Initializes the driver
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LEDS_Init(void)
{
  LEDS_regVal8 = 0;
  STCP1_ClrVal(); /* latch */
  DS1_ClrVal(); /* data */
  SHCP1_ClrVal(); /* clock */
}

/*
** ===================================================================
**     Method      :  LEDS_WriteByte (component 74HC595)
**     Description :
**         Shifts the given byte and latches it.
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - value to be shifted into the shift
**                           register. The value will be latched and
**                           will appear on the output pins.
**     Returns     : Nothing
** ===================================================================
*/
void LEDS_WriteByte(byte val)
{
  LEDS_ShiftByte(val); /* shift byte to the shift register */
  LEDS_Latch(); /* latch it and show it on the output pins */
}

/*
** ===================================================================
**     Method      :  LEDS_ReadByte (component 74HC595)
**     Description :
**         Returns the current value of the shift register. The current
**         value of the shift register is cached in the driver.
**     Parameters  : None
**     Returns     :
**         ---             - current shift register value (cached)
** ===================================================================
*/
byte LEDS_ReadByte(void)
{
  return LEDS_regVal8;
}

/* END LEDS. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/