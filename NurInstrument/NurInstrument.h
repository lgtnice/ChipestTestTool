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
#include "visa.h"
#include "NurBaseInstrument.h"
#include "NurCommonFunc.h"
using namespace std;

#define BUF_LEN 4096

class CNurCMW500 : public CNurInstrumentInterface
{
public: 
	CNurCMW500(void);
	~CNurCMW500(void);

	/**
	* Open instrument via GPIB or LAN
	* @param [out] ViStatus £º0,success£»otherwise,fail
	* @param [in] pInstrumentIP Instrument IP
	*/	
	ViStatus OpenInstru(const char* pInstrumentIP);

	/**
	* Close instrument and release instrument handle
	* @param [out] ViStatus £º0,success£»otherwise,fail	
	*/	
	ViStatus CloseInstru();

	ViStatus InitInstrument();

	ViStatus SetMeasurementFreq(double Freq, BOOL bCWSignal);

	ViStatus SetGeneratorFreq(double Freq);

	ViStatus SetMeasurementLoss(double loss, BOOL bCWSignal);

	ViStatus SetGeneratorLoss(double loss);

	ViStatus SetExpPower(double power, BOOL bCWSignal);

	ViStatus SetGeneratorPower(double power);

	ViStatus GetFreqErr(double& freqErr, double& power);
	
	ViStatus GetTXPower(double &fTXPower, BOOL bCWSignal);

	ViStatus TurnOnGenerator();

	ViStatus TurnOffGenerator();

	ViStatus InitNSFT();

	ViStatus NSFTHandover(double FreqDL, double FreqUL);

	ViStatus NSFTSetMeasurement(unsigned char mcs, unsigned char toneNum, unsigned char toneStart, unsigned char subCarrierSpacing, bool onlyPowerMeas = false);

	ViStatus NSFTTXMeasurement();

	ViStatus ReadPower(double& power);

	ViStatus ReadACLR(double alcr[]);

	ViStatus ReadMODulation(double& evm, double& freqErr);

	ViStatus ReadSEMask(double& power, bool& sem, double& obw);

	ViStatus ReadTXFreq(double &freq);

private:
	void StringToDouble(double *doubleArray, char *stringData);

	ViSession m_viDefaultRM;
	ViSession m_viInstruHandle;
	ViStatus m_viStatus;
	ViChar m_szCmdBuf[BUF_LEN];
	ViChar m_szRetValue[BUF_LEN];
};