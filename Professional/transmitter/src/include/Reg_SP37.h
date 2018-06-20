/**
@file Reg_SP370.h
@brief Declares all Special Function Registers for the SP37 including the SFRs
        of the Intel 80C51 code compatible CPU. The definition for the C Code
        only, because in the assembler they are known. For generic usage.
@author DI Klaus Buchner(SP35-D4), Markus Rauter (SP37) 

@verbatim
Target: SP37
Dev. Envir:   Keil uVision3 V3.51 (C51 V8.08, A51 V8.00d, BL51 V6.05)
History:
    Rev.224 27.02.2009  Created on AE request from IFX internal Register header file
Status: Development    
(C)opyright Infineon Technologies AG 2005-2008. All rights reserved.
@endverbatim
*/

/**
@addtogroup Registers
@{ */

#ifndef SP37_REGISTERS
  #define SP37_REGISTERS

/* ------------------------------------  MACRO DEFINITIONS  ----------------------------------------- */

#ifdef __C51__
  #ifndef C8051_REGISTERS_C
    #define C8051_REGISTERS_C
  #endif

/**
@addtogroup CPU_Special_SFR
@{ */
/** Accumulator */
sfr ACC      = 0xE0;   
/** B-Register */
sfr B        = 0xF0;   
/** Stack Pointer */
sfr SP       = 0x81;
/** Data Pointer 16-Bit */
sfr16 _DPTR  = 0x82;
/** Data Pointer Low Byte */
sfr DPL      = 0x82;   
/** Data Pointer High Byte */
sfr DPH      = 0x83;   

/** Program Status Word */
sfr PSW      = 0xD0;   
    /** Parity Flag */ 
    sbit P       = 0xD0;
    /** Free Bit 1 */
    sbit F1      = 0xD1;
    /** Overflow Flag */
    sbit OV      = 0xD2;
    /** Register Bank Selection Bit 0 */
    sbit RS0     = 0xD3;
    /** Register Bank Selection Bit 1 */
    sbit RS1     = 0xD4;   
    /** Free Bit 0 */
    sbit F0      = 0xD5;
    /** Auxilary Carry Flag */
    sbit AC      = 0xD6;
    /** Carry Flag */
    sbit CY      = 0xD7;   
/** @} */


/**
@addtogroup Timer_WDog_SFR
@{ */
/** Timer Control Register */ 
sfr TCON      = 0x88;
/** Timer 0 Register High Byte */
sfr TH0       = 0x8C;
/** Timer 1 Register High Byte */ 
sfr TH1       = 0x8D;
/** Timer 0 Register Low Byte */
sfr TL0       = 0x8A;
/** Timer 1 Register Low Byte */
sfr TL1       = 0x8B; 
/** Timer Mode Register */
sfr TMOD      = 0x89; 
/** @} */

#endif  /* __C51__ */


/**
@addtogroup Timer_Unit_Special_Bits 
@{ */
     /** Timer 0 Run Bit */
     sbit T0RUN  = 0x88;
     /** Timer 0 Full Bit */
     sbit T0FULL = 0x89; 
     /** Timer 1 Run Bit */
     sbit T1RUN  = 0x8C; 
     /** Timer 1 Full Bit */
     sbit T1FULL = 0x8D; 
/** @} */

/**
@addtogroup System_Configuration_Registers 
@{ */
/**  Configuration Register 0 */
sfr CFG0     = 0xF8;
    /** Enable XTAL Oscillator */
    sbit ENXOSC  = 0xFC; 
    /** Enter IDLE State - IDLE State (1), Run State (0) */
    sbit IDLE    = 0xFD; 
    /** Enter THERMAL SHUTDOWN State - TSHT State (1), Run State (0) */
    sbit TSHDWN  = 0xFE; 
    /**  Enter POWER DOWN State - PDWN State (1), Active RUN State (0) */
    sbit PDWN    = 0xFF;

/** Configuration Register 1 */
sfr CFG1     = 0xE8;
    /** Interval Timer Enable - (1 - Enable, 0 - Disable in Test Mode Only) */
    sbit ITEN    = 0xE8; 
    /** Interval Timer Initialization (0 - Init.complete, 1 - Init.in progress) */
    sbit ITINIT  = 0xE9; 
    /** Interval Timer Read Enable (0 - ITPR read result is not valid, 1 - ... is valid) */
    sbit ITRD    = 0xEA; 
    /** Rf-TX port out ENable (0 - Standard I/O Port functionality, 1 - Echoes RF transmission baseband data on port PP2/TXData */
    sbit RFTXPEN = 0xEC;
    /** Random Number Generator Enable */
    sbit RNGEN   = 0xED; 
    /** I2C Enable */
    sbit I2CEN   = 0xEE; 

/** Configuration Register 2 */
sfr CFG2     = 0xD8;     
    /** Activate Software Reset */
    sbit RESET   = 0xD8; 
    /** Reset Watchdog counter to 0 */
    sbit WDRES   = 0xD9; 
    /** Power Down iRAM lower memory block (0x00-0x7F) */
    sbit PDLMB   = 0xDC; 
    /** I2C General Call Enable */
    sbit I2CGCEN = 0xDE; 
/** @} */

/**
@addtogroup CRC_Generator_Checker_Registers
@{ */
/** CRC Control Register (CRC = Cyclic Redundancy Check) */
sfr CRCC     = 0xA9; 
/** CRC Data Register */
sfr CRCD     = 0xAA; 
/** CRC Shift Register (low byte) */
sfr CRC0     = 0xAC; 
/** CRC Shift Register (high byte) */
sfr CRC1     = 0xAD; 
/** @} */

/** Internal Clock Divider */
sfr DIVIC    = 0xB9;
/** Diagnosis and System Status Register */
sfr DSR      = 0xD9;  

/**
@addtogroup General_Purpose_Registers
@{ */
/** General Purpose Register 0 */
sfr GPR0     = 0xB8; 
/** General Purpose Register 1 */
sfr GPR1     = 0xB0; 
/** General Purpose Register 2 */
sfr GPR2     = 0xA8; 
/** General Purpose Register 3 */
sfr GPR3     = 0xF1; 
/** General Purpose Register 4 */
sfr GPR4     = 0xF2; 
/** General Purpose Register 5 */
sfr GPR5     = 0xF3; 
/** General Purpose Register 6 */
sfr GPR6     = 0xF5; 
/** General Purpose Register 7 */
sfr GPR7     = 0xF6; 
/** General Purpose Register 8 */
sfr GPR8     = 0xF7; 
/** General Purpose Register 9 */
sfr GPR9     = 0xF9; 
/** General Purpose Register 10 */
sfr GPRA     = 0xFA; 
/** General Purpose Register 11 */
sfr GPRB     = 0xFB; 
/** General Purpose Register 12 */
sfr GPRC     = 0xFC; 
/** General Purpose Register 13 */
sfr GPRD     = 0xFD; 
/** General Purpose Register 14 */
sfr GPRE     = 0xFE; 
/** General Purpose Register 15 */
sfr GPRF     = 0xFF; 
/** @} */

/**
@addtogroup I2C_Bus_Registers
@{ */
/** I2C Data Register */
sfr I2CD     = 0x9A;
/** I2C Status Register */
sfr I2CS     = 0x9B;
/** @} */

/**
@addtogroup Interval_Timer_Registers 
@{ */
/** Interval Timer Frequency Synthesizer (Low Byte) */
sfr ITPL     = 0xBA; 
/** Interval Timer Frequency Synthesizer (High Byte) */
sfr ITPH     = 0xBB; 
/** Interval Timer Period Register */
sfr ITPR     = 0xBC; 
/** @} */


/**
@addtogroup LF_Receiver_Registers 
@{ */
/** LF Carrier Detect Filtering */
sfr LFCDFLT   = 0xB2;
/** LF Carrier Detector Mode Register 0 */
sfr LFCDM0    = 0xB5; 
/** LF Division Factor */
sfr LFDIV     = 0xB4; 
/** LF On/Off Timer Configuration Register */
sfr LFOOT     = 0xC6; 
/** LF On/Off Timer Precounter */
sfr LFOOTP    = 0xC5; 
/** LF Pattern Detection Configuration Register */
sfr LFPCFG    = 0xC7; 
/** LF Pattern 0 Detector Sequence Data LSB */
sfr LFP0L     = 0xCC; 
/** LF Pattern 0 Detector Sequence Data MSB */
sfr LFP0H     = 0xCD; 
/** LF Pattern 1 Detector Sequence Data LSB */
sfr LFP1L     = 0xCE; 
/** LF Pattern 1 Detector Sequence Data MSB */
sfr LFP1H     = 0xCF; 
/** LF Receiver Configuration Register 0 */
sfr LFRX0     = 0xB7; 
/** LF Receiver Configuration Register 1 */
sfr LFRX1     = 0xB6; 
/** LF Receiver Configuration Register 2 */
sfr LFRX2     = 0xAF; 

/** LF Receiver Control Register */
sfr LFRXC     = 0x98; 
    /** LF ON/OFF indicator */
    sbit LFONIND = 0x98; 
    /** Synchronization indicator */
    sbit SYNCIND = 0x99; 
    /** Enable LF receiver */
    sbit ENLFRX  = 0x9A; 
    /** Enable on/off timer */
    sbit ENOOTIM = 0x9B; 
    /** LF baseband processor mode (bit 0) */
    sbit LFBBM0  = 0x9C; 
    /** LF baseband processor mode (bit 1) */
    sbit LFBBM1  = 0x9D; 
    /** Disable automatic gain control */
    sbit DISAGC  = 0x9E; 
    /** Restart Carrier Detect Recalibration */
    sbit CDRECAL = 0x9F; 

/** LF Receiver Data Register */
sfr LFRXD     = 0xA5; 
/** LF Receiver Status Register */
sfr LFRXS     = 0xA4; 
/** @} */


/**
@addtogroup Port_pin_assignment 
@{ */

/** IO-Port 1 Direction Register */
sfr P1DIR    = 0x91;
/** IO-Port 1 Data IN Register */
sfr P1IN     = 0x92; 

/** IO-Port 1 Data OUT Register */
sfr P1OUT    = 0x90;
    /** PP0 / P1.0 Port 1 Bit 0 Data Out / PullUp enable */
    sbit PPO0 = 0x90;
    /** PP1 / P1.1 Port 1 Bit 1 Data Out / PullUp enable */
    sbit PPO1 = 0x91;
    /**  PP2 / P1.2 Port 1 Bit 2 Data Out / PullUp enable */
    sbit PPO2 = 0x92;

/** IO-Port 1 Sensitivity Register */
sfr P1SENS   = 0x93; 

/** @} */


/** Resume Event Flag Register */
sfr REF      = 0xD1;


/**
@addtogroup RF_Encoder_Registers 
@{ */
/** RF-Transmitter Control Register */
sfr RFC      = 0xC8; 
	/** Enable RF Power Amplifier (PA) */
	sbit ENPA = 0xC8;
	/** Enable RF Frequency Synthesizer (PLL) */
	sbit ENPLL = 0xC9;
    /** Enables PLL Monitoring during RF transmission */
    sbit ENPLLMON = 0xCF;
/** RF-Encoder Tx Data Register */
sfr RFD      = 0xC9; 
/** RF-Encoder Tx Control Register */
sfr RFENC    = 0xCA; 
/** RF-Encoder Tx Status Register */
sfr RFS      = 0xCB; 
/** RF-Transmitter Configuration Register */
sfr RFTX     = 0xAE; 
/** @} */


/** Random Number Generator Register */
sfr RNGD     = 0xAB;

/**
@addtogroup Wakeup_Event_Registers 
@{ */
/** Wakeup Flag Register */
sfr WUF      = 0xC0;
     /** IntervalTimer Wakeup */
     sbit ITIM_FLAG   = 0xC0; 
     /** External Wakeup (PP2) */
     sbit EXT_FLAG    = 0xC1; 
     /** LF-Rx pattern2-match Wakeup */
     sbit LFPM0_FLAG  = 0xC2; 
     /** LF-Rx pattern1-match Wakeup */
     sbit LFPM1_FLAG  = 0xC3; 
     /** LF-Rx sync-match Wakeup */
     sbit LFSY_FLAG   = 0xC4; 
     /** LF-Rx carrier-detect Wakeup */
     sbit LFCD_FLAG   = 0xC5; 
     /** TMAX Underflow Wakeup */
     sbit TMAX_FLAG   = 0xC6; 
     /** Watchdog Wakeup */
     sbit WDOG_FLAG   = 0xC7; 
/** Wakeup Mask Register */
sfr WUM      = 0xC1; 
/** @} */

/** XTAL Configuration Register (FSKLOW) */
sfr XTAL0    = 0xC4;
/** XTAL Configuration Register (FSKHIGH/ASK) */
sfr XTAL1    = 0xC3;


#endif /* SP37_REGISTERS */
/** @} */