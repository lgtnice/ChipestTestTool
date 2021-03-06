/*
Module Name:    NurInstrument.h
Abstract:
    APIs for all instrument operations
Author:
    GTEST
Revision History:
V 1.0   2019.02.20    Jason            Initial version  
--*/
#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;


#define BAND_NUM		18
#define BAND_NV_NUM		13
#define TFC_FREQ_NUM	16
#define RFC_FREQ_NUM	16

#define TX_PA_NUM		3
#define RX_GAIN_NUM		3

#define APC_HIGH_START	28
#define APC_HIGH_END	-10
#define APC_HIGH_STEP	0.25
#define APC_HIGH_NUM	(int)((APC_HIGH_START - APC_HIGH_END) / APC_HIGH_STEP + 1)
#define APC_MID_START	0
#define APC_MID_END		-40
#define APC_MID_STEP	0.5
#define APC_MID_NUM		(int)((APC_MID_START - APC_MID_END) / APC_MID_STEP + 1)
#define APC_LOW_START	-30
#define APC_LOW_END		-60
#define APC_LOW_STEP	1
#define APC_LOW_NUM		(int)((APC_LOW_START - APC_LOW_END) / APC_LOW_STEP + 1)

#define NB_BC1_ULCHANNEL_MIN 18000
#define NB_BC1_ULCHANNEL_MAX 18599
#define NB_BC1_DLCHANNEL_MIN 0
#define NB_BC1_DLCHANNEL_MAX 599


#define NB_BC2_ULCHANNEL_MIN 18600
#define NB_BC2_ULCHANNEL_MAX 19199
#define NB_BC2_DLCHANNEL_MIN 600
#define NB_BC2_DLCHANNEL_MAX 1199

#define NB_BC3_ULCHANNEL_MIN 19200
#define NB_BC3_ULCHANNEL_MAX 19949
#define NB_BC3_DLCHANNEL_MIN 1200
#define NB_BC3_DLCHANNEL_MAX 1949

#define NB_BC5_ULCHANNEL_MIN 20400
#define NB_BC5_ULCHANNEL_MAX 20649
#define NB_BC5_DLCHANNEL_MIN 2400
#define NB_BC5_DLCHANNEL_MAX 2649

#define NB_BC8_ULCHANNEL_MIN 21450
#define NB_BC8_ULCHANNEL_MAX 21799
#define NB_BC8_DLCHANNEL_MIN 3450
#define NB_BC8_DLCHANNEL_MAX 3799

#define NB_BC11_ULCHANNEL_MIN 22750
#define NB_BC11_ULCHANNEL_MAX 22949
#define NB_BC11_DLCHANNEL_MIN 4750
#define NB_BC11_DLCHANNEL_MAX 4949

#define NB_BC12_ULCHANNEL_MIN 23010
#define NB_BC12_ULCHANNEL_MAX 23179
#define NB_BC12_DLCHANNEL_MIN 5010
#define NB_BC12_DLCHANNEL_MAX 5179

#define NB_BC13_ULCHANNEL_MIN 23180
#define NB_BC13_ULCHANNEL_MAX 23279
#define NB_BC13_DLCHANNEL_MIN 5180
#define NB_BC13_DLCHANNEL_MAX 5279

#define NB_BC17_ULCHANNEL_MIN 23730
#define NB_BC17_ULCHANNEL_MAX 23849
#define NB_BC17_DLCHANNEL_MIN 5730
#define NB_BC17_DLCHANNEL_MAX 5849

#define NB_BC18_ULCHANNEL_MIN 23850
#define NB_BC18_ULCHANNEL_MAX 23999
#define NB_BC18_DLCHANNEL_MIN 5850
#define NB_BC18_DLCHANNEL_MAX 5999

#define NB_BC19_ULCHANNEL_MIN 24000
#define NB_BC19_ULCHANNEL_MAX 24149
#define NB_BC19_DLCHANNEL_MIN 6000
#define NB_BC19_DLCHANNEL_MAX 6149

#define NB_BC20_ULCHANNEL_MIN 24150
#define NB_BC20_ULCHANNEL_MAX 24449
#define NB_BC20_DLCHANNEL_MIN 6150
#define NB_BC20_DLCHANNEL_MAX 6449

#define NB_BC25_ULCHANNEL_MIN 26040
#define NB_BC25_ULCHANNEL_MAX 26689
#define NB_BC25_DLCHANNEL_MIN 8040
#define NB_BC25_DLCHANNEL_MAX 8689

#define NB_BC26_ULCHANNEL_MIN 26690
#define NB_BC26_ULCHANNEL_MAX 27039
#define NB_BC26_DLCHANNEL_MIN 8690
#define NB_BC26_DLCHANNEL_MAX 9039

#define NB_BC28_ULCHANNEL_MIN 27210
#define NB_BC28_ULCHANNEL_MAX 27659
#define NB_BC28_DLCHANNEL_MIN 9210
#define NB_BC28_DLCHANNEL_MAX 9659

#define NB_BC31_ULCHANNEL_MIN 27760
#define NB_BC31_ULCHANNEL_MAX 27809
#define NB_BC31_DLCHANNEL_MIN 9870
#define NB_BC31_DLCHANNEL_MAX 9919

#define NB_BC66_ULCHANNEL_MIN 66436
#define NB_BC66_ULCHANNEL_MAX 67135
#define NB_BC66_DLCHANNEL_MIN 67136 //66BandChannel 67136~67335 NO Uplink
#define NB_BC66_DLCHANNEL_MAX 67335

#define NB_BC70_ULCHANNEL_MIN 68336
#define NB_BC70_ULCHANNEL_MAX 68485
#define NB_BC70_DLCHANNEL_MIN 68486
#define NB_BC70_DLCHANNEL_MAX 68585


#define NB_BC1_ULFREQ_MIN 1920
#define NB_BC1_DLFREQ_MIN 2110
#define NB_BC1_ULFREQ_MAX 1980
#define NB_BC1_DLFREQ_MAX 2170

#define NB_BC2_ULFREQ_MIN 1850
#define NB_BC2_DLFREQ_MIN 1930
#define NB_BC2_ULFREQ_MAX 1910
#define NB_BC2_DLFREQ_MAX 1980

#define NB_BC3_ULFREQ_MIN 1710
#define NB_BC3_DLFREQ_MIN 1805
#define NB_BC3_ULFREQ_MAX 1785
#define NB_BC3_DLFREQ_MAX 1880

#define NB_BC5_ULFREQ_MIN 824
#define NB_BC5_DLFREQ_MIN 869
#define NB_BC5_ULFREQ_MAX 849
#define NB_BC5_DLFREQ_MAX 894

#define NB_BC8_ULFREQ_MIN 880
#define NB_BC8_DLFREQ_MIN 925
#define NB_BC8_ULFREQ_MAX 915
#define NB_BC8_DLFREQ_MAX 960

#define NB_BC11_ULFREQ_MIN 1427.9
#define NB_BC11_DLFREQ_MIN 1475.9
#define NB_BC11_ULFREQ_MAX 1447.9
#define NB_BC11_DLFREQ_MAX 1495.9

#define NB_BC12_ULFREQ_MIN 699
#define NB_BC12_DLFREQ_MIN 729
#define NB_BC12_ULFREQ_MAX 716
#define NB_BC12_DLFREQ_MAX 746

#define NB_BC13_ULFREQ_MIN 777
#define NB_BC13_DLFREQ_MIN 746
#define NB_BC13_ULFREQ_MAX 787
#define NB_BC13_DLFREQ_MAX 756

#define NB_BC17_ULFREQ_MIN 704
#define NB_BC17_DLFREQ_MIN 734
#define NB_BC17_ULFREQ_MAX 716
#define NB_BC17_DLFREQ_MAX 746

#define NB_BC18_ULFREQ_MIN 815
#define NB_BC18_DLFREQ_MIN 875
#define NB_BC18_ULFREQ_MAX 830
#define NB_BC18_DLFREQ_MAX 890

#define NB_BC19_ULFREQ_MIN 830
#define NB_BC19_DLFREQ_MIN 875
#define NB_BC19_ULFREQ_MAX 845
#define NB_BC19_DLFREQ_MAX 890

#define NB_BC20_ULFREQ_MIN 832
#define NB_BC20_DLFREQ_MIN 791
#define NB_BC20_ULFREQ_MAX 862
#define NB_BC20_DLFREQ_MAX 821

#define NB_BC25_ULFREQ_MIN 1850
#define NB_BC25_DLFREQ_MIN 1930
#define NB_BC25_ULFREQ_MAX 1915
#define NB_BC25_DLFREQ_MAX 1995

#define NB_BC26_ULFREQ_MIN 814
#define NB_BC26_DLFREQ_MIN 859
#define NB_BC26_ULFREQ_MAX 849
#define NB_BC26_DLFREQ_MAX 894

#define NB_BC28_ULFREQ_MIN 703
#define NB_BC28_DLFREQ_MIN 758
#define NB_BC28_ULFREQ_MAX 748
#define NB_BC28_DLFREQ_MAX 803

#define NB_BC31_ULFREQ_MIN 452.5
#define NB_BC31_DLFREQ_MIN 462.5
#define NB_BC31_ULFREQ_MAX 457.5
#define NB_BC31_DLFREQ_MAX 467.5

#define NB_BC66_ULFREQ_MIN 1710
#define NB_BC66_DLFREQ_MIN 2110
#define NB_BC66_ULFREQ_MAX 1780
#define NB_BC66_DLFREQ_MAX 2200

#define NB_BC70_ULFREQ_MIN 1695
#define NB_BC70_DLFREQ_MIN 1995
#define NB_BC70_ULFREQ_MAX 1710
#define NB_BC70_DLFREQ_MAX 2020

typedef enum
{
	TX_PA_HIGH	= 0,
	TX_PA_MID	= 1,
	TX_PA_LOW	= 2
} TX_PA;

typedef enum
{
	RX_GAIN_LOW		= 0,
	RX_GAIN_MID		= 1,
	RX_GAIN_HIGH	= 2
} RX_GAIN;

/*
typedef enum
{
	GAIN_LOW = 0,
	GAIN_MID = 1,
	GAIN_HIGH = 2
} RX_GAIN;
*/
typedef enum
{
	BAND1		= 1,
	BAND2		= 2,
	BAND3		= 3,
	BAND5		= 5,
	BAND8		= 8,
	BAND11		= 11,
	BAND12		= 12,
	BAND13		= 13,
	BAND17		= 17,
	BAND18		= 18,
	BAND19		= 19,
	BAND20		= 20,
	BAND25		= 25,
	BAND26		= 26,
	BAND28		= 28,
	BAND31		= 31,
	BAND66		= 66,
	BAND70		= 70
} NurBand;

typedef enum
{
	BAND1_INDEX = 0,
	BAND2_INDEX = 1,
	BAND3_INDEX = 2,
	BAND5_INDEX = 3,
	BAND8_INDEX = 4,
	BAND11_INDEX = 5,
	BAND12_INDEX = 6,
	BAND13_INDEX = 7,
	BAND17_INDEX = 8,
	BAND18_INDEX = 9,
	BAND19_INDEX = 10,
	BAND20_INDEX = 11,
	BAND25_INDEX = 12,
	BAND26_INDEX = 13,
	BAND28_INDEX = 14,
	BAND31_INDEX = 15,
	BAND66_INDEX = 16,
	BAND70_INDEX = 17
} NurBandIndex;

typedef enum
{
	BAND1_NV_INDEX = 0,
	BAND3_NV_INDEX = 1,
	BAND8_NV_INDEX = 2,
	BAND11_NV_INDEX = 3,
	BAND12_NV_INDEX = 4,
	BAND13_NV_INDEX = 5,
	BAND20_NV_INDEX = 6,
	BAND25_NV_INDEX = 7,
	BAND26_NV_INDEX = 8,
	BAND28_NV_INDEX = 9,
	BAND31_NV_INDEX = 10,
	BAND66_NV_INDEX = 11,
	BAND70_NV_INDEX = 12
} NurBandNVIndex;

typedef enum
{
	SCSP_15K	= 0,
	SCSP_3K75	= 1
} NurSubCarrierSpacing;

typedef enum
{	
	BPSK = 2,
	QPSK = 4,
	QAM8 = 8,
	QAM16 = 16,
	QAM32 = 32
} NurMcs;

typedef struct
{
	unsigned short		afcInit;					// ??????????????
	short				afcSlope;					// ??????????
} NurRFNVCalAfc;

typedef struct
{
	unsigned short		apcHigh[APC_HIGH_NUM];		//APC????????????
	unsigned short		apcMid[APC_MID_NUM];		//APC????????????
	unsigned short		apcLow[APC_LOW_NUM];		//APC????????????
} NurRFNVCalApc;

typedef struct
{
	short				frequency;					// ??????????????100kHz
	signed char			comp[TX_PA_NUM];			// ????PA??????????
} NurRFNVCalTfcInfo;

typedef struct
{
	NurRFNVCalTfcInfo	info[TFC_FREQ_NUM];			// ????????????????????
} NurRFNVCalTfc;

typedef struct
{
	//????????????????????????????????????????0????????????????????????????
	unsigned char		agcGain[RX_GAIN_NUM];		//????????????
} NurRFNVCalAgc;

typedef struct
{
	short				frequency;					// ??????????????100kHz
	char				comp;						// ????????????????
} NurRFNVCalRfcInfo;

typedef struct
{
	NurRFNVCalRfcInfo	info[RFC_FREQ_NUM];			// ????????????????????
} NurRFNVCalRfc;

typedef struct
{
	NurRFNVCalApc		apc;						//APC????
	NurRFNVCalTfc		tfc;						//TFC????
	NurRFNVCalAgc		agc;						//AGC????
	NurRFNVCalRfc		rfc;						//RFC????
} NurRFNVCalInfo;

typedef struct
{
	NurRFNVCalAfc		afc;						//AFC????
	NurRFNVCalInfo		info[BAND_NV_NUM];			//BAND_NUM??band??nv????
} NurRFNVCal;