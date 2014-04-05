/**
  ******************************************************************************
  * @file    smartrf_cc2500.h
  * @author  Adopted by Matthew Vertescher
  * @version V1.0.0
  * @date    28-March-2014
  * @brief   This file contains all the configuration macros for the CC2500.
  ******************************************************************************
  */ 

#ifndef SMARTRF_CC2500_H 
#define SMARTRF_CC2500_H 
 
#define SMARTRF_RADIO_CC2500 
 
// Frequency calculation: 
//  2433 MHz + 72 KHz 

#define SMARTRF_SETTING_FSCTRL1       0x0C //0x12 //Frequency offset = 457kHz 
#define SMARTRF_SETTING_FSCTRL0       0x48 // 72KHz
#define SMARTRF_SETTING_FREQ2         0x5D        //0x5D // 0x5D // Carrier Frequency is 2.433GHz 
#define SMARTRF_SETTING_FREQ1         0x93        //0x94 // 0x93 // Carrier Frequency is 2,433,072kHz
#define SMARTRF_SETTING_FREQ0         0xB1        //0x66 // 0xB1 
#define SMARTRF_SETTING_MDMCFG4       0x0E //0x2D // BW of channel = 541.666kHz 
#define SMARTRF_SETTING_MDMCFG3       0x3B // Baud Rate = 125kb 
#define SMARTRF_SETTING_MDMCFG2       0x73 // before demodulator, MSK modulation, 16/16 sync word bits detected 
#define SMARTRF_SETTING_MDMCFG1       0x42  /* num_preamble bytes set here */ 
#define SMARTRF_SETTING_MDMCFG0       0xF8 // Default Channel Spacing of 200kHz 
#define SMARTRF_SETTING_CHANNR        0x00 // Channel 0 
#define SMARTRF_SETTING_DEVIATN       0x00 //0x01 // 1785kHz 
#define SMARTRF_SETTING_FREND1        0xB6 
#define SMARTRF_SETTING_FREND0        0x10 
#define SMARTRF_SETTING_MCSM1         0x30 // This is the default, next state for RX and TX is idle       
#define SMARTRF_SETTING_MCSM0         0x18 // Automatically calibrate When going from IDLE to RX or TX (or FSTXON) check CC2500 datasheet 
#define SMARTRF_SETTING_FOCCFG        0x1D // check datasheet 
#define SMARTRF_SETTING_BSCFG         0x1C 
#define SMARTRF_SETTING_AGCCTRL2      0xC7 
#define SMARTRF_SETTING_AGCCTRL1      0x40 //0x00 
#define SMARTRF_SETTING_AGCCTRL0      0xB0 
#define SMARTRF_SETTING_FSCAL3        0xEA 
#define SMARTRF_SETTING_FSCAL2        0x0A 
#define SMARTRF_SETTING_FSCAL1        0x00 
#define SMARTRF_SETTING_FSCAL0        0x19 //0x11 
#define SMARTRF_SETTING_FSTEST        0x59 
#define SMARTRF_SETTING_TEST2         0x88 
#define SMARTRF_SETTING_TEST1         0x31 
#define SMARTRF_SETTING_TEST0         0x0B 
#define SMARTRF_SETTING_FIFOTHR       0x07 
#define SMARTRF_SETTING_IOCFG2        0x29 
#define SMARTRF_SETTING_IOCFG0        0x06 
#define SMARTRF_SETTING_PKTCTRL1      0x04 
#define SMARTRF_SETTING_PKTCTRL0      0x44 //0x05 // Fixed Packet Length (0x05), 0x40 -> WHITE_DATA 
#define SMARTRF_SETTING_ADDR          0x00 // Global Broadcast Address 
#define SMARTRF_SETTING_PKTLEN        0x0A  // Packet Length of 10bytes (0xFF) 
 
#endif /* SMARTRF_CC2500_H  */
