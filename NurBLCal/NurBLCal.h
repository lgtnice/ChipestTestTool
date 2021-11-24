#pragma once

#include <string>
#include <exception>
#include "NurRFNVTable.h"
#include "NurBaseDUT.h"
#include "NurBaseInstrument.h"
#include "NurCommonFunc.h"
#include <vcclr.h>

using namespace std;

class NurBLCal
{
private:
	CNurInstrumentInterface* m_pInstru =  NULL ;
	CNurDUTInterface* m_pDUT =  NULL ;
	HINSTANCE m_nurDutDLL = NULL;
	HINSTANCE m_nurInstruDLL =  NULL ;
	SEND_LOG_TO_UI m_logOutput = NULL;

	double m_power1[BAND_NV_NUM][TX_PA_NUM] = { 0 };		// 保存每个PA档位校准时使用的第一个功率点的APC控制字对应的测量功率
	double m_power2[BAND_NV_NUM][TX_PA_NUM] = { 0 };		// 保存每个PA档位校准时使用的第二个功率点的APC控制字对应的测量功率
public:
	NurBLCal();
	~NurBLCal();

	//设置打印log的函数指针
	int SetLogPtr(SEND_LOG_TO_UI logOutputEX);
	//连接UE和仪表的函数
	int InitDUTAPI();
	int ReleaseDUTAPI();
	int InitInstrumentAPI();
	int ReleaseInstrumentAPI();
	int OpenSerialPortEX(int baudRate,const wchar_t* serialPort);
	int CloseSerialPortEX();
	int OpenInstrument(const wchar_t* intrumentIP);
	int ReleaseInstrument();
	int InitReceiver(AGCConf* config);
	int InitTransmitter(APCConf* config);
	int GetRSSI(double compFreqDL, double pathLoss, double cellPower, int& rssi);
	

	//校准函数
	int AFCCal(AFCConf* config);
	int APCCal(APCConf* config);
	int TFCCal(APCConf* config);
	int AGCCal(AGCConf* config);
	int RFCCal(AGCConf* config);


	
};

