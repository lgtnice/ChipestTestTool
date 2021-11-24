#pragma once
#include "stdafx.h"
#include "windows.h"
#include <string>
#include "NurRFNVTable.h"
#include "NurBaseDUT.h"
#include "NurBaseInstrument.h"

using namespace std;

#define THREAD_NUM		2
#define LOG_BUFF_LEN	1024

// ���ڷ������۲�ĺ�
#define CHANNEL_NUM		3		// ÿ���۲�Ƶ����Ҫ���Ե�Ƶ��
#define TXTESTCASE_NUM	7		// �����۲���Ŀ����
#define RXTESTCASE_NUM	3		// �����۲���Ŀ����

typedef enum
{
	PASS = 0,
	FAIL = 1
} NurResult;

typedef enum
{
	LOG_SUCCESS = 0,
	LOG_ERROR = 1,
	LOG_WARNING = 2,
	LOG_DEBUG = 3,
	LOG_DATE = 4
} LOG_CONTENT;

typedef enum
{
	MAXPOWER = 0,
	MINPOWER = 1,
	FREQERR = 2,
	EVM = 3,
	ACLR = 4,
	SEM = 5,
	OBW = 6
} NurTXTestCase;

typedef enum
{
	MAXINPUT = 0,
	RSRP = 1,
	BLER = 2
} NurRXTestCase;

typedef CNurInstrumentInterface * (*INSTRUFUNC)();
typedef CNurDUTInterface * (*DUTFUNC)();


typedef void(__stdcall    * SEND_LOG_TO_UI)(char*);//log���ݣ��̺߳� 0/1����ɫ���
typedef struct 
{
	int apcData;				// ����UE��������ʿ����֣�����UE�������С����configҳ���ж�ȡ
	int paMode;		// ����UE�����PA��λ������UE�������С����configҳ���ж�ȡ
	int afcData1;			// ����UE�ĵ�һ��AFC�����֣�У׼ʱΪ�˼�������п����ֵ�ƫ��ֵ��б�ʣ���configҳ���ж�ȡ
	int afcData2;			// ����UE�ĵڶ���AFC�����֣�У׼ʱΪ�˼�������п����ֵ�ƫ��ֵ��б�ʣ���configҳ���ж�ȡ
	int cycleNum;				// AFC΢��ѭ����������configҳ���ж�ȡ
	double freq;			// ��λMHz��Ƶ��У׼������Ƶ�㣬��configҳ��������			
	double expPower;			// ��λdBm�������������ʣ���configҳ��������
	double pathLoss;			// ���𣬴�configҳ���ж�ȡ
	bool bCWSinaling;		// ��־λ��true��ʾ���з���CW����false��ʾ���з�����Ʋ�������configҳ���ж�ȡ
}AFCConf;
typedef struct 
{
	bool bandCalEnable[BAND_NUM];				// ��Ҫtx powerУ׼��band��true��ʾ��ҪУ׼��false��ʾ����ҪУ׼����configҳ���ȡ
	int centerChannel[BAND_NUM];					// ÿ��У׼Ƶ�ε��м��ŵ�����Ҫ��configҳ���ȡ
	int compChannelHigh[BAND_NUM];					// ÿ��У׼Ƶ�εĸ߲����ŵ�����Ҫ��configҳ���ȡ
	int compChannelLow[BAND_NUM];					// ÿ��У׼Ƶ�εĵͲ����ŵ�����Ҫ��configҳ���ȡ
	int apcData1[TX_PA_NUM];						// ÿ��PA��λУ׼ʱʹ�õĵ�һ�����ʵ��APC�����֣���Ҫ��configҳ���ȡ
	int apcData2[TX_PA_NUM];						// ÿ��PA��λУ׼ʱʹ�õĵڶ������ʵ��APC�����֣���Ҫ��configҳ���ȡ

	int initApcData;										// ����UE��ʼ����������ʿ����֣�����UE�������С����configҳ���ж�ȡ
	int paMode;								// ����UE��ʼ�������PA��λ������UE�������С����configҳ���ж�ȡ
	double freq;									// ��λMHz������UE��ʼ�������Ƶ�ʣ���configҳ���ж�ȡ
	double centerFreqPathLossUL[BAND_NUM];			// ÿ����У׼Ƶ������Ƶ���Ӧ���������𣬣���configҳ���ж�ȡ
	double compFreqPathLossHighUL[BAND_NUM];		// ÿ����У׼Ƶ�θ�Ƶ���Ӧ���������𣬣���configҳ���ж�ȡ
	double compFreqPathLossLowUL[BAND_NUM];			// ÿ����У׼Ƶ�ε�Ƶ���Ӧ���������𣬣���configҳ���ж�ȡ
	bool bCWSinaling;								// ��־λ��true��ʾ���з���CW����false��ʾ���з�����Ʋ�������configҳ���ж�ȡ

}APCConf;
typedef struct 
{
	bool bandCalEnable[BAND_NUM];				// ��Ҫtx powerУ׼��band��true��ʾ��ҪУ׼��false��ʾ����ҪУ׼����configҳ���ȡ
	int centerChannel[BAND_NUM];					// ÿ��У׼Ƶ�ε��м��ŵ�����Ҫ��configҳ���ȡ
	int compChannelHigh[BAND_NUM];					// ÿ��У׼Ƶ�εĸ߲����ŵ�����Ҫ��configҳ���ȡ
	int compChannelLow[BAND_NUM];					// ÿ��У׼Ƶ�εĵͲ����ŵ�����Ҫ��configҳ���ȡ
	double agcPower[RX_GAIN_NUM];					// ��ͬ���ջ�����״̬�����õ��Ǳ����й��ʣ���Ҫ��configҳ���ȡ

	double freq;									// ��λMHz������UE��ʼ�����ջ�Ƶ�ʣ���configҳ���ж�ȡ
	double rxGainMode;								// ��ʼ�����ջ�ʹ�õĽ��ջ����浵λ����configҳ���ж�ȡ
	double cellPower;								// ��λdBm�����ڳ�ʼ�����ջ�ʱ�Ǳ�����й��ʣ���configҳ���ж�ȡ
	double centerFreqPathLossDL[BAND_NUM];			// ÿ����У׼Ƶ������Ƶ���Ӧ���������𣬴�configҳ���ж�ȡ
	double compFreqPathLossHighDL[BAND_NUM];		// ÿ����У׼Ƶ�θ�Ƶ���Ӧ���������𣬴�configҳ���ж�ȡ
	double compFreqPathLossLowDL[BAND_NUM];			// ÿ����У׼Ƶ�ε�Ƶ���Ӧ���������𣬴�configҳ���ж�ȡ

}AGCConf;

typedef struct
{
	bool bandNSFTEnable[BAND_NUM];						// ��Ҫ�۲��band��true��ʾ��Ҫ���ԣ�false��ʾ����Ҫ���ԣ���configҳ���ȡ
	int channel[BAND_NUM][CHANNEL_NUM];					// ÿ���۲�Ƶ�ε��ŵ�����Ҫ��configҳ���ȡ
	bool TXTestCaseEnable[TXTESTCASE_NUM];				// ���в�����ʹ�ܣ�Ŀǰ��7�0������ʣ�1����С���ʣ�2��Ƶ����3��EVM��4��ACLR��5��SEM��6��OBW
	bool RXTestCaseEnable[RXTESTCASE_NUM];				// ���в�����ʹ�ܣ�Ŀǰ��3�0����������ƽ��1��RSRP��2��BLER
	double TXMaxPower;									// ���������
	int TXMaxPowerLimit;								// �������������
	int TXMinPowerLimit;								// ������С��������
	double FreqErrLimit;								// Ƶ��������ޣ���λΪppm��һ������Ϊ1ppm
	double EVMLimit;									// EVM��������
	double OBWLimit;									// OBW��������
	double ACLRLimit[2];								// ACLR��������
	double MaxInputLimit;								// ��������ƽ����
	double RSRPLimit[RX_GAIN_NUM];						// RSRP����
	double BLERLimit;									// BLER���ޣ���λ%
	double RXMaxPower;									// ���С�����ʣ����ڲ�����������ƽ
	double RXBlerPower;									// ���ڲ���BLER��С������
	double RXRSCPPower[RX_GAIN_NUM];					// ����RSRP����ÿ�ε��Ǳ�С������
	int RXRSCPLimit[RX_GAIN_NUM];						// ����RSRP��������

	double freq;										// ��λMHz������UE��ʼ�����ջ�Ƶ�ʣ���configҳ���ж�ȡ
	double cellPower;									// ��λdBm�����ڳ�ʼ�����ջ�ʱ�Ǳ�����й��ʣ���configҳ���ж�ȡ
	unsigned char mcs;									// ���ƽ���
	unsigned char toneNum;								// tone ��
	unsigned char toneStart;							// tone��ʼλ��
	unsigned char subCarrierSpacing;					// ���ز������0��ʾ15kHz��1��ʾ3.75kHz
	int subFrameNumber;									// ��֡��������BLER����
	int timeout;										// BLER���Գ�ʱʱ�䣬��λΪms


	double centerFreqPathLossDL[BAND_NUM][CHANNEL_NUM];	// ÿ���۲�Ƶ���������𣬴�configҳ���ж�ȡ
	double centerFreqPathLossUL[BAND_NUM][CHANNEL_NUM];	// ÿ���۲�Ƶ�����������𣬴�configҳ���ж�ȡ
}NSFTConf;

typedef struct
{
	double maxPower;
	double minPower;
	double evm;
	double freqErr;
	double obw;
	bool sem;
	double alcr[4];
}TXResult;

typedef struct
{
	double maxInput;
	double rsrp[RX_GAIN_NUM];
	int totalBlock;
	int errorBlock;
}RXResult;

class NurCommonFunc
{
public:
	static string GetProgramDir(void)
	{
		char szapipath[MAX_PATH];
		memset(szapipath, 0, MAX_PATH);
		GetModuleFileNameA(NULL, szapipath, MAX_PATH);

		string strPath = (string)szapipath;
		int pos = strPath.find_last_of('\\', strPath.length());
		return strPath.substr(0, pos);
	}
	static wstring StringToWString(string str)
	{
		int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t *wide = new wchar_t[num];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
		std::wstring w_str(wide);
		delete[] wide;
		return w_str;
	}
	static LPCWSTR stringToLPCWSTR(string orig)
	{
		wchar_t *wcstring = 0;
		try
		{
			size_t origsize = orig.length() + 1;
			const size_t newsize = 100;
			size_t convertedChars = 0;
			if (orig == "")
			{
				wcstring = (wchar_t *)malloc(0);
				mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
			}
			else
			{
				wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
				mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
			}
		}
		catch (exception e)
		{
		}
		return wcstring;
	}
	//�ϲ��ֽ�����
	static bool CombineByte(BYTE* result, BYTE* a, BYTE* b, BYTE* c, int lengthA, int lengthB, int lengthC)
	{
		memcpy(result, a, lengthA);
		memcpy(result + lengthA, b, lengthB);
		memcpy(result + lengthA + lengthB, c, lengthC);
		return true;
	}



	//-----------------------------------------------------------------------------------------------------------//
	static int GetFreqFromDLChan(int downlinkChannel, double &freqDL, double &freqUL)
	{
		int ret = PASS;
		if ((downlinkChannel >= NB_BC1_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC1_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC1_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC1_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC1_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC1_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC2_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC2_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC2_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC2_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC2_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC2_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC3_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC3_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC3_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC3_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC3_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC3_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC5_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC5_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC5_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC5_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC5_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC5_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC8_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC8_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC8_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC8_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC8_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC8_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC11_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC11_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC11_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC11_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC11_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC11_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC12_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC12_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC12_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC12_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC12_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC12_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC13_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC13_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC13_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC13_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC13_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC13_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC17_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC17_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC17_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC17_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC17_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC17_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC18_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC18_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC18_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC18_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC18_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC18_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC19_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC19_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC19_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC19_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC19_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC19_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC20_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC20_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC20_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC20_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC20_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC20_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC25_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC25_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC25_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC25_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC25_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC25_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC26_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC26_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC26_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC26_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC26_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC26_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC28_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC28_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC28_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC28_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC28_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC28_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC31_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC31_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC31_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC31_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC31_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC31_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC66_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC66_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC66_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC66_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC66_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC66_DLCHANNEL_MIN)*0.1;
		}
		else if ((downlinkChannel >= NB_BC70_DLCHANNEL_MIN) && (downlinkChannel <= NB_BC70_DLCHANNEL_MAX))
		{
			freqDL = (double)(NB_BC70_DLFREQ_MIN)+(double)(downlinkChannel - NB_BC70_DLCHANNEL_MIN)*0.1;
			freqUL = (double)(NB_BC70_ULFREQ_MIN)+(double)(downlinkChannel - NB_BC70_DLCHANNEL_MIN)*0.1;
		}
		else
		{
			ret = FAIL;
		}

		return ret;
	}
	static int GetBandIndexFromBand(int band, int &index)
	{
		int ret = PASS;
		switch (band)
		{
		case BAND1:
			index = BAND1_INDEX;
			break;
		case BAND2:
			index = BAND2_INDEX;
			break;
		case BAND3:
			index = BAND3_INDEX;
			break;
		case BAND5:
			index = BAND5_INDEX;
			break;
		case BAND8:
			index = BAND8_INDEX;
			break;
		case BAND11:
			index = BAND11_INDEX;
			break;
		case BAND12:
			index = BAND12_INDEX;
			break;
		case BAND13:
			index = BAND13_INDEX;
			break;
		case BAND17:
			index = BAND17_INDEX;
			break;
		case BAND18:
			index = BAND18_INDEX;
			break;
		case BAND19:
			index = BAND19_INDEX;
			break;
		case BAND20:
			index = BAND20_INDEX;
			break;
		case BAND25:
			index = BAND25_INDEX;
			break;
		case BAND26:
			index = BAND26_INDEX;
			break;
		case BAND28:
			index = BAND28_INDEX;
			break;
		case BAND31:
			index = BAND31_INDEX;
			break;
		case BAND66:
			index = BAND66_INDEX;
			break;
		case BAND70:
			index = BAND70_INDEX;
			break;
		default:
			index = BAND1_INDEX;
			ret = FAIL;
			break;
		}
		return ret;
	}
	static int GetNVIndexFromBandIndex(int bandIndex, int &nvIndex, double &freqMax, double &freqMin)
	{
		int ret = PASS;
		switch (bandIndex)
		{
		case BAND1_INDEX:
			nvIndex = BAND1_NV_INDEX;
			freqMax = NB_BC1_DLFREQ_MAX;
			freqMin = NB_BC1_DLFREQ_MIN;
			break;
		case BAND3_INDEX:
			nvIndex = BAND3_NV_INDEX;
			freqMax = NB_BC3_DLFREQ_MAX;
			freqMin = NB_BC3_DLFREQ_MIN;
			break;
		case BAND8_INDEX:
			nvIndex = BAND8_NV_INDEX;
			freqMax = NB_BC8_DLFREQ_MAX;
			freqMin = NB_BC8_DLFREQ_MIN;
			break;
		case BAND11_INDEX:
			nvIndex = BAND11_NV_INDEX;
			freqMax = NB_BC11_DLFREQ_MAX;
			freqMin = NB_BC11_DLFREQ_MIN;
			break;
		case BAND12_INDEX:
		case BAND17_INDEX:
			nvIndex = BAND12_NV_INDEX;
			freqMax = NB_BC12_DLFREQ_MAX;
			freqMin = NB_BC12_DLFREQ_MIN;
			break;
		case BAND13_INDEX:
			nvIndex = BAND13_NV_INDEX;
			freqMax = NB_BC13_DLFREQ_MAX;
			freqMin = NB_BC13_DLFREQ_MIN;
			break;
		case BAND20_INDEX:
			nvIndex = BAND20_NV_INDEX;
			freqMax = NB_BC20_DLFREQ_MAX;
			freqMin = NB_BC20_DLFREQ_MIN;
			break;
		case BAND2_INDEX:
		case BAND25_INDEX:
			nvIndex = BAND25_NV_INDEX;
			freqMax = NB_BC25_DLFREQ_MAX;
			freqMin = NB_BC25_DLFREQ_MIN;
			break;
		case BAND5_INDEX:
		case BAND18_INDEX:
		case BAND19_INDEX:
		case BAND26_INDEX:
			nvIndex = BAND26_NV_INDEX;
			freqMax = NB_BC26_DLFREQ_MAX;
			freqMin = NB_BC26_DLFREQ_MIN;
			break;
		case BAND28_INDEX:
			nvIndex = BAND28_NV_INDEX;
			freqMax = NB_BC28_DLFREQ_MAX;
			freqMin = NB_BC28_DLFREQ_MIN;
			break;
		case BAND31_INDEX:
			nvIndex = BAND31_NV_INDEX;
			freqMax = NB_BC31_DLFREQ_MAX;
			freqMin = NB_BC31_DLFREQ_MIN;
			break;
		case BAND66_INDEX:
			nvIndex = BAND66_NV_INDEX;
			freqMax = NB_BC66_DLFREQ_MAX;
			freqMin = NB_BC66_DLFREQ_MIN;
			break;
		case BAND70_INDEX:
			nvIndex = BAND70_NV_INDEX;
			freqMax = NB_BC70_DLFREQ_MAX;
			freqMin = NB_BC70_DLFREQ_MIN;
			break;
		default:
			nvIndex = BAND1_NV_INDEX;
			freqMax = NB_BC1_DLFREQ_MAX;
			freqMin = NB_BC1_DLFREQ_MIN;
			ret = FAIL;
			break;
		}
		return ret;
	}
	

};