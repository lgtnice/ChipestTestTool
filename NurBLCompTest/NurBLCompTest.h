#pragma once

#include <string>
#include <exception>
#include "NurRFNVTable.h"
#include "NurBaseDUT.h"
#include "NurBaseInstrument.h"
#include "NurCommonFunc.h"
#include <vcclr.h>

using namespace std;

class NurBLTest
{
private:
	CNurInstrumentInterface* m_pInstru =  NULL ;
	CNurDUTInterface* m_pDUT = NULL;
	HINSTANCE m_nurDutDLL = NULL;
	HINSTANCE m_nurInstruDLL = NULL;
	SEND_LOG_TO_UI m_logOutput = NULL;
	TXResult m_txResult;
public:
	NurBLTest();
	~NurBLTest();

	//设置打印log的函数指针
	int SetLogPtr(SEND_LOG_TO_UI logOutputEX);
	//连接UE和仪表的函数
	int InitDUTAPI();
	int ReleaseDUTAPI();
	int InitInstrumentAPI();
	int ReleaseInstrumentAPI();
	int OpenSerialPortEX(int baudRate, const wchar_t* serialPort);
	int CloseSerialPortEX();
	int OpenInstrument(const wchar_t* intrumentIP);
	int ReleaseInstrument();


	//综测函数
	int NSFTConn(NSFTConf* config);
	int NSFTTXMeasurement(NSFTConf* config);
	int NSFTRXMeasurement(NSFTConf* config);
	int NSFTHandover(int band, int channel);
	int CheckTXMeasurementResult(NSFTConf* config, TXResult m_txResult);
	int CheckRXMeasurementResult(NSFTConf* config, RXResult rxResult);
};