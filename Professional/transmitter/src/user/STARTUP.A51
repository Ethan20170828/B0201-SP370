;/* ****************************************************************************************************/
;/*                             Startup File for SP37                      							   */
;/*   -----------------------------------------------------------------------------------------------  */
;/*                                                                                                    */   
;/*   Purpose: 	 Sets the Stackpointer and defines the used Crystal                                    */
;/*				        																		       */
;/*                                                                                                    */
;/*                                                                                                    */
;/*   Filename:   STARTUP.A51		                                                                   */
;/*                                                                                                    */
;/*   Target:     SP37                                                                                 */
;/*                                                                                                    */
;/*   Dev. Envir: Keil Software C51 V7.10                                                              */
;/*               µVision2 V2.40                                                                       */
;/*   Author:     KBu, SH		                                                 	   		           */
;/*                                                                     	                           */
;/*   History:    19.October 2005: First Release				   									   */
;/*				  10.October 2006: Update for Target Datasheet V1.2                                    */
;/*                                                                                                    */
;/*   Status:     Release V1.1                                                                         */
;/* ************************************************************************************************** */
;/*  (C)opyright Infineon Technologies AG. All rights reserved.                                        */
;/* ****************************************************************************************************/
;/*  This SOFTWARE is Provided "AS IS" Without ANY WARRANTIES OF ANY KIND, WHETHER 
;/*  EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF 
;/*  MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE or warranties of 
;/*  non-infringement of THIRD PARTY intellectual property rights. infineon 
;/*  disclaims all liability regardless of the cause in law, in particular, but 
;/*  not limited to, the liability for indirect or consequential damages arising 
;/*  from interrupted operation, loss of profits, loss of information and data, 
;/*  unless in cases of gross negligence, intent, lack of assured characteristics 
;/*  or in any cases where liability is mandatory at law.
;/* ****************************************************************************************************/ 


#define _18080000_Crystal_433_92MHz

NAME	SEG_C_STARTUP

		ISEG	AT 0A0h		; select the stack segment
?STACK:		DS	  20h   	; reserve 32bytes of space

PUBLIC	?C_STARTUP

; Example how to insert a crystal clock value of 18.884MHz
; ========================================================

; First, divide the crystal clock value by 2: 

;       18.080MHz 
;       ---------  = 9.040MHz
;           2

; Then transform the result of the division into a hexadecimal value: 

; 9.040MHz = 9040000 Hz => 0x89F080 Hz

; The result of this calculation has to be a 3 byte value in the range of:
; 0x989689 >= crystal clock frequency >= 0x895440
; This result has to be inserted in the User Configuration Sector at address 0x57FA (MSByte)
; to 0x57FC (LSByte)

CSEG AT 0x57FA 

#ifdef _19687500_Crystal_315MHz		
		DB  0x96 ; MSByte
        DB  0x34
        DB  0x26 ; LSByte 
#endif

#ifdef _18080000_Crystal_433_92MHz
        DB	0x89 ; MSByte 
        DB  0xF0
        DB  0x80 ; LSByte 
#endif


; After Reset:
CSEG	AT 	04000h
EXTRN CODE (MAIN)

?C_STARTUP:
      mov       SP, #?STACK - 1		; load stack pointer
      ljmp      MAIN                    ; Goto Main


END
