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

	double m_power1[BAND_NV_NUM][TX_PA_NUM] = { 0 };		// ����ÿ��PA��λУ׼ʱʹ�õĵ�һ�����ʵ��APC�����ֶ�Ӧ�Ĳ�������
	double m_power2[BAND_NV_NUM][TX_PA_NUM] = { 0 };		// ����ÿ��PA��λУ׼ʱʹ�õĵڶ������ʵ��APC�����ֶ�Ӧ�Ĳ�������
public:
	NurBLCal();
	~NurBLCal();

	//���ô�ӡlog�ĺ���ָ��
	int SetLogPtr(SEND_LOG_TO_UI logOutputEX);
	//����UE���Ǳ�ĺ���
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
	

	//У׼����
	int AFCCal(AFCConf* config);
	int APCCal(APCConf* config);
	int TFCCal(APCConf* config);
	int AGCCal(AGCConf* config);
	int RFCCal(AGCConf* config);


	
};

