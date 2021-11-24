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

// 用于非信令综测的宏
#define CHANNEL_NUM		3		// 每个综测频段需要测试的频点
#define TXTESTCASE_NUM	7		// 上行综测项目个数
#define RXTESTCASE_NUM	3		// 下行综测项目个数

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


typedef void(__stdcall    * SEND_LOG_TO_UI)(char*);//log内容，线程号 0/1，颜色序号
typedef struct 
{
	int apcData;				// 设置UE发射机功率控制字，控制UE发射机大小，从config页面中读取
	int paMode;		// 设置UE发射机PA档位，控制UE发射机大小，从config页面中读取
	int afcData1;			// 设置UE的第一个AFC控制字，校准时为了计算马表中控制字的偏置值和斜率，从config页面中读取
	int afcData2;			// 设置UE的第二个AFC控制字，校准时为了计算马表中控制字的偏置值和斜率，从config页面中读取
	int cycleNum;				// AFC微调循环次数，从config页面中读取
	double freq;			// 单位MHz，频率校准的上行频点，在config页面中配置			
	double expPower;			// 单位dBm，上行期望功率，在config页面中配置
	double pathLoss;			// 线损，从config页面中读取
	bool bCWSinaling;		// 标志位，true表示上行发送CW波，false表示上行发射调制波，，从config页面中读取
}AFCConf;
typedef struct 
{
	bool bandCalEnable[BAND_NUM];				// 需要tx power校准的band，true表示需要校准，false表示不需要校准，从config页面读取
	int centerChannel[BAND_NUM];					// 每个校准频段的中间信道，需要从config页面读取
	int compChannelHigh[BAND_NUM];					// 每个校准频段的高补偿信道，需要从config页面读取
	int compChannelLow[BAND_NUM];					// 每个校准频段的低补偿信道，需要从config页面读取
	int apcData1[TX_PA_NUM];						// 每个PA档位校准时使用的第一个功率点的APC控制字，需要从config页面读取
	int apcData2[TX_PA_NUM];						// 每个PA档位校准时使用的第二个功率点的APC控制字，需要从config页面读取

	int initApcData;										// 设置UE初始化发射机功率控制字，控制UE发射机大小，从config页面中读取
	int paMode;								// 设置UE初始化发射机PA档位，控制UE发射机大小，从config页面中读取
	double freq;									// 单位MHz，设置UE初始化发射机频率，从config页面中读取
	double centerFreqPathLossUL[BAND_NUM];			// 每个待校准频段中心频点对应的上行线损，，从config页面中读取
	double compFreqPathLossHighUL[BAND_NUM];		// 每个待校准频段高频点对应的上行线损，，从config页面中读取
	double compFreqPathLossLowUL[BAND_NUM];			// 每个待校准频段低频点对应的上行线损，，从config页面中读取
	bool bCWSinaling;								// 标志位，true表示上行发送CW波，false表示上行发射调制波，，从config页面中读取

}APCConf;
typedef struct 
{
	bool bandCalEnable[BAND_NUM];				// 需要tx power校准的band，true表示需要校准，false表示不需要校准，从config页面读取
	int centerChannel[BAND_NUM];					// 每个校准频段的中间信道，需要从config页面读取
	int compChannelHigh[BAND_NUM];					// 每个校准频段的高补偿信道，需要从config页面读取
	int compChannelLow[BAND_NUM];					// 每个校准频段的低补偿信道，需要从config页面读取
	double agcPower[RX_GAIN_NUM];					// 不同接收机增益状态下配置的仪表下行功率，需要从config页面读取

	double freq;									// 单位MHz，设置UE初始化接收机频率，从config页面中读取
	double rxGainMode;								// 初始化接收机使用的接收机增益档位，从config页面中读取
	double cellPower;								// 单位dBm，用于初始化接收机时仪表的下行功率，从config页面中读取
	double centerFreqPathLossDL[BAND_NUM];			// 每个待校准频段中心频点对应的下行线损，从config页面中读取
	double compFreqPathLossHighDL[BAND_NUM];		// 每个待校准频段高频点对应的下行线损，从config页面中读取
	double compFreqPathLossLowDL[BAND_NUM];			// 每个待校准频段低频点对应的下行线损，从config页面中读取

}AGCConf;

typedef struct
{
	bool bandNSFTEnable[BAND_NUM];						// 需要综测的band，true表示需要测试，false表示不需要测试，从config页面读取
	int channel[BAND_NUM][CHANNEL_NUM];					// 每个综测频段的信道，需要从config页面读取
	bool TXTestCaseEnable[TXTESTCASE_NUM];				// 上行测试项使能，目前是7项，0：最大功率，1：最小功率，2：频率误差，3：EVM，4：ACLR，5：SEM，6：OBW
	bool RXTestCaseEnable[RXTESTCASE_NUM];				// 下行测试项使能，目前是3项，0：最大输入电平，1：RSRP，2：BLER
	double TXMaxPower;									// 上行最大功率
	int TXMaxPowerLimit;								// 上行最大功率门限
	int TXMinPowerLimit;								// 下行最小功率门限
	double FreqErrLimit;								// 频率误差门限，单位为ppm，一般设置为1ppm
	double EVMLimit;									// EVM测试门限
	double OBWLimit;									// OBW测试门限
	double ACLRLimit[2];								// ACLR测试门限
	double MaxInputLimit;								// 最大输入电平门限
	double RSRPLimit[RX_GAIN_NUM];						// RSRP门限
	double BLERLimit;									// BLER门限，单位%
	double RXMaxPower;									// 最大小区功率，用于测量最大输入电平
	double RXBlerPower;									// 用于测试BLER的小区功率
	double RXRSCPPower[RX_GAIN_NUM];					// 下行RSRP测试每段的仪表小区功率
	int RXRSCPLimit[RX_GAIN_NUM];						// 下行RSRP测试门限

	double freq;										// 单位MHz，设置UE初始化接收机频率，从config页面中读取
	double cellPower;									// 单位dBm，用于初始化接收机时仪表的下行功率，从config页面中读取
	unsigned char mcs;									// 调制阶数
	unsigned char toneNum;								// tone 数
	unsigned char toneStart;							// tone起始位置
	unsigned char subCarrierSpacing;					// 子载波间隔，0表示15kHz，1表示3.75kHz
	int subFrameNumber;									// 子帧数，用于BLER测试
	int timeout;										// BLER测试超时时间，单位为ms


	double centerFreqPathLossDL[BAND_NUM][CHANNEL_NUM];	// 每个综测频段下行线损，从config页面中读取
	double centerFreqPathLossUL[BAND_NUM][CHANNEL_NUM];	// 每个综测频段上行行线损，从config页面中读取
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
	//合并字节数组
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