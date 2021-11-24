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
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#ifdef  _WIN32
	#include "Win32\\VISA.H"
#endif
#ifdef  _WIN64
	#include "x64\\VISA.H"
#endif
using namespace std;

#define BUF_LEN 4096

class CNurInstrumentInterface
{
public: 
	CNurInstrumentInterface() {};
	virtual ~CNurInstrumentInterface() = 0 {};

	/**
	* Open instrument via GPIB or LAN
	* @param [out] ViStatus £º0,success£»otherwise,fail
	* @param [in] pInstrumentIP Instrument IP
	*/	
	virtual ViStatus OpenInstru(const char* pInstrumentIP) = 0;

	/**
	* Close instrument and release instrument handle
	* @param [out] ViStatus £º0,success£»otherwise,fail	
	*/	
	virtual ViStatus CloseInstru(void) = 0;

	virtual ViStatus InitInstrument() = 0;

	virtual ViStatus SetMeasurementFreq(double Freq, BOOL bCWSignal) = 0;

	virtual ViStatus SetGeneratorFreq(double Freq) = 0;

	virtual ViStatus SetMeasurementLoss(double loss, BOOL bCWSignal) = 0;

	virtual ViStatus SetGeneratorLoss(double loss) = 0;

	virtual ViStatus SetExpPower(double power, BOOL bCWSignal) = 0;

	virtual ViStatus SetGeneratorPower(double power) = 0;

	virtual ViStatus GetFreqErr(double& freqErr, double& power) = 0;

	virtual ViStatus GetTXPower(double &fTXPower, BOOL bCWSignal) = 0;

	virtual ViStatus TurnOnGenerator() = 0;

	virtual ViStatus TurnOffGenerator() = 0;

	virtual ViStatus InitNSFT() = 0;

	virtual ViStatus NSFTHandover(double FreqDL, double FreqUL) = 0;

	virtual ViStatus NSFTSetMeasurement(unsigned char mcs, unsigned char toneNum, unsigned char toneStart, unsigned char subCarrierSpacing, bool onlyPowerMeas = false) = 0;

	virtual ViStatus NSFTTXMeasurement() = 0;

	virtual ViStatus ReadPower(double& power) = 0;

	virtual ViStatus ReadACLR(double alcr[]) = 0;

	virtual ViStatus ReadMODulation(double& evm, double& freqErr) = 0;

	virtual ViStatus ReadSEMask(double& power, bool& sem, double& obw) = 0;

	virtual ViStatus ReadTXFreq(double &freq) = 0;
};

