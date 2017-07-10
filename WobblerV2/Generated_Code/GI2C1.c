/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : GI2C1.c
**     Project     : WobblerV2
**     Processor   : MKL02Z32VFM4
**     Component   : GenericI2C
**     Version     : Component 01.023, Driver 01.00, CPU db: 3.00.000
**     Repository  : Legacy User Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-07-04, 08:30, # CodeGen: 0
**     Abstract    :
**         This component implements a generic I2C driver wrapper to work both with LDD and non-LDD I2C components.
**     Settings    :
**          Component name                                 : GI2C1
**          Wait                                           : WAIT1
**          Support STOP_NOSTART                           : no
**          Write Buffer Size                              : 16
**          non-LDD I2C                                    : Enabled
**            I2C                                          : CI2C1
**          LDD I2C                                        : Disabled
**          RTOS                                           : Disabled
**          Init() on startup                              : yes
**     Contents    :
**         SelectSlave       - byte GI2C1_SelectSlave(byte i2cAddr);
**         UnselectSlave     - byte GI2C1_UnselectSlave(void);
**         RequestBus        - void GI2C1_RequestBus(void);
**         ReleaseBus        - void GI2C1_ReleaseBus(void);
**         ReadBlock         - byte GI2C1_ReadBlock(void* data, word dataSize, GI2C1_EnumSendFlags flags);
**         WriteBlock        - byte GI2C1_WriteBlock(void* data, word dataSize, GI2C1_EnumSendFlags flags);
**         ReadAddress       - byte GI2C1_ReadAddress(byte i2cAddr, byte *memAddr, byte memAddrSize, byte...
**         WriteAddress      - byte GI2C1_WriteAddress(byte i2cAddr, byte *memAddr, byte memAddrSize, byte...
**         ReadByteAddress8  - byte GI2C1_ReadByteAddress8(byte i2cAddr, byte memAddr, byte *data);
**         WriteByteAddress8 - byte GI2C1_WriteByteAddress8(byte i2cAddr, byte memAddr, byte data);
**         ProbeACK          - byte GI2C1_ProbeACK(void* data, word dataSize, GI2C1_EnumSendFlags flags,...
**         GetSemaphore      - void* GI2C1_GetSemaphore(void);
**         Deinit            - void GI2C1_Deinit(void);
**         ScanDevice        - byte GI2C1_ScanDevice(byte i2cAddr);
**         Init              - void GI2C1_Init(void);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2013-2014, all rights reserved.
**     http          : www.mcuoneclipse.com
**     This an open source software implementing software using Processor Expert.
**     This is a free software and is opened for education,  research  and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file GI2C1.c
** @version 01.00
** @brief
**         This component implements a generic I2C driver wrapper to work both with LDD and non-LDD I2C components.
*/         
/*!
**  @addtogroup GI2C1_module GI2C1 module documentation
**  @{
*/         

/* MODULE GI2C1. */

#include "GI2C1.h"
#ifndef NULL
  #define NULL 0L
#endif /* NULL */

/*
** ===================================================================
**     Method      :  GI2C1_RequestBus (component GenericI2C)
**     Description :
**         Starts a critical section for accessing the bus.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void GI2C1_RequestBus(void)
{
}

/*
** ===================================================================
**     Method      :  GI2C1_ReleaseBus (component GenericI2C)
**     Description :
**         Finishes a critical section for accessing the bus.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void GI2C1_ReleaseBus(void)
{
}

/*
** ===================================================================
**     Method      :  GI2C1_SelectSlave (component GenericI2C)
**     Description :
**         Selects the slave device on the bus. Method might use a
**         semaphore to protect bus access.
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_SelectSlave(byte i2cAddr)
{
  if (CI2C1_SelectSlave(i2cAddr)!=ERR_OK) {
    return ERR_FAILED;
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  GI2C1_UnselectSlave (component GenericI2C)
**     Description :
**         Unselects the device. Method will release a used a semaphore.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_UnselectSlave(void)
{
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  GI2C1_ReadBlock (component GenericI2C)
**     Description :
**         Read from the device a block.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Read buffer
**         dataSize        - Size of read buffer
**         flags           - flags for the transaction
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_ReadBlock(void* data, word dataSize, GI2C1_EnumSendFlags flags)
{
  byte res = ERR_OK;
  word nof;

  for(;;) { /* breaks */
    res = CI2C1_RecvBlock(data, dataSize, &nof);
    if (res!=ERR_OK) {
      break; /* break for(;;) */
    }
    if (flags==GI2C1_SEND_STOP) {
      res = CI2C1_SendStop();
      if (res!=ERR_OK) {
        break; /* break for(;;) */
      }
    }
    break; /* break for(;;) */
  } /* for(;;) */
  return res;
}

/*
** ===================================================================
**     Method      :  GI2C1_WriteBlock (component GenericI2C)
**     Description :
**         Write a block to the device.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Data write buffer
**         dataSize        - 
**         flags           - flags for the transaction
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_WriteBlock(void* data, word dataSize, GI2C1_EnumSendFlags flags)
{
  word nof;
  byte res = ERR_OK;

  for(;;) { /* breaks */
    res = CI2C1_SendBlock(data, dataSize, &nof);
    if (res!=ERR_OK) {
      (void)CI2C1_SendStop();
      break; /* break for(;;) */
    }
    if (flags==GI2C1_SEND_STOP || (flags==GI2C1_STOP_NOSTART)) {
      res = CI2C1_SendStop();
      if (res!=ERR_OK) {
        break; /* break for(;;) */
      }
    }
    break; /* break for(;;) */
  } /* for(;;) */
  return res;
}

/*
** ===================================================================
**     Method      :  GI2C1_ReadAddress (component GenericI2C)
**     Description :
**         Read from the device. This writes (S+i2cAddr+0), (memAddr),
**         (Sr+i2cAddr+1), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**       * memAddr         - Pointer to device memory address
**         memAddrSize     - number of address bytes
**       * data            - Pointer to read buffer
**         dataSize        - Size of read buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_ReadAddress(byte i2cAddr, byte *memAddr, byte memAddrSize, byte *data, word dataSize)
{
  byte res = ERR_OK;
  word nof;

  if (GI2C1_SelectSlave(i2cAddr)!=ERR_OK) {
    return ERR_FAILED;
  }
  for(;;) { /* breaks */
    res = CI2C1_SendBlock((void*)memAddr, memAddrSize, &nof);
    if (res!=ERR_OK) {
      (void)CI2C1_SendStop();
      break; /* break for(;;) */
    }
    res = CI2C1_RecvBlock(data, dataSize, &nof);
    if (res!=ERR_OK) {
      (void)CI2C1_SendStop();
      break; /* break for(;;) */
    }
    res = CI2C1_SendStop();
    if (res!=ERR_OK) {
      break; /* break for(;;) */
    }
    break; /* break for(;;) */
  } /* for(;;) */
  if (GI2C1_UnselectSlave()!=ERR_OK) {
    return ERR_FAILED;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  GI2C1_WriteAddress (component GenericI2C)
**     Description :
**         Write to the device: (S+i2cAddr+0), (memAddr), (data)...
**         (data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**       * memAddr         - Pointer to device memory address
**         memAddrSize     - number of address bytes
**       * data            - Pointer to data write buffer
**         dataSize        - Size of data buffer in bytes
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_WriteAddress(byte i2cAddr, byte *memAddr, byte memAddrSize, byte *data, word dataSize)
{
  static byte writeBuf[GI2C1_WRITE_BUFFER_SIZE];
  byte *p;
  word i;
  word nof;
  byte res = ERR_OK;

  if (GI2C1_SelectSlave(i2cAddr)!=ERR_OK) {
    return ERR_FAILED;
  }
  if (memAddrSize+dataSize>GI2C1_WRITE_BUFFER_SIZE) {
    return ERR_FAILED;
  }
  i = 0; p = memAddr;
  while(i<GI2C1_WRITE_BUFFER_SIZE && memAddrSize>0) {
    writeBuf[i++] = *p++;
    memAddrSize--;
  }
  p = data;
  while(i<GI2C1_WRITE_BUFFER_SIZE && dataSize>0) {
    writeBuf[i++] = *p++;
    dataSize--;
  }
  for(;;) { /* breaks */
    res = CI2C1_SendBlock((void*)writeBuf, i, &nof);
    if (res!=ERR_OK) {
      (void)CI2C1_SendStop();
      break; /* break for(;;) */
    }
    res = CI2C1_SendStop();
    if (res!=ERR_OK) {
      break; /* break for(;;) */
    }
    break; /* break for(;;) */
  } /* for(;;) */
  if (GI2C1_UnselectSlave()!=ERR_OK) {
    return ERR_FAILED;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  GI2C1_Init (component GenericI2C)
**     Description :
**         Initializes the driver.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void GI2C1_Init(void)
{
}

/*
** ===================================================================
**     Method      :  GI2C1_Deinit (component GenericI2C)
**     Description :
**         Deinitializes the driver.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void GI2C1_Deinit(void)
{
}

/*
** ===================================================================
**     Method      :  GI2C1_GetSemaphore (component GenericI2C)
**     Description :
**         Returns the currently allocated semaphore.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
void* GI2C1_GetSemaphore(void)
{
  return NULL; /* RTOS and Semaphore enabled in properties */
}

/*
** ===================================================================
**     Method      :  GI2C1_ReadByteAddress8 (component GenericI2C)
**     Description :
**         Read a byte from the device using an 8bit memory address.
**         This writes (S+i2cAddr+0), (memAddr), (Sr+i2cAddr+1), (data)..
**         .(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**         memAddr         - Device memory address
**       * data            - Pointer to read buffer (single byte)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_ReadByteAddress8(byte i2cAddr, byte memAddr, byte *data)
{
  return GI2C1_ReadAddress(i2cAddr, &memAddr, sizeof(memAddr), data, 1);
}

/*
** ===================================================================
**     Method      :  GI2C1_WriteByteAddress8 (component GenericI2C)
**     Description :
**         Write a byte to the device using an 8bit memory address:
**         (S+i2cAddr+0), (memAddr), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**         memAddr         - Device memory address
**         data            - Data value
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_WriteByteAddress8(byte i2cAddr, byte memAddr, byte data)
{
  return GI2C1_WriteAddress(i2cAddr, &memAddr, sizeof(memAddr), &data, 1);
}

/*
** ===================================================================
**     Method      :  GI2C1_ScanDevice (component GenericI2C)
**     Description :
**         Checks if a device responds on the bus with an ACK.
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - 7bit I2C device address
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_ScanDevice(byte i2cAddr)
{
  byte res = ERR_OK;
  word nof;
  byte dummy;

  if (GI2C1_SelectSlave(i2cAddr)!=ERR_OK) {
    return ERR_FAILED;
  }
  for(;;) { /* breaks */
    res = CI2C1_RecvBlock((void*)&dummy, 1, &nof);
    if (res!=ERR_OK) {
      (void)CI2C1_SendStop();
      break; /* break for(;;) */
    }
    res = CI2C1_SendStop();
    if (res!=ERR_OK) {
      break; /* break for(;;) */
    }
    break; /* break for(;;) */
  } /* for(;;) */
  if (GI2C1_UnselectSlave()!=ERR_OK) {
    return ERR_FAILED;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  GI2C1_ProbeACK (component GenericI2C)
**     Description :
**         Accesses the bus to check if the device responds with an ACK
**         (ACK polling).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Data write buffer
**         dataSize        - 
**         flags           - flags for the transaction
**         WaitTimeUS      - Waiting time in microseconds
**                           to wait for the ACK on the bus.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
byte GI2C1_ProbeACK(void* data, word dataSize, GI2C1_EnumSendFlags flags, word WaitTimeUS)
{
  word nof;
  byte res = ERR_OK;

  (void)WaitTimeUS; /* not used */
  for(;;) { /* breaks */
    res = CI2C1_SendBlock(data, dataSize, &nof);
    if (res!=ERR_OK) {
      (void)CI2C1_SendStop();
      break; /* break for(;;) */
    }
    if (flags==GI2C1_SEND_STOP) {
      res = CI2C1_SendStop();
      if (res!=ERR_OK) {
        break; /* break for(;;) */
      }
    }
    break; /* break for(;;) */
  } /* for(;;) */
  return res;
}

/* END GI2C1. */

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
