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
#include "windows.h"
#include "NurRFNVTable.h"
using namespace std;

class CNurDUTInterface
{
public: 
	CNurDUTInterface() {};
	virtual ~CNurDUTInterface() = 0 {};

	virtual bool SetDUTMode(unsigned char mode) = 0;
	virtual bool OpenSerialPort(DWORD BaudRate, LPCTSTR portName) = 0;
	virtual bool CloseSerialPort() = 0;
	virtual bool WriteAFCCodeTable(NurRFNVCalAfc codeTable) = 0;
	virtual bool WriteBandCodeTable(unsigned char band, NurRFNVCalInfo codeTable) = 0;
	virtual bool WriteAllCodeTable(NurRFNVCal codeTable) = 0;
	virtual bool WriteTagBit(unsigned char flagID, unsigned char flag) = 0;
	virtual bool UpdateCodeTableToRAM() = 0;
	virtual bool ReadAFCCodeTable(NurRFNVCalAfc& codeTable) = 0;
	virtual bool ReadSpecialBandCodeTable(unsigned char& band, NurRFNVCalInfo& codeTable) = 0;
	virtual bool ReadAllCodeTable(NurRFNVCal& codeTable) = 0;
	virtual bool ReadTagBit(unsigned char& flagID, unsigned char& flag) = 0;

	virtual bool OpenTransmitter(unsigned short freq, unsigned char pa, unsigned short vga) = 0;
	virtual bool CloseTransmitter() = 0;
	virtual bool SetTransmitterFrequency(unsigned short freq) = 0;
	virtual bool SetTransmitterPower(unsigned char pa, unsigned short vga) = 0;
	virtual bool SetTransmitterFrequencyOffset(unsigned short afc) = 0;
	virtual bool OpenReceiver(unsigned short freq, unsigned char gain) = 0;
	virtual bool CloseReceiver() = 0;
	virtual bool SetReceiverFrequency(unsigned short freq) = 0;
	virtual bool SetReceiverGain(unsigned short gain) = 0;
	virtual bool GetReceiverSignalStrength(unsigned short& rxpower) = 0;

	virtual bool EnterNSTMode() = 0;
	virtual bool ExitNSTMode() = 0;
	virtual bool NSTDownlinkSync(unsigned int channel, unsigned char mcs, unsigned char toneNum, unsigned char toneStart, unsigned char subCarrierSpacing, double power) = 0;
	virtual bool NSTSetUplinkPower(double power) = 0;
	virtual bool NSTNoticeUEPrepareSwitchFrequency() = 0;
	virtual bool NSTSwitchChannel(unsigned char band, unsigned int channel) = 0;
	virtual bool NSTSetBLERArg(unsigned short subFrameNumber, unsigned short time) = 0;
	virtual bool InquireConnectStateUEInstr(unsigned char& status) = 0;
	virtual bool GetBLERValue(unsigned short& totalBlock, unsigned short& errorBlock) = 0;
	virtual bool GetRSRPValue(unsigned short& rsrp) = 0;

	virtual bool OpenGNSS(unsigned char tech, char band, unsigned short bandwidth, unsigned short time) = 0;
	virtual bool CloseGNSS() = 0;
	virtual bool GetSNR(unsigned short& rsrp) = 0;
};

