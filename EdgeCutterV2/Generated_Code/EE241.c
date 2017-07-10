/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : EE241.c
**     Project     : EdgeCutterV2
**     Processor   : MKL02Z32VFM4
**     Component   : 24AA_EEPROM
**     Version     : Component 01.031, Driver 01.00, CPU db: 3.00.000
**     Repository  : Legacy User Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-07-08, 23:01, # CodeGen: 0
**     Abstract    :
**         Driver for Microchip 24_AA/LC EEPROMs
**     Settings    :
**          Component name                                 : EE241
**          Device                                         : 8
**          Initial Device Address                         : 0x0
**          Block buffer size                              : 32
**          Acknowledge Polling                            : Enabled
**            Page Write Time (ms)                         : 0
**            Wait                                         : WAIT1
**            ACK Polling Time (us)                        : 100
**          Connection                                     : 
**            I2C                                          : GI2C1
**            Write Protection Pin                         : Disabled
**          Timeout                                        : Disabled
**          Shell                                          : Disabled
**     Contents    :
**         ReadByte   - byte EE241_ReadByte(EE241_Address addr, byte *data);
**         WriteByte  - byte EE241_WriteByte(EE241_Address addr, byte data);
**         ReadBlock  - byte EE241_ReadBlock(EE241_Address addr, byte *data, word dataSize);
**         WriteBlock - byte EE241_WriteBlock(EE241_Address addr, byte *data, word dataSize);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2013, all rights reserved.
**     This an open source software implementation with Processor Expert.
**     This is a free software and is opened for education,  research  and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file EE241.c
** @version 01.00
** @brief
**         Driver for Microchip 24_AA/LC EEPROMs
*/         
/*!
**  @addtogroup EE241_module EE241 module documentation
**  @{
*/         

/* MODULE EE241. */

#include "EE241.h"

#define EE241_I2CAddress (0&EE241_MAX_I2C_ADDR_MASK) /* address defined by the A2|A1|A0 pins */

/* macros for the control byte: */
#define EE241_CTRL_NBL       (0x0A<<3)  /* control byte high nibble. Typically this is 1010 (shifted by one to the right) */
#if EE241_DEVICE_ID==EE241_DEVICE_ID_8
  #define EE241_CTRL_ADDR      0        /* no additional address bits */
  /* define control byte as 1010|Bx|B1|B0 */
  #define EE241_BANK_0         (0<<2)   /* B0 bit (0) inside the CTRL_BYTE: 1010|B0|A1|A0 */
  #define EE241_BANK_1         (1<<2)   /* B0 bit (1) inside the CTRL_BYTE: 1010|B0|A1|A0 */
  #define EE241_CTRL_BYTE      (EE241_CTRL_NBL|EE241_CTRL_ADDR) /* 1010|B0|A1|A0 */
  #define EE241_DEVICE_ADDR(addr) \
    ( EE241_CTRL_BYTE|((addr>>8)&0x07) )
  #if 0 /* old style */
    (((addr)&0x400)? \
        (EE241_CTRL_BYTE|EE241_BANK_1) \
      : (EE241_CTRL_BYTE|EE241_BANK_0) ) /* 7bit address of device used to select device */
  #endif        
#elif (EE241_DEVICE_ID==EE241_DEVICE_ID_32) || (EE241_DEVICE_ID==EE241_DEVICE_ID_256) || (EE241_DEVICE_ID==EE241_DEVICE_ID_512)
  #define EE241_CTRL_ADDR      EE241_I2CAddress /* address inside control byte */
  /* define control byte as 1010|A2|A1|A0 */
  #define EE241_CTRL_BYTE         (EE241_CTRL_NBL|EE241_CTRL_ADDR) /* 1010|A2|A1|A0 */
  #define EE241_DEVICE_ADDR(addr) EE241_CTRL_BYTE /* 7bit address of device used to select device */
#elif EE241_DEVICE_ID==EE241_DEVICE_ID_1025
  #define EE241_CTRL_ADDR      EE241_I2CAddress /* address inside control byte */
  /* define control byte as 1010|Bx|A1|A0 */
  #define EE241_BANK_0         (0<<2)   /* B0 bit (0) inside the CTRL_BYTE: 1010|B0|A1|A0 */
  #define EE241_BANK_1         (1<<2)   /* B0 bit (1) inside the CTRL_BYTE: 1010|B0|A1|A0 */
  #define EE241_CTRL_BYTE      (EE241_CTRL_NBL|EE241_CTRL_ADDR) /* 1010|B0|A1|A0 */
  #define EE241_DEVICE_ADDR(addr) \
    (((addr)&0x10000)? \
        (EE241_CTRL_BYTE|EE241_BANK_1) \
      : (EE241_CTRL_BYTE|EE241_BANK_0) ) /* 7bit address of device used to select device */
#endif

/*
** ===================================================================
**     Method      :  EE241_WriteByte (component 24AA_EEPROM)
**     Description :
**         Writes a single byte to specified address
**     Parameters  :
**         NAME            - DESCRIPTION
**         addr            - The address inside the EEPROM
**         data            - The data value to write
**     Returns     :
**         ---             - Error code, possible values
**                           ERR_OK - OK
**                           otherwise it can return an error code of
**                           the underlying communication protocol.
** ===================================================================
*/
byte EE241_WriteByte(EE241_Address addr, byte data)
{
  uint8_t res, block[3];

  res = GI2C1_SelectSlave(EE241_DEVICE_ADDR(addr));
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
  #if EE241_DEVICE_ID==EE241_DEVICE_ID_8  
    block[0] = (uint8_t)(addr&0xff);    /* low byte of address */
    block[1] = data; /* switch to read mode */
    res = GI2C1_WriteBlock(block, 2, GI2C1_SEND_STOP); /* send address and data */
  #else
    block[0] = (uint8_t)(addr>>8);      /* high byte of address */
    block[1] = (uint8_t)(addr&0xff);    /* low byte of address */
    block[2] = data; /* switch to read mode */
    res = GI2C1_WriteBlock(block, sizeof(block), GI2C1_SEND_STOP); /* send address and data */
  #endif
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
#if EE241_DO_ACKNOWLEDGE_POLLING
  /* do acknowledge polling */
  block[0] = 0xff; /* dummy value */
  do {
    WAIT1_WaitOSms(EE241_PAGE_WRITE_TIME_MS);
    res = GI2C1_ProbeACK(block, 1, GI2C1_SEND_STOP, EE241_ACK_POLLING_TIME_US); /* send address and data */
  } while(res!=ERR_OK); /* wait until we get an ACK */
#endif /* EE241_DO_ACKNOWLEDGE_POLLING */
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
  return GI2C1_UnselectSlave();
}

/*
** ===================================================================
**     Method      :  EE241_ReadByte (component 24AA_EEPROM)
**     Description :
**         Reads a single byte from the given memory address
**     Parameters  :
**         NAME            - DESCRIPTION
**         addr            - The address where to read from memory.
**       * data            - Pointer to a location where to store the
**                           data
**     Returns     :
**         ---             - Error code, possible values
**                           ERR_OK - OK
**                           otherwise it can return an error code of
**                           the underlying communication protocol.
** ===================================================================
*/
byte EE241_ReadByte(EE241_Address addr, byte *data)
{
  uint8_t res;
  #if EE241_DEVICE_ID==EE241_DEVICE_ID_8
    uint8_t addr8;
    addr8 = (uint8_t)(addr&0xff); // low address byte
  #else
    uint8_t addr16[2];                  /* big endian address on I2C bus needs to be 16bit */
  
    addr16[0] = (uint8_t)(addr>>8); /* 16 bit address must be in big endian format */
    addr16[1] = (uint8_t)(addr&0xff);
  #endif

  res = GI2C1_SelectSlave(EE241_DEVICE_ADDR(addr));
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
  #if EE241_DEVICE_ID==EE241_DEVICE_ID_8
    res = GI2C1_WriteBlock(&addr8, 1, GI2C1_DO_NOT_SEND_STOP); /* send 8bit address */
  #else /* use 16bit address */
    res = GI2C1_WriteBlock(addr16, 2, GI2C1_DO_NOT_SEND_STOP); /* send 16bit address */
  #endif
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
  res = GI2C1_ReadBlock(data, 1, GI2C1_SEND_STOP); /* read data byte from bus */
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
  return GI2C1_UnselectSlave();
}

/*
** ===================================================================
**     Method      :  EE241_ReadBlock (component 24AA_EEPROM)
**     Description :
**         Read a block of memory.
**     Parameters  :
**         NAME            - DESCRIPTION
**         addr            - Address where to read the memory
**       * data            - Pointer to a buffer where to store the
**                           data
**         dataSize        - Size of buffer the data pointer
**                           is pointing to
**     Returns     :
**         ---             - Error code, possible values
**                           ERR_OK - OK
**                           otherwise it can return an error code of
**                           the underlying communication protocol.
** ===================================================================
*/
byte EE241_ReadBlock(EE241_Address addr, byte *data, word dataSize)
{
  uint8_t res;
  #if EE241_DEVICE_ID==EE241_DEVICE_ID_8
    uint8_t addr8;
    addr8 = (uint8_t)(addr&0xff);
  #else  
    uint8_t addr16[2];                  /* big endian address on I2C bus needs to be 16bit */
    addr16[0] = (uint8_t)(addr>>8); /* 16 bit address must be in big endian format */
    addr16[1] = (uint8_t)(addr&0xff);
  #endif

  res = GI2C1_SelectSlave(EE241_DEVICE_ADDR(addr));
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
  #if EE241_DEVICE_ID==EE241_DEVICE_ID_8
    res = GI2C1_WriteBlock(&addr8, 1, GI2C1_DO_NOT_SEND_STOP); /* send 8bit address */
  #else
    res = GI2C1_WriteBlock(addr16, 2, GI2C1_DO_NOT_SEND_STOP); /* send 16bit address */
  #endif
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
  res = GI2C1_ReadBlock(data, dataSize, GI2C1_SEND_STOP);
  if (res != ERR_OK) {
    (void)GI2C1_UnselectSlave();
    return res;
  }
  return GI2C1_UnselectSlave();
}

/*
** ===================================================================
**     Method      :  EE241_WriteBlockPage (component 24AA_EEPROM)
**
**     Description :
**         Writes a block with pages of data to the EEPROM
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#ifdef __HIWARE__
#pragma MESSAGE DISABLE C1855 /* recursive function call */
#endif
byte EE241_WriteBlockPage(EE241_Address addr, byte *data, word dataSize)
{
  uint8_t res, i, *p, block[EE241_BLOCK_BUF_SIZE+2]; /* additional 2 bytes for the address */
  uint16_t eepromPage = (uint16_t)(addr/EE241_PAGE_SIZE);
  uint8_t offset = (uint8_t)(addr%EE241_PAGE_SIZE);

  if (dataSize==0 || dataSize>EE241_BLOCK_BUF_SIZE) {
    return ERR_OVERFLOW;                /* you may increase the buffer size in the properties? */
  }
  if (dataSize>EE241_PAGE_SIZE) {
    uint16_t size;

    size = (uint16_t)(EE241_PAGE_SIZE-offset);
    if (size!=0) {
      res = EE241_WriteBlock(addr, data, size); /* first page write */
      if (res != ERR_OK) {
        return res;
      }
      data += size; /* increment data pointer */
      addr += size; /* increment address */
      dataSize -= size; /* reduce size */
    }
    /* write multiple block of PAGE_SIZE */
    while (dataSize>EE241_PAGE_SIZE) {
      res = EE241_WriteBlock(addr, data, EE241_PAGE_SIZE);
      if (res != ERR_OK) {
        return res;
      }
      data += EE241_PAGE_SIZE; /* increment data pointer */
      addr += EE241_PAGE_SIZE; /* increment address */
      dataSize -= EE241_PAGE_SIZE; /* reduce size */
    }
    /* write remainder (if any) */
    if (dataSize>0) {
      return EE241_WriteBlock(addr, data, dataSize);
    }
    return ERR_OK;
  }
  if (offset+dataSize <= EE241_PAGE_SIZE) { /* no page boundary crossing */
    res = GI2C1_SelectSlave(EE241_DEVICE_ADDR(addr));
    if (res != ERR_OK) {
      (void)GI2C1_UnselectSlave();
      return res;
    }
    #if EE241_DEVICE_ID==EE241_DEVICE_ID_8 
      /* 8 bit address byte, high byte of address have been place in SelectSlave(addr) */
      block[0] = (uint8_t)(addr&0xff);  /* low byte of address */
      p = &block[1]; i = (uint8_t)dataSize;
    #else /* 16 bit address byte */
      block[0] = (uint8_t)(addr>>8);    /* high byte of address */
      block[1] = (uint8_t)(addr&0xff);  /* low byte of address */
      p = &block[2]; i = (uint8_t)dataSize;
    #endif

    /* copy block */
    while(i>0) {
      *p++ = *data++;
      i--;
    }
    res = GI2C1_WriteBlock(block, 
        dataSize+((EE241_DEVICE_ID==EE241_DEVICE_ID_8)? 1:2), GI2C1_SEND_STOP); /* send address and data */
    if (res != ERR_OK) {
      (void)GI2C1_UnselectSlave();
      return res;
    }
#if EE241_DO_ACKNOWLEDGE_POLLING
    /* do acknowledge polling */
    block[0] = 0xff; /* dummy value */
    do {
      WAIT1_WaitOSms(EE241_PAGE_WRITE_TIME_MS);
      res = GI2C1_ProbeACK(block, 1, GI2C1_SEND_STOP, EE241_ACK_POLLING_TIME_US); /* send address and data */
    } while(res!=ERR_OK); /* wait until we get an ACK */
    if (res != ERR_OK) {
      (void)GI2C1_UnselectSlave();
      return res;
    }
#endif /* EE241_DO_ACKNOWLEDGE_POLLING */
    return GI2C1_UnselectSlave();
  } else { /* crossing page boundaries: make two page writes */
    res = EE241_WriteBlock(addr, data, (uint16_t)(EE241_PAGE_SIZE-offset)); /* first page write */
    if (res != ERR_OK) {
      return res;
    }
    res = EE241_WriteBlock((EE241_Address)((eepromPage+1)*EE241_PAGE_SIZE),
       data+(EE241_PAGE_SIZE-offset),
       (uint16_t)(dataSize-(EE241_PAGE_SIZE-offset))); /* first page write */
    if (res != ERR_OK) {
      return res;
    }
  }
  return res;
}
#ifdef __HIWARE__
  #pragma MESSAGE DEFAULT C1855 /* recursive function call */
#endif

/*
** ===================================================================
**     Method      :  EE241_WriteBlock (component 24AA_EEPROM)
**     Description :
**         Writes a block of data to the EEPROM
**     Parameters  :
**         NAME            - DESCRIPTION
**         addr            - Address of memory
**       * data            - Pointer to the data
**         dataSize        - Size of data
**     Returns     :
**         ---             - Error code, possible values
**                           ERR_OK - OK
**                           ERR_OVERFLOW - data block passed has either
**                           size of zero or exceeds internal buffer
**                           size
**                           otherwise it can return an error code of
**                           the underlying communication protocol.
** ===================================================================
*/
byte EE241_WriteBlock(EE241_Address addr, byte *data, word dataSize)
{
  int32_t size;

  if (dataSize<=EE241_BLOCK_BUF_SIZE) { /* fits into internal buffer */
    return EE241_WriteBlockPage(addr, data, dataSize);
  }
  size = dataSize;
  while(size>=EE241_BLOCK_BUF_SIZE) { /* write in chunks EE241_BLOCK_BUF_SIZE */
    if (EE241_WriteBlock(addr, data, EE241_BLOCK_BUF_SIZE)!=ERR_OK) {
      return ERR_FAILED;
    }
    addr += EE241_BLOCK_BUF_SIZE;
    data += EE241_BLOCK_BUF_SIZE;
    size -= EE241_BLOCK_BUF_SIZE;
  }
  if (size>0) { /* write remainder which is < EE241_BLOCK_BUF_SIZE  */
    if (EE241_WriteBlockPage(addr, data, size)!=ERR_OK) {
      return ERR_FAILED;
    }
  }
  return ERR_OK;
}

/* END EE241. */

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
