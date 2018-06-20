/**************************************************************************************************/
/*                               ROM Library Functions for the SP37                               */
/**************************************************************************************************/
/*                                                                                                */
/*   Filename:    SP37_ROMLibrary.h                                                               */
/*                                                                                                */
/*   Description: Defines the Prototypes for the ROM Library functions in the SP37                */
/*                                                                                                */
/*   Target:      SP37                                                                            */
/*                                                                                                */
/*   Dev. Envir:  Keil uVision3 V3.51 (C51 V8.08, A51 V8.00d, BL51 V6.05)                         */
/*                                                                                                */
/*                                                                                                */
/**************************************************************************************************/
/*  (C)opyright Infineon Technologies AG 2005-2009. All rights reserved.                          */
/**************************************************************************************************/

// ----------------------------------------------------------------------------------------------------
//
// =========================================================
// = Reservation of the Idata memory used by LIB functions =
// =========================================================
//
// The Library Functions use a specified block of Idata memory
// that MUST NOT be used by user functions.
//
// RAM Usage
// ---------
//
// From address 0xC1 until address 0xEA (42 bytes)
//
// C-style
//
//      unsigned char idata LibSpace[0x2A] _at_ 0xC1;
//
// ASM-style
//
//      ISEG AT 0xC1
//      DS 02Ah



// ========================================================
// = Reservation of the Idata memory used by the debugger =
// ========================================================
//
// The debugger needs some internal stack for debugging user
// programs that MUST NOt be used by user functions.
//
// RAM Usage
// ---------
//
// From address 0xEB until address 0xFF (21 bytes)
//
// C-style
//
//      unsigned char idata DebuggerStack[0x15] _at_ 0xEB;
//
// ASM-style
//
//      ISEG AT 0xEB
//      DS 015h
//
// ----------------------------------------------------------------------------------------------------

#ifdef C_LIBFCT_H
   #ifdef A_LIBFCT_H
   #endif
#else
   #ifdef __C51__
     #define C_LIBFCT_H
   #else
     #define A_LIBFCT_H


  #define USED     1
  #define UNUSED   0


/*  --  A T T E N T I O N  --  A T T E N T I O N  --  A T T E N T I O N  --  A T T E N T I O N  --
/* *************************************************************************************************/
/* This section has to be filled out by the Customer when using assembler source files:            */
/* ====================================================================================            */
/* Every function that is called from an assembler source file has to be marked as "USED".         */
/* By default the funcions are marked as "UNUSED" to avoid a loading into the Flash memory without */
/* further usage. For C source files this loading is done automatically by the linker.             */
/* *************************************************************************************************/
// unused的含义是，用到了就去链接，没用到就不需要链接；used是不管用没用到都需要链接。
// 用汇编方式调用的标记为USED，用C方式调用的标记为UNUSED
  #define ADC_Selftest                     UNUSED
  #define Comp_Temperature                 UNUSED
  #define CRC8_Calc                        UNUSED
  #define CRC_Baicheva_Calc                UNUSED
  #define CRC16_Check                      UNUSED
  #define ECC_Check                        UNUSED
  #define Erase_UserConfigSector           UNUSED
  #define FlashSetLock                     UNUSED
  #define FW_Revision_Nb                   UNUSED
  #define Get_Supply_Voltage               UNUSED
  #define GetCompValue                     UNUSED
  #define HIRC_Clock_Check                 UNUSED
  #define Internal_SFR_Refresh             UNUSED
  #define IntervalTimerCalibration         UNUSED
  #define LFBaudrateCalibration            UNUSED
  #define ManuRevNb                        UNUSED
  #define Meas_Acceleration                UNUSED
  #define Meas_Pressure                    UNUSED
  #define Meas_Sensor                      UNUSED
  #define Meas_Supply_Voltage              UNUSED
  #define Meas_Temperature                 UNUSED
  #define PLL_Ref_Signal_Check             UNUSED
  #define Powerdown                        UNUSED
  #define Raw_Temperature                  UNUSED
  #define Read_ID                          UNUSED
  #define Scale_Pressure                   UNUSED
  #define SMulIntInt                       UNUSED
  #define Start_Supply_Voltage             UNUSED
  #define StartXtalOsc                     UNUSED
  #define StopXtalOsc                      UNUSED
  #define ThermalShutdown                  UNUSED
  #define Trig_Supply_Voltage              UNUSED
  #define UDivIntInt                       UNUSED
  #define UDivLongLong                     UNUSED
  #define VCO_Tuning                       UNUSED
  #define WriteFlashUserConfigSectorLine   UNUSED
  #define Send_RF_Telegram                 UNUSED
  #define Wait100usMultiples               UNUSED
#endif

/* ****************************************************************************************************/


// -----------------------
// Macros and symbols definitions
// -----------------------




// -----------------------
// Type definitions
// -----------------------

#ifdef __C51__
/**
Structure of the Chip's Identification information.
*/
typedef struct {
    /** The ID code itself */
    unsigned long int ID;	// 屏幕上显示的ID，就是从这里得到的，不同的芯片的ID是不一样的。
    /** Product Code pressure range indication: \n
        - xxxx.x000B: reserved
        - xxxx.x100B: 450 kPa, Green Package
        - xxxx.x001B: reserved
        - xxxx.x101B: reserved
        - xxxx.xx10B: reserved
        - xxxx.xx11B: reserved
    */
    unsigned char ProdCode;	// 测量压力的范围(100-450kPa)
    } ID_Struct;
#endif

// -----------------------
// - Function prototypes -
// -----------------------

#ifdef __C51__
   extern unsigned char ADC_Selftest (signed int idata *Delta);		// 如果其他文件想调用这个函数，只需在那个文件中包含这个头文件即可。
#else
  #if ADC_Selftest == USED
    #undef ADC_Selftest
    EXTRN CODE (_ADC_Selftest)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Comp_Temperature(signed int idata TempRawIn, signed int idata *TempResult);
#else
  #if Comp_Temperature == USED
     #undef Comp_Temperature
     EXTRN CODE (_Comp_Temperature)
  #endif
#endif


#ifdef __C51__
   extern unsigned char CRC8_Calc(unsigned char Preload, unsigned char idata *BlockStart, unsigned char BlockLength);
#else
  #if CRC8_Calc == USED
     #undef CRC8_Calc
     EXTRN CODE (_CRC8_Calc)
  #endif
#endif


#ifdef __C51__
   extern unsigned char CRC_Baicheva_Calc(unsigned char Preload, unsigned char idata *BlockStart, unsigned char BlockLength);
#else
  #if CRC_Baicheva_Calc == USED
     #undef CRC_Baicheva_Calc
     EXTRN CODE (_CRC_Baicheva_Calc)
  #endif
#endif


#ifdef __C51__
   extern signed char CRC16_Check(unsigned char code *StartAddr, unsigned int Length);
#else
  #if CRC16_Check == USED
     #undef CRC16_Check
     EXTRN CODE (CRC16_Check)
  #endif
#endif


#ifdef __C51__
   extern signed char ECC_Check (void);
#else
  #if ECC_Check == USED
     #undef ECC_Check
     EXTRN CODE (ECC_Check)
  #endif
#endif


#ifdef __C51__
   extern signed char Erase_UserConfigSector(void);
#else
  #if Erase_UserConfigSector == USED
     #undef Erase_UserConfigSector
     EXTRN CODE (Erase_UserConfigSector)
  #endif
#endif


#ifdef __C51__
   extern signed char FlashSetLock(void);
#else
  #if FlashSetLock == USED
     #undef FlashSetLock
     EXTRN CODE (FlashSetLock)
  #endif
#endif


#ifdef __C51__
   extern void FW_Revision_Nb (unsigned int idata *ROM_Rev, unsigned int idata *Lib_Rev);
#else
  #if FW_Revision_Nb == USED
     #undef FW_Revision_Nb
     EXTRN CODE (_FW_Revision_Nb)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Get_Supply_Voltage(signed int idata *BattResult);
#else
  #if Get_Supply_Voltage == USED
     #undef Get_Supply_Voltage
     EXTRN CODE (_Get_Supply_Voltage)
  #endif
#endif


#ifdef __C51__
   extern unsigned char GetCompValue(unsigned char code *TablePointer, unsigned char Value1, unsigned char Value2);
#else
  #if GetCompValue == USED
     #undef GetCompValue
     EXTRN CODE (_GetCompValue)
  #endif
#endif


#ifdef __C51__
   extern signed char HIRC_Clock_Check(void);
#else
  #if HIRC_Clock_Check == USED
     #undef HIRC_Clock_Check
     EXTRN CODE (HIRC_Clock_Check)
  #endif
#endif


#ifdef __C51__
   extern void Internal_SFR_Refresh(void);
#else
  #if Internal_SFR_Refresh == USED
     #undef Internal_SFR_Refresh
     EXTRN CODE (Internal_SFR_Refresh)
  #endif 
#endif


#ifdef __C51__
   extern signed char IntervalTimerCalibration(unsigned char WU_Frequency);
#else
  #if IntervalTimerCalibration == USED
     #undef IntervalTimerCalibration
     EXTRN CODE (_IntervalTimerCalibration)
  #endif 
#endif


#ifdef __C51__
   extern signed char LFBaudrateCalibration(unsigned int Baudrate);
#else
  #if LFBaudrateCalibration == USED
     #undef LFBaudrateCalibration
     EXTRN CODE (_LFBaudrateCalibration)
  #endif
#endif


#ifdef __C51__
   extern signed int ManuRevNb(void);
#else
  #if ManuRevNb == USED
     #undef ManuRevNb
     EXTRN CODE (ManuRevNb)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Meas_Acceleration (unsigned int SensorConfig, unsigned char SampRate, signed int idata *AccelResult);
#else
  #if Meas_Acceleration == USED
    #undef Meas_Acceleration
    EXTRN CODE (_Meas_Acceleration)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Meas_Pressure (unsigned int SensorConfig, unsigned char SampRate, signed int idata *PressResult );
#else
  #if Meas_Pressure == USED
    #undef Meas_Pressure
    EXTRN CODE (_Meas_Pressure)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Meas_Sensor (unsigned int SensorConfig, unsigned char SampRate, signed int idata *SensorResult );
#else
  #if Meas_Sensor == USED
    #undef Meas_Sensor
    EXTRN CODE (_Meas_Sensor)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Meas_Supply_Voltage(signed int idata *BattResult);
#else
  #if Meas_Supply_Voltage == USED
    #undef Meas_Supply_Voltage
    EXTRN CODE (_Meas_Supply_Voltage)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Meas_Temperature(signed int idata *TempResult);
#else
  #if Meas_Temperature == USED
    #undef Meas_Temperature
    EXTRN CODE (_Meas_Temperature)
  #endif
#endif


#ifdef __C51__
   signed char PLL_Ref_Signal_Check(void);
#else
  #if PLL_Ref_Signal_Check == USED
    #undef PLL_Ref_Signal_Check
    EXTRN CODE (PLL_Ref_Signal_Check)
  #endif
#endif


#ifdef __C51__
   extern void Powerdown(void);
#else
  #if Powerdown == USED
    #undef Powerdown
    EXTRN CODE (Powerdown)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Raw_Temperature(signed int idata *TempResult);
#else
  #if Raw_Temperature == USED
     #undef Raw_Temperature
     EXTRN CODE (_Raw_Temperature)
  #endif
#endif


#ifdef __C51__
   extern void Read_ID(ID_Struct idata * idata ID_Result);
#else
  #if Read_ID == USED
     #undef Read_ID
     EXTRN CODE (_Read_ID)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Scale_Pressure(signed int idata* Measurement_Result);
#else
  #if Scale_Pressure == USED
     #undef Scale_Pressure
     EXTRN CODE (_Scale_Pressure)
  #endif
#endif


#ifdef __C51__
   extern unsigned char Start_Supply_Voltage(void);
#else
  #if Start_Supply_Voltage == USED
     #undef Start_Supply_Voltage
     EXTRN CODE (Start_Supply_Voltage)
  #endif
#endif


#ifdef __C51__
   extern signed char StartXtalOsc(unsigned char Delay);
#else
  #if StartXtalOsc == USED
     #undef StartXtalOsc
     EXTRN CODE (_StartXtalOsc)
  #endif
#endif


#ifdef __C51__
   extern signed char StopXtalOsc(void);
#else
  #if StopXtalOsc == USED
     #undef StopXtalOsc
     EXTRN CODE (StopXtalOsc)
  #endif
#endif


#ifdef __C51__
   extern void ThermalShutdown(void);
#else
  #if ThermalShutdown == USED
     #undef ThermalShutdown
     EXTRN CODE (ThermalShutdown)
  #endif
#endif


#ifdef __C51__
   extern void Trig_Supply_Voltage (void);
#else
  #if Trig_Supply_Voltage == USED
     #undef Trig_Supply_Voltage
     EXTRN CODE (Trig_Supply_Voltage)
  #endif
#endif


#ifdef __C51__
   extern void SMulIntInt(signed int idata * Multiplicand1, signed int idata * Multiplicand2, signed long idata * Product);
#else 
  #if SMulIntInt == USED
    #undef SMulIntInt
	EXTRN CODE (_SMulIntInt)
  #endif
#endif


#ifdef __C51__
   extern unsigned int UDivIntInt(unsigned int Dividend, unsigned int Divisor);
#else 
  #if UDivIntInt == USED
    #undef UDivIntInt
	EXTRN CODE (_UDivIntInt)
  #endif
#endif


#ifdef __C51__
   extern unsigned long UDivLongLong(unsigned long idata *Dividend, unsigned long idata *Divisor);
#else 
  #if UDivLongLong == USED
    #undef UDivLongLong
	EXTRN CODE (_UDivLongLong)
  #endif
#endif


#ifdef __C51__
   extern signed char VCO_Tuning (void);
#else 
  #if VCO_Tuning == USED
    #undef VCO_Tuning
    EXTRN CODE (VCO_Tuning)
  #endif
#endif


#ifdef __C51__
   extern signed char WriteFlashUserConfigSectorLine(unsigned int Startaddress, unsigned char idata *WrData);
#else
  #if WriteFlashUserConfigSectorLine == USED
     #undef WriteFlashUserConfigSectorLine
     EXTRN CODE (_WriteFlashUserConfigSectorLine)
  #endif
#endif


#ifdef __C51__
   extern void Wait100usMultiples(unsigned int Counter);
#else
  #if Wait100usMultiples == USED
     #undef Wait100usMultiples
     EXTRN CODE (_Wait100usMultiples)
  #endif
#endif


#ifdef __C51__
    extern signed char Send_RF_Telegram(unsigned int baudrate, unsigned char idata * descriptorPtr);
#else
  #if Send_RF_Telegram == USED
     #undef Send_RF_Telegram
     EXTRN CODE (_Send_RF_Telegram)
  #endif
#endif

#endif // C_LIBFCT_H
