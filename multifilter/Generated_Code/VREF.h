/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : VREF.h
**     Project     : multifilter
**     Processor   : MK22FN512VLH12
**     Component   : Init_VREF
**     Version     : Component 01.004, Driver 01.04, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-02-19, 02:32, # CodeGen: 0
**     Abstract    :
**          This file implements the VREF (VREF) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : VREF
**          Device                                         : VREF
**          Settings                                       : 
**            Clock gate                                   : Enabled
**            Mode                                         : Tight
**            Regulator                                    : Enabled
**          Pins                                           : 
**            VREFO Pin                                    : Enabled
**              Output Pin                                 : <Automatic>
**          Initialization                                 : 
**            Enable VREF                                  : yes
**            Call Init method                             : yes
**            Utilize after reset values                   : default
**     Contents    :
**         Init - void VREF_Init(void);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
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
** @file VREF.h
** @version 01.04
** @brief
**          This file implements the VREF (VREF) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup VREF_module VREF module documentation
**  @{
*/         

#ifndef VREF_H_
#define VREF_H_

/* MODULE VREF. */

#include "VREF_Init.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialization method user name */
#define VREF_Init VREF_Init
/* PDD macros peripheral base address parameter */
#define VREF_DEVICE VREF_BASE_PTR

#ifdef __cplusplus
}
#endif

/* END VREF. */
#endif /* #ifndef __VREF_H_ */
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

