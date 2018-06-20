//File build with nextGen V2.37, File Parser V1.0.
//File generated by: Deathgo
//Date: 2017-02-27 22:14:4
//Output generated by B12  Explorer (build B12.6.51) for product TDA5235
// $LastChangedDate$
// $Rev$
// $Author$
// SPI_CONFIGURATION_METAINFO
//<please enter a setting description>
// SPI_CONFIGURATION_SETTINGS
//Wizard_4_Digital_Receiving_UnitDATARATEA:9600
//Wizard_4_Digital_Receiving_UnitFDEVA:45
//Wizard_4_Digital_Receiving_UnitFDEVMINA:15
//Wizard_4_Digital_Receiving_UnitCHIPSPERBITA:1
//Wizard_5_RF_IF_FrontendAUTOCONFA:1
//Wizard_4_Digital_Receiving_UnitAUTOCONFA:1
//Wizard_4_Digital_Receiving_UnitAUTOCONF_SLICERA:1
//Wizard_4_Digital_Receiving_UnitSTIMEA:15
//Wizard_6_Clock_Data_RecoveryCVA:1
//XTAL:21948717
//Wizard_4_Digital_Receiving_UnitOVSPLA:1
//Wizard_7_Frame_Synchronization_UnitPROTOCOL_NAMEA:'IFX Reference Pattern 2'
//Wizard_7_Frame_Synchronization_UnitTSILENA6A:16
//Wizard_7_Frame_Synchronization_UnitDLLIMIT1A:112
//Wizard_7_Frame_Synchronization_UnitPATTERN16A:'0101011001100110'
//SAVEMODE:'Save Active[A]'
//------------END------------

#ifndef _INITDATA_H_
#define _INITDATA_H_


#include<TDA5235_SFR.h>

#define DEFINEGLOBALSHERE		//����TDA5235_INIT.h��ĳ�ʼ������

#ifdef DEFINEGLOBALSHERE
const unsigned char init_length = 30;
const unsigned char init_data[30][2] = {
	{SFR_CMC0, 0x00}, 
	{SFR_SFRPAGE, 0x00}, 
	{SFR_A_SIGDETSAT, 0x1C}, 
	{SFR_A_SYSRCTO, 0x7E}, 
	{SFR_A_TOTIM_SYNC, 0x01}, 
	{SFR_A_AFCK1CFG0, 0x50}, 
	{SFR_A_AFCK2CFG0, 0x50}, 
	{SFR_A_PMFUDSF, 0x22}, 
	{SFR_A_PDECF, 0x02}, 
	{SFR_A_PDECSCFSK, 0x0E}, 
	{SFR_A_PDECSCASK, 0x2A}, 
	{SFR_A_MFC, 0x09}, 
	{SFR_A_SRC, 0x31}, 
	{SFR_A_EXTSLC, 0x04}, 
	{SFR_A_SIGDET0, 0x0C}, 
	{SFR_A_SIGDET1, 0x05}, 
	{SFR_A_SIGDETLO, 0x71}, 
	{SFR_A_SIGDETSEL, 0x7D}, 
	{SFR_A_NDTHRES, 0x08}, 
	{SFR_A_NDCONFIG, 0x37}, 
	{SFR_A_CDRRI, 0x02}, 
	{SFR_A_SLCCFG, 0x75}, 
	{SFR_A_TSILENA, 0x10}, 
	{SFR_A_TSIPTA0, 0x66}, 
	{SFR_A_TSIPTA1, 0x56}, 
	{SFR_A_EOMDLEN, 0x70}, 
	{SFR_A_CHCFG, 0x15}, 
	{SFR_A_PLLFRAC0C1, 0xF2}, 
	{SFR_IM0, 0x02}, 
	{SFR_CMC0, 0x02}
	};
#else
extern const unsigned char init_length;
extern const unsigned char init_data[30][2];
#endif

#endif
