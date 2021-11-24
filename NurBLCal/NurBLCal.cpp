#include "stdafx.h"
#include "NurBLCal.h"
#include "comdef.h"


NurBLCal::NurBLCal()
{
}


NurBLCal::~NurBLCal()
{
}



int NurBLCal::SetLogPtr(SEND_LOG_TO_UI logOutputEX)
{
	m_logOutput = logOutputEX;
	return PASS;
}

int NurBLCal::InitDUTAPI()
{
	char logBuff[LOG_BUFF_LEN] = { '\0' };
	string currentPath = NurCommonFunc::GetProgramDir();
	LPCWSTR lp = NurCommonFunc::stringToLPCWSTR(currentPath + "\\NurDUT.dll");
	m_nurDutDLL = LoadLibrary(lp);
	if (m_nurDutDLL != NULL)
	{
		DUTFUNC lpFunc = (DUTFUNC)GetProcAddress(m_nurDutDLL, "createDUT");
		if (!lpFunc)
		{
			FreeLibrary(m_nurDutDLL);

			sprintf_s(logBuff, "初始化DUT的API失败");
			m_logOutput(logBuff);
			return FAIL;
		}
		else
		{
			m_pDUT = lpFunc();

			sprintf_s(logBuff, "初始化DUT的API成功");
			m_logOutput(logBuff);
			return PASS;
		}
	}
	return FAIL;
}

int NurBLCal::ReleaseDUTAPI()
{
	char logBuff[LOG_BUFF_LEN] = { '\0' };
	if (NULL != m_pDUT)
	{
		delete (m_pDUT);
		m_pDUT = nullptr;
		FreeLibrary(m_nurDutDLL);
	}
	sprintf_s(logBuff, "释放DUT的API成功");
	m_logOutput(logBuff);
	return PASS;
}

int NurBLCal::InitInstrumentAPI()
{
	char logBuff[LOG_BUFF_LEN] = { '\0' };
	string currentPath = NurCommonFunc::GetProgramDir();
	LPCWSTR lp = NurCommonFunc::stringToLPCWSTR(currentPath + "\\NurInstrument.dll");
	m_nurInstruDLL = LoadLibrary(lp);
	if (m_nurInstruDLL != NULL)
	{
		INSTRUFUNC lpFunc = (INSTRUFUNC)GetProcAddress(m_nurInstruDLL, "createInstrument");
		if (!lpFunc)
		{
			FreeLibrary(m_nurInstruDLL);
			sprintf_s(logBuff, "初始化仪表的API失败");
			m_logOutput(logBuff);
			return FAIL;
		}
		else
		{
			m_pInstru = lpFunc();
			sprintf_s(logBuff, "初始化仪表的API成功");
			m_logOutput(logBuff);
			return PASS;
		}
	}
	return FAIL;
}

int NurBLCal::ReleaseInstrumentAPI()
{
	char logBuff[LOG_BUFF_LEN] = { '\0' };
	if (NULL != m_pInstru)
	{
		delete (m_pInstru);
		m_pInstru = nullptr;
		FreeLibrary(m_nurInstruDLL);
	}
	sprintf_s(logBuff, "释放仪表的API成功");
	m_logOutput(logBuff);
	return PASS;
}

int NurBLCal::OpenSerialPortEX(int baudRate, const wchar_t* serialPort)
{
	char logBuff[LOG_BUFF_LEN] = { '\0' };
	int ret = FAIL;
	bool result = false;
	if (NULL != m_pDUT)
	{
		result = m_pDUT->OpenSerialPort(baudRate, serialPort);
	}
	if (result)
	{
		sprintf_s(logBuff, "打开串口成功");
		m_logOutput(logBuff);
		ret = PASS;
	}
	else
	{
		sprintf_s(logBuff, "打开串口失败");
		m_logOutput(logBuff);
		ret = FAIL;
	}
	return ret;
}

int NurBLCal::CloseSerialPortEX()
{
	char logBuff[LOG_BUFF_LEN] = { '\0' };
	int ret = FAIL;
	bool result = false;
	if (NULL != m_pDUT)
	{
		result = m_pDUT->CloseSerialPort();
	}
	if (result)
	{
		sprintf_s(logBuff, "关闭串口成功");
		m_logOutput(logBuff);
		ret = PASS;
	}
	else
	{
		sprintf_s(logBuff, "关闭串口失败");
		m_logOutput(logBuff);
		ret = FAIL;
	}
	return ret;
}

int NurBLCal::OpenInstrument(const wchar_t* intrumentIP)
{
	int ret = FAIL;					// 仪表操作结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };		// log打印内容

	// 如果仪表类对象指针不为空，和仪表进行连接，获取仪表操作句柄
	if (NULL != m_pInstru)
	{
		// 和仪表进行连接，获取仪表操作句柄
		ret = m_pInstru->OpenInstru((_bstr_t)intrumentIP);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "连接仪表失败！仪表IP = %ws", intrumentIP);
			m_logOutput(logBuff);
			ret = FAIL;
		}
		else
		{
			sprintf_s(logBuff, "连接仪表成功！仪表IP = %s", intrumentIP);
			m_logOutput(logBuff);
			ret = PASS;
		}
	}
	else
	{
		sprintf_s(logBuff, "仪表类对象指针为空，无法调用仪表库函数！");
		m_logOutput(logBuff);
		ret = FAIL;
	}
	return ret;
}

int NurBLCal::ReleaseInstrument()
{
	int ret = FAIL;					// 仪表操作结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };		// log打印内容

									// 如果仪表类对象指针不为空，调用仪表库函数释放仪表资源
	if (NULL != m_pInstru)
	{
		ret = m_pInstru->CloseInstru();
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "释放仪表资源失败！");
			m_logOutput(logBuff);
			ret = FAIL;
		}
		else
		{
			sprintf_s(logBuff, "释放仪表资源失败！");
			m_logOutput(logBuff);
			ret = PASS;
		}
	}
	else
	{
		sprintf_s(logBuff, "仪表类对象指针为空，无法调用仪表库函数！");
		m_logOutput(logBuff);
		ret = FAIL;
	}

	return ret;
}

//-------------------------------------------------------校准函数----------------------------------//


/********************************************************************
* Function：AFCCal
* Description: frequency calibration
* Input:
* SEND_LOG_TO_UI m_logOutput: the contents of log
* NurRFNVCal nvTable: nv table
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History：
Name	Date		Contents Reason
Jason	2019-1-21	Add frequency calibration
.
.
********************************************************************/
int NurBLCal::AFCCal(AFCConf* config)
{
	NurRFNVCal nvTable;

	/*----------------------------------------完美分割线---------------------------------------------*/

	int ret = PASS;					// 校准结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };		// log打印内容

	int afcData = 0;				// 频率控制字的偏置值，该值在校准过程中可以满足频率误差要求，保存至AFC码表中
	int slope = 0;					// 频率控制的斜率，该值为频率 - 控制字之间的变化关系，保存至AFC码表中

	double freqErr = 0;				// 频率误差，单位Hz，从仪表获取，用于保存最终保存码表的频率控制字对应的频偏
	double freqErr1 = 0;				// 频率误差，单位Hz，从仪表获取，用于保存第一个AFC控制字对应的频偏
	double freqErr2 = 0;				// 频率误差，单位Hz，从仪表获取，用于保存第二个AFC控制字对应的频偏

	int limit = config->freq;				// 频率误差门限，单位Hz，为上行校准频率的1ppm，该门限会根据实际测试频点变化
	double power = 20.0;			// 单位dBm，频率校准的上行功率，用于从仪表获取UE上行功率

									// 首先判定仪表类对象指针和DUT类对象指针，如果类对象指针不为空，继续校准
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "仪表类对象指针为空，无法调用仪表库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "终端通信类对象指针为空，无法调用终端通信库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 初始化仪表配置参数
	ret = m_pInstru->InitInstrument();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "初始化仪表配置参数失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "初始化仪表配置参数成功！");
		m_logOutput(logBuff);
	}

	// 设置仪表参数中的上行测量频率
	ret = m_pInstru->SetMeasurementFreq(config->freq, config->bCWSinaling);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表参数中的上行测量频率失败！频率为 %.1f MHz", config->freq);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表参数中的上行测量频率成功！频率为 %.1f MHz", config->freq);
		m_logOutput(logBuff);
	}

	// 设置仪表参数中的上行期望功率
	ret = m_pInstru->SetExpPower(config->expPower, config->bCWSinaling);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表参数中的上行期望功率失败！期望功率为 %.1f dBm", config->expPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表参数中的上行期望功率成功！期望功率为 %.1f dBm", config->expPower);
		m_logOutput(logBuff);
	}

	// 设置仪表上行线损
	ret = m_pInstru->SetMeasurementLoss(config->pathLoss, config->bCWSinaling);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表参数中的上行线损失败！线损为 %.1f dB", config->pathLoss);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表参数中的上行线损成功！线损为 %.1f dB", config->pathLoss);
		m_logOutput(logBuff);
	}

	// 打开UE发射机，需要从config页面中获取频率、APC控制字和PA档位
	if (m_pDUT->OpenTransmitter(config->freq, config->paMode, config->apcData))
	{
		sprintf_s(logBuff, "打开UE发射机成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "打开UE发射机失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 设置UE发射机AFC控制字1
	if (m_pDUT->SetTransmitterFrequencyOffset(config->afcData1))
	{
		sprintf_s(logBuff, "设置UE发射机AFC控制字1成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "设置UE发射机AFC控制字1失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 从仪表读取第一个AFC控制字对应的频率误差
	ret = m_pInstru->GetFreqErr(freqErr1, power);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "从仪表读取第一个AFC控制字对应的频率误差失败！AFC控制字为 %d", config->afcData1);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "从仪表读取第一个AFC控制字对应的频率误差成功！AFC控制字为 %d，频率误差为 %d Hz，上行功率为 %.1f dBm", config->afcData1, freqErr1, power);
		m_logOutput(logBuff);
	}

	// 设置UE发射机AFC控制字2
	if (m_pDUT->SetTransmitterFrequencyOffset(config->afcData2))
	{
		sprintf_s(logBuff, "设置UE发射机AFC控制字2成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "设置UE发射机AFC控制字2失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 从仪表读取第二个AFC控制字对应的频率误差
	ret = m_pInstru->GetFreqErr(freqErr2, power);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "从仪表读取第二个AFC控制字对应的频率误差失败！AFC控制字为 %d", config->afcData2);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "从仪表读取第二个AFC控制字对应的频率误差成功！AFC控制字为 %d，频率误差为 %d Hz，上行功率为 %.1f dBm", config->afcData2, freqErr2, power);
		m_logOutput(logBuff);
	}

	// 计算AFC控制字斜率和偏置值，斜率单位为 Hz，表示AFC控制字每变化1，频率的变化值
	slope = (freqErr1 - freqErr2) / (config->afcData1 - config->afcData2);
	afcData = config->afcData2 + freqErr2 / slope;

	sprintf_s(logBuff, "计算AFC控制字斜率和偏置值，AFC控制字斜率为 %d，AFC偏置值为 %d", slope, afcData);
	m_logOutput(logBuff);

	// 确认计算出的AFC控制字偏置量是否满足要求，不满足要求需要根据斜率进行微调
	for (int actualCycleNum = 0; actualCycleNum <= config->cycleNum; actualCycleNum++)
	{
		// 设置UE发射机AFC控制字，该控制字为刚刚计算出的AFC控制字偏置值
		if (m_pDUT->SetTransmitterFrequencyOffset(afcData))
		{
			sprintf_s(logBuff, "设置UE发射机AFC控制字成功");
			m_logOutput(logBuff);
		}
		else
		{
			sprintf_s(logBuff, "设置UE发射机AFC控制字失败");
			m_logOutput(logBuff);
			return FAIL;
		}


		// 从仪表读取第AFC控制字偏置值对应的频率误差
		ret = m_pInstru->GetFreqErr(freqErr, power);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "从仪表读取AFC控制字偏置值对应的频率误差失败！AFC控制字为 %d", afcData);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "从仪表读取AFC控制字偏置值对应的频率误差成功！AFC控制字为 %d，频率误差为 %d Hz，上行功率为 %.1f dBm", afcData, freqErr, power);
			m_logOutput(logBuff);
		}

		// 判断AFC控制字偏置值对应的频偏是否满足要求，
		// 如果满足要求，则将偏置值和斜率写入AFC码表
		// 如果不满足要求，需要根据斜率进行微调
		// 如果超出微调循环次数，则报错退出
		if (abs(freqErr) < limit)
		{
			// 赋值给码表
			sprintf_s(logBuff, "从仪表读取的频率误差满足要求！AFC控制字为 %d，斜率为 %d", afcData, slope);
			m_logOutput(logBuff);
			nvTable.afc.afcSlope = (short)slope;
			nvTable.afc.afcInit = (unsigned short)afcData;
			ret = PASS;
			break;
		}
		else if (actualCycleNum < config->cycleNum)
		{
			afcData = afcData + freqErr / slope;
			sprintf_s(logBuff, "从仪表读取的频率误差不满足要求！AFC控制字正在进行第%d次微调，调整后的AFC控制字为 %d", actualCycleNum + 1, afcData);
			m_logOutput(logBuff);
		}
		else
		{
			sprintf_s(logBuff, "从仪表读取的频率误差不满足要求！已达到最大微调次数！AFC校准失败！");
			m_logOutput(logBuff);
			ret = FAIL;
			return ret;
		}
	}

	// 和UE通信，将AFC码表写入NV
	if (m_pDUT->WriteAFCCodeTable(nvTable.afc))
	{
		sprintf_s(logBuff, "AFC码表写入NV成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "AFC码表写入NV失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 和UE通信，更新AFC码表至RAM
	if (m_pDUT->UpdateCodeTableToRAM())
	{
		sprintf_s(logBuff, "更新AFC码表至RAM成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "更新AFC码表至RAM失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	return ret;
}

/********************************************************************
* Function：APCCal
* Description: tx power calibration
* Input:
* NurRFNVCal nvTable: nv table
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History：
Name	Date		Contents Reason
Jason	2019-1-22	Add tx power calibration
.
.
********************************************************************/
int NurBLCal::APCCal(APCConf* config)
{
	NurRFNVCal nvTable;

	int ret = PASS;											// 校准结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log打印内容

	int apcData[BAND_NUM][TX_PA_NUM] = { 0 };				// 每个校准频段每个PA档位校准后计算出的APC控制字偏置值
	double apcSlope[BAND_NUM][TX_PA_NUM] = { 0 };			// 每个校准频段每个PA档位校准后计算出的APC控制字斜率

	double apcSlope_High[BAND_NUM][TX_PA_NUM] = { 0 };		// 每个校准频段每个PA档位校准后计算出的高频点补偿的斜率
	double apcSlope_Low[BAND_NUM][TX_PA_NUM] = { 0 };		// 每个校准频段每个PA档位校准后计算出的低频点补偿的斜率
	double expPower1[TX_PA_NUM] = { 0 };					// 每个PA档位校准时使用的第一个功率点对应的期望功率，根据码表宏定义获取
	double expPower2[TX_PA_NUM] = { 0 };					// 每个PA档位校准时使用的第二个功率点对应的期望功率，根据码表宏定义获取

	// 仪表配置参数期望功率赋值，从码表宏定义获取码表范围，根据范围进行定义
	expPower1[0] = APC_HIGH_START + 10;
	expPower1[1] = APC_MID_START + 10;
	expPower1[2] = APC_LOW_START + 10;
	expPower2[0] = APC_HIGH_END + 20;
	expPower2[1] = APC_MID_END + 20;
	expPower2[2] = APC_LOW_END + 20;

	// 首先判定仪表类对象指针和DUT类对象指针，如果类对象指针不为空，继续校准
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "仪表类对象指针为空，无法调用仪表库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "终端通信类对象指针为空，无法调用终端通信库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 初始化发射机流程
	ret = InitTransmitter(config);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "初始化终端发射机流程失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "初始化终端发射机流程成功！");
		m_logOutput(logBuff);
	}

	// 循环开始每个待测频段的校准
	for (int i = 0; i < BAND_NUM; i++)
	{
		// 按照3GPP标准，一共18个待测频段，依次分别为1、2、3、5、8、11、12、13、17、18、19、20、25、26、28、31、66、70
		// 这18个频段对应到数组bandCalEnable[]的下标从0-17
		// 由于出现了频点范围的重复，band12包含band17，band25包含band2，band26包含band2/18/19，因此NV码表去除了band17/2/5/18/19
		// NV码表一共13个频段，分别是1、3、8、11、12、13、20、25、26、28、31、66、70
		// 这13个频段对应到NV码表的数组NurRFNVCal.Info[]的下标从0-12
		// 因此，如果band12/17互斥，band2/25互斥，band5/18/19互斥，在配置界面中，互斥频段中的一个频段被使能，则其他频段被禁止
		// 并且在校准中，band17会自动映射到band12，band2会自动映射到band25，band2/18/19会自动映射到band26
		/*			Index		Band			UL Frequencies				DL frequencies				NV Index

		0			1				1920 - 1980 MHz				2110 - 2170 MHz				0
		1			2				1850 - 1910 MHz				1930 - 1990 MHz				7
		2			3				1710 - 1785 MHz				1805 - 1880 MHz				1
		3			5				824 - 849MHz				869 - 894MHz				8
		4			8				880 - 915 MHz				925 - 960 MHz				2
		5			11				1427.9 - 1447.9 MHz			1475.9 - 1495.9 MHz			3
		6			12				699 - 716 MHz				729 - 746 MHz				4
		7			13				777 - 787 MHz				746 - 756 MHz				5
		8			17				704 - 716 MHz				734 - 746 MHz				4
		9			18				815 - 830 MHz				875 - 890 MHz				8
		10			19				830 - 845 MHz				875 - 890 MHz				8
		11			20				832 - 862 MHz				791 - 821 MHz				6
		12			25				1850 - 1915 MHz				1930 - 1995 MHz				7
		13			26				814 - 849 MHz				859 - 894 MHz				8
		14			28				703 - 748 MHz				785 - 803 MHz				9
		15			31				452.5 – 457.5 MHz			462.5 – 467.5 MHz			10
		16			66				1710 - 1780 MHz				2110 - 2200 MHz				11
		17			70				1695 - 1710 MHz				1995 - 2020 MHz				12
		*/

		// 首先判定待测频段是否被使能，如果没有使能，则直接跳转到下一个频段
		if (false == config->bandCalEnable[i])
		{
			continue;
		}

		// i就是上表中的index，现在需要映射到NV码表中对应的NV index，并得到该NV频段对应的最大频点和最小频点
		int nvIndex = 0;						// 待测频段在NV马表中对应的序号
		double freqDLMax = 0;					// 待测频段对应的NV码表频段的最大频点，用于频率补偿计算
		double freqDLMin = 0;					// 待测频段对应的NV码表频段的最小频点，用于频率补偿计算
		ret = NurCommonFunc::GetNVIndexFromBandIndex(i, nvIndex, freqDLMax, freqDLMin);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "根据频带的索引获取NV的索引，失败");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据频带的索引获取NV的索引，成功");
			m_logOutput(logBuff);
		}

		double centerFreqDL = 0;				// 待测频段下行中心频点
		double centerFreqUL = 0;				// 待测频段上行中心频点

		// 根据下行信道号获取对应的上下行频点
		ret = NurCommonFunc::GetFreqFromDLChan(config->centerChannel[i], centerFreqDL, centerFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "中心信道信道号为非法数据！信道号为 %d", config->centerChannel[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据下行信道号获取对应的上下行频点成功！下行信道号为 %d，下行中心频点为 %.1f MHz，上行中心频点为 %.1f MHz", config->centerChannel[i], centerFreqDL, centerFreqUL);
			m_logOutput(logBuff);
		}

		// 设置仪表参数中的上行测量频率
		ret = m_pInstru->SetMeasurementFreq(centerFreqUL, config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表参数中的上行测量频率失败！频率为 %.1f MHz", centerFreqUL);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表参数中的上行测量频率成功！频率为 %.1f MHz", centerFreqUL);
			m_logOutput(logBuff);
		}

		// 设置仪表上行线损
		ret = m_pInstru->SetMeasurementLoss(config->centerFreqPathLossUL[i], config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表参数中的上行线损失败！线损为 %.1f dB", config->centerFreqPathLossUL[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表参数中的上行线损成功！线损为 %.1f dB", config->centerFreqPathLossUL[i]);
			m_logOutput(logBuff);
		}

		// 循环测试中心频点不同PA档位下的功率
		for (int j = 0; j < TX_PA_NUM; j++)
		{
			// 不同PA档位对应的码表最大功率和精度
			string paMode = "";				// PA档位描述字符串，仅用于log输出更加直观
			double apcNVPowerMax = 0;		// 不同PA档位对应的APC码表最大功率
			double apcNVPowerStep = 0;		// 不同PA档位对应的APC码表精度
			int apcNVLen = 0;
			switch (j)
			{
			case TX_PA_LOW:
				paMode = "低增益档";
				apcNVPowerMax = APC_LOW_START;
				apcNVPowerStep = APC_LOW_STEP;
				apcNVLen = APC_LOW_NUM;
				break;
			case TX_PA_MID:
				paMode = "中增益档";
				apcNVPowerMax = APC_MID_START;
				apcNVPowerStep = APC_MID_STEP;
				apcNVLen = APC_MID_NUM;
				break;
			default:
				paMode = "高增益档";
				apcNVPowerMax = APC_HIGH_START;
				apcNVPowerStep = APC_HIGH_STEP;
				apcNVLen = APC_HIGH_NUM;
				break;
			}

			// 设置仪表参数中的上行期望功率
			ret = m_pInstru->SetExpPower(expPower1[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "设置仪表参数中的上行期望功率失败！期望功率为 %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "设置仪表参数中的上行期望功率成功！期望功率为 %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
			}

			// 设置UE发射机频点

			// 设置UE发射机PA档位和该PA档位下第一个APC控制字

			// 从仪表读取第一个APC控制字对应的上行功率
			ret = m_pInstru->GetTXPower(m_power1[nvIndex][j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "从仪表读取第一个APC控制字对应的上行功率失败！APC控制字为 %d, PA增益档位为%s", config->apcData1[j], paMode);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "从仪表读取第一个APC控制字对应的上行功率成功！APC控制字为 %d，PA增益档位为%s，上行功率为 %.1f dBm", config->apcData1[j], paMode, m_power1[j]);
				m_logOutput(logBuff);
			}

			// 设置UE发射机PA档位和该PA档位下第二个APC控制字

			// 设置仪表参数中的上行期望功率
			ret = m_pInstru->SetExpPower(expPower2[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "设置仪表参数中的上行期望功率失败！期望功率为 %.1f dBm", expPower2[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "设置仪表参数中的上行期望功率成功！期望功率为 %.1f dBm", expPower2[j]);
				m_logOutput(logBuff);
			}

			// 从仪表读取第二个APC控制字对应的上行功率
			ret = m_pInstru->GetTXPower(m_power2[nvIndex][j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "从仪表读取第二个APC控制字对应的上行功率失败！APC控制字为 %d, PA增益档位为%s", config->apcData2[j], paMode);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "从仪表读取第二个APC控制字对应的上行功率成功！APC控制字为 %d，PA增益档位为%s，上行功率为 %.1f dBm", config->apcData2[j], paMode, m_power1[j]);
				m_logOutput(logBuff);
			}

			// 根据该PA档位下的两个功率点数据，计算出偏置值和斜率
			// 根据偏置值和斜率计算出该PA档位码表所有APC功率对应APC控制字

			// 计算APC控制字斜率和偏置值，斜率单位为 /dB，表示上行功率每变化1dB，APC控制字的变化值
			apcSlope[i][j] = (config->apcData1[j] - config->apcData2[j]) / (m_power1[j] - m_power2[j]);

			// 计算出的偏置值为上行功率在不同增益档位码表最大功率(APC_HIGH/MID/LOW_START)对应的APC控制字
			apcData[i][j] = (apcNVPowerMax - m_power1[nvIndex][j]) / apcSlope[i][j] + config->apcData1[j];

			sprintf_s(logBuff, "计算APC控制字斜率和偏置值，APC控制字斜率为 %s，APC偏置值为 %d，PA增益档位为%s", apcSlope[i][j], apcData[i][j], paMode);
			m_logOutput(logBuff);
			for (int k = 0; k < apcNVLen; k++)
			{
				// 以PA某增益档位码表最大功率(APC_HIGH/MID/LOW_START)对应的APC控制字为偏置值，码表数据下标0表示该码表范围内的最大功率
				// 码表数组下标每增加1，则功率减少一个精度APC_HIGH/MID/LOW_STEP，对应的控制字减少k * 精度 * 斜率
				nvTable.info[nvIndex].apc.apcHigh[k] = apcData[i][j] - int(k * apcNVPowerStep * apcSlope[i][j]);
			}
		}
	}
	return ret;
}

/********************************************************************
* Function：TFCCal
* Description: tx power frequency compensation calibration
* Input:
* NurRFNVCal nvTable: nv table
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History：
Name	Date		Contents Reason
Jason	2019-3-28	Add tx power frequency compensation calibration
.
.
********************************************************************/
int NurBLCal::TFCCal(APCConf* config)
{
	NurRFNVCal nvTable;

	int ret = PASS;											// 校准结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log打印内容

	double apcSlope_High[BAND_NUM][TX_PA_NUM] = { 0 };		// 每个校准频段每个PA档位校准后计算出的高频点补偿的斜率
	double apcSlope_Low[BAND_NUM][TX_PA_NUM] = { 0 };		// 每个校准频段每个PA档位校准后计算出的低频点补偿的斜率
	double expPower1[TX_PA_NUM] = { 0 };					// 每个PA档位校准时使用的第一个功率点对应的期望功率，根据码表宏定义获取

	// 仪表配置参数期望功率赋值，从码表宏定义获取码表范围，根据范围进行定义
	expPower1[0] = APC_HIGH_START + 10;
	expPower1[1] = APC_MID_START + 10;
	expPower1[2] = APC_LOW_START + 10;

	// 首先判定仪表类对象指针和DUT类对象指针，如果类对象指针不为空，继续校准
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "仪表类对象指针为空，无法调用仪表库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "终端通信类对象指针为空，无法调用终端通信库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 初始化发射机流程
	ret = InitTransmitter(config);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "初始化终端发射机流程失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "初始化终端发射机流程成功！");
		m_logOutput(logBuff);
	}

	// 循环开始每个待测频段的校准
	for (int i = 0; i < BAND_NUM; i++)
	{
		// 按照3GPP标准，一共18个待测频段，依次分别为1、2、3、5、8、11、12、13、17、18、19、20、25、26、28、31、66、70
		// 这18个频段对应到数组bandCalEnable[]的下标从0-17
		// 由于出现了频点范围的重复，band12包含band17，band25包含band2，band26包含band2/18/19，因此NV码表去除了band17/2/5/18/19
		// NV码表一共13个频段，分别是1、3、8、11、12、13、20、25、26、28、31、66、70
		// 这13个频段对应到NV码表的数组NurRFNVCal.Info[]的下标从0-12
		// 因此，如果band12/17互斥，band2/25互斥，band5/18/19互斥，在配置界面中，互斥频段中的一个频段被使能，则其他频段被禁止
		// 并且在校准中，band17会自动映射到band12，band2会自动映射到band25，band2/18/19会自动映射到band26
		/*			Index		Band			UL Frequencies				DL frequencies				NV Index

		0			1				1920 - 1980 MHz				2110 - 2170 MHz				0
		1			2				1850 - 1910 MHz				1930 - 1990 MHz				7
		2			3				1710 - 1785 MHz				1805 - 1880 MHz				1
		3			5				824 - 849MHz				869 - 894MHz				8
		4			8				880 - 915 MHz				925 - 960 MHz				2
		5			11				1427.9 - 1447.9 MHz			1475.9 - 1495.9 MHz			3
		6			12				699 - 716 MHz				729 - 746 MHz				4
		7			13				777 - 787 MHz				746 - 756 MHz				5
		8			17				704 - 716 MHz				734 - 746 MHz				4
		9			18				815 - 830 MHz				875 - 890 MHz				8
		10			19				830 - 845 MHz				875 - 890 MHz				8
		11			20				832 - 862 MHz				791 - 821 MHz				6
		12			25				1850 - 1915 MHz				1930 - 1995 MHz				7
		13			26				814 - 849 MHz				859 - 894 MHz				8
		14			28				703 - 748 MHz				785 - 803 MHz				9
		15			31				452.5 – 457.5 MHz			462.5 – 467.5 MHz			10
		16			66				1710 - 1780 MHz				2110 - 2200 MHz				11
		17			70				1695 - 1710 MHz				1995 - 2020 MHz				12
		*/

		// 首先判定待测频段是否被使能，如果没有使能，则直接跳转到下一个频段
		if (false == config->bandCalEnable[i])
		{
			continue;
		}

		// i就是上表中的index，现在需要映射到NV码表中对应的NV index，并得到该NV频段对应的最大频点和最小频点
		int nvIndex = 0;						// 待测频段在NV马表中对应的序号
		double freqDLMax = 0;					// 待测频段对应的NV码表频段的最大频点，用于频率补偿计算
		double freqDLMin = 0;					// 待测频段对应的NV码表频段的最小频点，用于频率补偿计算
		ret = NurCommonFunc::GetNVIndexFromBandIndex(i, nvIndex, freqDLMax, freqDLMin);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "根据频带的索引获取NV的索引，失败");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据频带的索引获取NV的索引，成功");
			m_logOutput(logBuff);
		}

		double centerFreqDL = 0;				// 待测频段下行中心频点
		double centerFreqUL = 0;				// 待测频段上行中心频点
		double compPower[TX_PA_NUM] = { 0 };	// 保存每个PA档位校准时使用的频率补偿功率点的APC控制字对应的测量功率

		// 高频点频率补偿校准	
		double compFreqDL = 0;					// 待测频段下行最大频点
		double compFreqUL = 0;					// 待测频段上行最小频点

		// 根据下行信道号获取对应的上下行频点
		ret = NurCommonFunc::GetFreqFromDLChan(config->compChannelHigh[i], compFreqDL, compFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "频率补偿信道信道号为非法数据！信道号为 %d", config->compChannelHigh[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据下行信道号获取对应的上下行频点成功！下行信道号为 %d，下行中心频点为 %.1f MHz，上行中心频点为 %.1f MHz", config->compChannelHigh[i], compFreqDL, compFreqUL);
			m_logOutput(logBuff);
		}

		// 设置仪表参数中的上行测量频率
		ret = m_pInstru->SetMeasurementFreq(compFreqUL, config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表参数中的上行测量频率失败！频率为 %.1f MHz", compFreqUL);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表参数中的上行测量频率成功！频率为 %.1f MHz", compFreqUL);
			m_logOutput(logBuff);
		}

		// 设置仪表上行线损
		ret = m_pInstru->SetMeasurementLoss(config->compFreqPathLossHighUL[i], config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表参数中的上行线损失败！线损为 %.1f dB", config->compFreqPathLossHighUL[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表参数中的上行线损成功！线损为 %.1f dB", config->compFreqPathLossHighUL[i]);
			m_logOutput(logBuff);
		}

		// 循环测试补偿信道不同PA档位下的功率
		for (int j = 0; j < TX_PA_NUM; j++)
		{
			// 不同PA档位对应的码表最大功率和精度
			string paMode = "";				// PA档位描述字符串，仅用于log输出更加直观
			double apcNVPowerStep = 0;		// 不同PA档位对应的APC码表精度
			switch (j)
			{
			case TX_PA_LOW:
				paMode = "低增益档";
				apcNVPowerStep = APC_LOW_STEP;
				break;
			case TX_PA_MID:
				paMode = "中增益档";
				apcNVPowerStep = APC_MID_STEP;
				break;
			default:
				paMode = "高增益档";
				apcNVPowerStep = APC_HIGH_STEP;
				break;
			}

			// 设置仪表参数中的上行期望功率
			ret = m_pInstru->SetExpPower(expPower1[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "设置仪表参数中的上行期望功率失败！期望功率为 %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "设置仪表参数中的上行期望功率成功！期望功率为 %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
			}

			// 设置UE发射机频点

			// 设置UE发射机PA档位和该PA档位下第一个APC控制字，根据这个控制字对应的上行功率去和中心频点第一个APC控制字对应的上行功率比较，计算带内平坦度

			// 从仪表读取补偿信道APC控制字对应的上行功率
			ret = m_pInstru->GetTXPower(compPower[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "从仪表读取补偿信道APC控制字对应的上行功率失败！APC控制字为 %d, PA增益档位为%s", config->apcData1[j], paMode);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "从仪表读取补偿信道APC控制字对应的上行功率成功！APC控制字为 %d，PA增益档位为%s，上行功率为 %.1f dBm", config->apcData1[j], paMode, compPower[j]);
				m_logOutput(logBuff);
			}

			// 根据该PA档位下的中心频点和补偿频点两个功率点数据，计算出斜率
			// 根据斜率计算出该PA档位码表所有TFC补偿值

			// 计算发射机频率补偿斜率，斜率单位为APC码表下标偏移 / 100kHz，表示频率每变化100kHz(即一个信道)，物理层查找APC码表下标的偏移值
			// 一定要注意这里的补偿是APC码表下标补偿，不是具体的dB值，下标补偿的精度和APC码表精度一致
			apcSlope_High[i][j] = (compPower[j] - m_power1[nvIndex][j]) / (config->compChannelHigh[i] - config->centerChannel[i]) / apcNVPowerStep;
			sprintf_s(logBuff, "计算发射机频率补偿斜率，补偿斜率为 %.2f，PA档位为%s", apcSlope_High[i][j], paMode);
			m_logOutput(logBuff);

			// 计算频率补偿中的高频点的频率补偿，以待测中心频点为分界，补偿频点个数为TFC码表长度的一半
			for (int k = TFC_FREQ_NUM / 2; k < TFC_FREQ_NUM; k++)
			{
				nvTable.info[nvIndex].tfc.info[k].frequency = short((freqDLMax - centerFreqDL) * 10 * (k - double(TFC_FREQ_NUM / 2) + 1) / (TFC_FREQ_NUM / 2));
				nvTable.info[nvIndex].tfc.info[k].comp[j] = char(nvTable.info[nvIndex].tfc.info[k].frequency * apcSlope_High[i][j]);
			}
		}

		// 根据下行信道号获取对应的上下行频点
		ret = NurCommonFunc::GetFreqFromDLChan(config->compChannelLow[i], compFreqDL, compFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "频率补偿信道信道号为非法数据！信道号为 %d", config->compChannelLow[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据下行信道号获取对应的上下行频点成功！下行信道号为 %d，下行中心频点为 %.1f MHz，上行中心频点为 %.1f MHz", config->compChannelLow[i], compFreqDL, compFreqUL);
			m_logOutput(logBuff);
		}

		// 设置仪表参数中的上行测量频率
		ret = m_pInstru->SetMeasurementFreq(compFreqUL, config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表参数中的上行测量频率失败！频率为 %.1f MHz", compFreqUL);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表参数中的上行测量频率成功！频率为 %.1f MHz", compFreqUL);
			m_logOutput(logBuff);
		}

		// 设置仪表上行线损
		ret = m_pInstru->SetMeasurementLoss(config->compFreqPathLossLowUL[i], config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表参数中的上行线损失败！线损为 %.1f dB", config->compFreqPathLossLowUL[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表参数中的上行线损成功！线损为 %.1f dB", config->compFreqPathLossLowUL[i]);
			m_logOutput(logBuff);
		}

		// 循环测试补偿信道不同PA档位下的功率
		for (int j = 0; j < TX_PA_NUM; j++)
		{
			// 不同PA档位对应的码表最大功率和精度
			string paMode = "";				// PA档位描述字符串，仅用于log输出更加直观
			double apcNVPowerStep = 0;		// 不同PA档位对应的APC码表精度
			switch (j)
			{
			case TX_PA_LOW:
				paMode = "低增益档";
				apcNVPowerStep = APC_LOW_STEP;
				break;
			case TX_PA_MID:
				paMode = "中增益档";
				apcNVPowerStep = APC_MID_STEP;
				break;
			default:
				paMode = "高增益档";
				apcNVPowerStep = APC_HIGH_STEP;
				break;
			}

			// 设置仪表参数中的上行期望功率
			ret = m_pInstru->SetExpPower(expPower1[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "设置仪表参数中的上行期望功率失败！期望功率为 %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "设置仪表参数中的上行期望功率成功！期望功率为 %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
			}

			// 设置UE发射机频点

			// 设置UE发射机PA档位和该PA档位下第一个APC控制字，根据这个控制字对应的上行功率去和中心频点第一个APC控制字对应的上行功率比较，计算带内平坦度

			// 从仪表读取补偿信道APC控制字对应的上行功率
			ret = m_pInstru->GetTXPower(compPower[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "从仪表读取补偿信道APC控制字对应的上行功率失败！APC控制字为 %d, PA增益档位为%s", config->apcData1[j], paMode);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "从仪表读取补偿信道APC控制字对应的上行功率成功！APC控制字为 %d，PA增益档位为%s，上行功率为 %.1f dBm", config->apcData1[j], paMode, compPower[j]);
				m_logOutput(logBuff);
			}

			// 根据该PA档位下的中心频点和补偿频点两个功率点数据，计算出斜率
			// 根据斜率计算出该PA档位码表所有TFC补偿值

			// 计算发射机频率补偿斜率，斜率单位为APC码表下标偏移 / 100kHz，表示频率每变化100kHz(即一个信道)，物理层查找APC码表下标的偏移值
			// 一定要注意这里的补偿是APC码表下标补偿，不是具体的dB值，下标补偿的精度和APC码表精度一致
			apcSlope_Low[i][j] = (compPower[j] - m_power1[nvIndex][j]) / (config->compChannelLow[i] - config->centerChannel[i]) / apcNVPowerStep;
			sprintf_s(logBuff, "计算发射机频率补偿斜率，补偿斜率为 %.2f，PA档位为%s", apcSlope_Low[i][j], paMode);
			m_logOutput(logBuff);

			// 计算频率补偿中的低频点的频率补偿，以待测中心频点为分界，补偿频点个数为TFC码表长度的一半
			for (int k = 0; k < TFC_FREQ_NUM / 2; k++)
			{
				nvTable.info[nvIndex].tfc.info[k].frequency = short((freqDLMin - centerFreqDL) * 10 * (1 - double(k + 1) / (TFC_FREQ_NUM / 2)));
				nvTable.info[nvIndex].tfc.info[k].comp[j] = char(nvTable.info[nvIndex].tfc.info[k].frequency * apcSlope_Low[i][j]);
			}
		}
	}
	return ret;
}

/********************************************************************
* Function：AGCCal
* Description: rx gain calibration
* Input:
* NurRFNVCal nvTable: nv table
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History：
Name	Date		Contents Reason
Jason	2019-1-22	Add rx gain calibration
.
.
********************************************************************/
int NurBLCal::AGCCal(AGCConf* config)
{
	NurRFNVCal nvTable;
	int ret = 0;											// 校准结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log打印内容

	int rssi[RX_GAIN_NUM] = { 0 };							// UE在不同接收机增益状态下获取到的接收机信号能量值
	int rxGain[RX_GAIN_NUM] = { 0 };						// 根据UE获取的接收机能量值，换算出具体的功率，再减去下行输入信号，得到接收机增益

	// 首先判定仪表类对象指针和DUT类对象指针，如果类对象指针不为空，继续校准
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "仪表类对象指针为空，无法调用仪表库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "终端通信类对象指针为空，无法调用终端通信库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 初始化接收机流程
	ret = InitReceiver(config);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "初始化终端接收机流程失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "初始化终端接收机流程成功！");
		m_logOutput(logBuff);
	}

	// 循环开始每个待测频段的校准
	for (int i = 0; i < BAND_NUM; i++)
	{
		// 按照3GPP标准，一共18个待测频段，依次分别为1、2、3、5、8、11、12、13、17、18、19、20、25、26、28、31、66、70
		// 这18个频段对应到数组bandCalEnable[]的下标从0-17
		// 由于出现了频点范围的重复，band12包含band17，band25包含band2，band26包含band5/18/19，因此NV码表去除了band17/2/5/18/19
		// NV码表一共13个频段，分别是1、3、8、11、12、13、20、25、26、28、31、66、70
		// 这13个频段对应到NV码表的数组NurRFNVCal.Info[]的下标从0-12
		// 因此，如果band12/17互斥，band2/25互斥，band5/18/19互斥，在配置界面中，互斥频段中的一个频段被使能，则其他频段被禁止
		// 并且在校准中，band17会自动映射到band12，band2会自动映射到band25，band2/18/19会自动映射到band26
		/*		Index		Band			UL Frequencies				DL frequencies				NV Index

		0			1				1920 - 1980 MHz				2110 - 2170 MHz				0
		1			2				1850 - 1910 MHz				1930 - 1990 MHz				7
		2			3				1710 - 1785 MHz				1805 - 1880 MHz				1
		3			5				824 - 849MHz				869 - 894MHz				8
		4			8				880 - 915 MHz				925 - 960 MHz				2
		5			11				1427.9 - 1447.9 MHz			1475.9 - 1495.9 MHz			3
		6			12				699 - 716 MHz				729 - 746 MHz				4
		7			13				777 - 787 MHz				746 - 756 MHz				5
		8			17				704 - 716 MHz				734 - 746 MHz				4
		9			18				815 - 830 MHz				875 - 890 MHz				8
		10			19				830 - 845 MHz				875 - 890 MHz				8
		11			20				832 - 862 MHz				791 - 821 MHz				6
		12			25				1850 - 1915 MHz				1930 - 1995 MHz				7
		13			26				814 - 849 MHz				859 - 894 MHz				8
		14			28				703 - 748 MHz				785 - 803 MHz				9
		15			31				452.5 – 457.5 MHz			462.5 – 467.5 MHz			10
		17			66				1710 - 1780 MHz				2110 - 2200 MHz				11
		18			70				1695 - 1710 MHz				1995 - 2020 MHz				12
		*/
		// 首先判定待测频段是否被使能，如果没有使能，则直接跳转到下一个频段
		if (false == config->bandCalEnable[i])
		{
			continue;
		}

		// i就是上表中的index，现在需要映射到NV码表中对应的NV index，并得到该NV频段对应的最大频点和最小频点
		int nvIndex = 0;						// 待测频段在NV马表中对应的序号
		double freqDLMax = 0;					// 待测频段对应的NV码表频段的最大频点，用于频率补偿计算
		double freqDLMin = 0;					// 待测频段对应的NV码表频段的最小频点，用于频率补偿计算
		ret = NurCommonFunc::GetNVIndexFromBandIndex(i, nvIndex, freqDLMax, freqDLMin);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "根据频带的索引获取NV的索引，失败");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据频带的索引获取NV的索引，成功");
			m_logOutput(logBuff);
		}

		double centerFreqDL = 0;				// 待测频段下行中心频点
		double centerFreqUL = 0;				// 待测频段上行中心频点


		// 根据下行信道号获取对应的上下行频点
		ret = NurCommonFunc::GetFreqFromDLChan(config->centerChannel[i], centerFreqDL, centerFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "中心信道信道号为非法数据！信道号为 %d", config->centerChannel[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据下行信道号获取对应的上下行频点成功！下行信道号为 %d，下行中心频点为 %.1f MHz，上行中心频点为 %.1f MHz", config->centerChannel[i], centerFreqDL, centerFreqUL);
			m_logOutput(logBuff);
		}

		// 设置仪表参数中的下行频率
		ret = m_pInstru->SetGeneratorFreq(centerFreqDL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表参数中的下行功率频率失败！频率为 %.1f MHz", centerFreqDL);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表参数中的下行功率频率成功！频率为 %.1f MHz", centerFreqDL);
			m_logOutput(logBuff);
		}

		// 设置仪表下行线损
		ret = m_pInstru->SetGeneratorLoss(config->centerFreqPathLossDL[i]);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表参数中的下行线损失败！线损为 %.1f dB", config->centerFreqPathLossDL[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表参数中的下行线损成功！线损为 %.1f dB", config->centerFreqPathLossDL[i]);
			m_logOutput(logBuff);
		}

		// 循环测试中心频点不同接收机增益状态下的接收机增益
		for (int j = 0; j < RX_GAIN_NUM; j++)
		{
			// 不同接收增益状态描述
			string rxGainMode = "";				// 接收机增益状态描述字符串，仅用于log输出更加直观
			switch (j)
			{
			case RX_GAIN_LOW:
			{
				rxGainMode = "低增益状态";
				sprintf_s(logBuff, "正在进行低增益状态的AGC校准......");
				m_logOutput(logBuff);
				break;
			}
			case RX_GAIN_MID:
			{
				rxGainMode = "中增益状态";
				sprintf_s(logBuff, "正在进行中增益状态的AGC校准......");
				m_logOutput(logBuff);
				break;
			}
			case RX_GAIN_HIGH:
			{
				rxGainMode = "高增益状态";
				sprintf_s(logBuff, "正在进行高增益状态的AGC校准......");
				m_logOutput(logBuff);
				break;
			}
			default:
				break;
			}

			// 设置仪表参数中的下行功率
			ret = m_pInstru->SetGeneratorPower(config->agcPower[j]);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "设置仪表参数中的下行功率失败！功率为 %.1f dBm", config->agcPower[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "设置仪表参数中的下行功率成功！功率为 %.1f dBm", config->agcPower[j]);
				m_logOutput(logBuff);
			}

			// 设置接收机下行频点
			if (m_pDUT->SetReceiverFrequency(centerFreqDL))
			{
				sprintf_s(logBuff, "设置UE接收机频点成功");
				m_logOutput(logBuff);
			}
			else
			{
				sprintf_s(logBuff, "设置UE接收机频点失败");
				m_logOutput(logBuff);
				return FAIL;
			}

			// 设置UE接收机增益状态
			if (m_pDUT->SetReceiverGain(j))
			{
				sprintf_s(logBuff, "设置UE接收机的增益状态成功");
				m_logOutput(logBuff);
			}
			else
			{
				sprintf_s(logBuff, "设置UE接收机的增益状态失败");
				m_logOutput(logBuff);
				return FAIL;
			}

			// 从UE侧获取接收机捕获的信号强度，rssi和功率的换算关系需要物理层给出，这里只是给出一个范例
			rxGain[j] = (int)rssi[j] - config->agcPower[j];

			// 将接收机增益值保存进码表
			nvTable.info[nvIndex].agc.agcGain[j] = (signed char)rxGain[j];
			sprintf_s(logBuff, "接收机增益值为 %d！接收机增益状态为 %s", rxGain[i], rxGainMode);
			m_logOutput(logBuff);
		}
	}
	return ret;
}

/********************************************************************
* Function：RFCCal
* Description: rx gain frequency compensation calibration
* Input:
* NurRFNVCal nvTable: nv table
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History：
Name	Date		Contents Reason
Jason	2019-3-28	Add rx gain frequency compensation calibration
.
.
********************************************************************/
int NurBLCal::RFCCal(AGCConf* config)
{
	NurRFNVCal nvTable;
	int ret = 0;											// 校准结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log打印内容

	int rssi[RX_GAIN_NUM] = { 0 };							// UE在不同接收机增益状态下获取到的接收机信号能量值
	int rxGain[RX_GAIN_NUM] = { 0 };						// 根据UE获取的接收机能量值，换算出具体的功率，再减去下行输入信号，得到接收机增益
	double agcSlope_High[BAND_NUM] = { 0 };					// 高频点接收机增益补偿斜率
	double agcSlope_Low[BAND_NUM] = { 0 };					// 低频点接收机增益补偿斜率


	// 首先判定仪表类对象指针和DUT类对象指针，如果类对象指针不为空，继续校准
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "仪表类对象指针为空，无法调用仪表库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "终端通信类对象指针为空，无法调用终端通信库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 初始化接收机流程
	ret = InitReceiver(config);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "初始化终端接收机流程失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "初始化终端接收机流程成功！");
		m_logOutput(logBuff);
	}

	// 循环开始每个待测频段的校准
	for (int i = 0; i < BAND_NUM; i++)
	{
		// 按照3GPP标准，一共18个待测频段，依次分别为1、2、3、5、8、11、12、13、17、18、19、20、25、26、28、31、66、70
		// 这18个频段对应到数组bandCalEnable[]的下标从0-17
		// 由于出现了频点范围的重复，band12包含band17，band25包含band2，band26包含band5/18/19，因此NV码表去除了band17/2/5/18/19
		// NV码表一共13个频段，分别是1、3、8、11、12、13、20、25、26、28、31、66、70
		// 这13个频段对应到NV码表的数组NurRFNVCal.Info[]的下标从0-12
		// 因此，如果band12/17互斥，band2/25互斥，band5/18/19互斥，在配置界面中，互斥频段中的一个频段被使能，则其他频段被禁止
		// 并且在校准中，band17会自动映射到band12，band2会自动映射到band25，band2/18/19会自动映射到band26
		/*		Index		Band			UL Frequencies				DL frequencies				NV Index

		0			1				1920 - 1980 MHz				2110 - 2170 MHz				0
		1			2				1850 - 1910 MHz				1930 - 1990 MHz				7
		2			3				1710 - 1785 MHz				1805 - 1880 MHz				1
		3			5				824 - 849MHz				869 - 894MHz				8
		4			8				880 - 915 MHz				925 - 960 MHz				2
		5			11				1427.9 - 1447.9 MHz			1475.9 - 1495.9 MHz			3
		6			12				699 - 716 MHz				729 - 746 MHz				4
		7			13				777 - 787 MHz				746 - 756 MHz				5
		8			17				704 - 716 MHz				734 - 746 MHz				4
		9			18				815 - 830 MHz				875 - 890 MHz				8
		10			19				830 - 845 MHz				875 - 890 MHz				8
		11			20				832 - 862 MHz				791 - 821 MHz				6
		12			25				1850 - 1915 MHz				1930 - 1995 MHz				7
		13			26				814 - 849 MHz				859 - 894 MHz				8
		14			28				703 - 748 MHz				785 - 803 MHz				9
		15			31				452.5 – 457.5 MHz			462.5 – 467.5 MHz			10
		17			66				1710 - 1780 MHz				2110 - 2200 MHz				11
		18			70				1695 - 1710 MHz				1995 - 2020 MHz				12
		*/
		// 首先判定待测频段是否被使能，如果没有使能，则直接跳转到下一个频段
		if (false == config->bandCalEnable[i])
		{
			continue;
		}

		// i就是上表中的index，现在需要映射到NV码表中对应的NV index，并得到该NV频段对应的最大频点和最小频点
		int nvIndex = 0;						// 待测频段在NV马表中对应的序号
		double freqDLMax = 0;					// 待测频段对应的NV码表频段的最大频点，用于频率补偿计算
		double freqDLMin = 0;					// 待测频段对应的NV码表频段的最小频点，用于频率补偿计算
		ret = NurCommonFunc::GetNVIndexFromBandIndex(i, nvIndex, freqDLMax, freqDLMin);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "根据频带的索引获取NV的索引，失败");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据频带的索引获取NV的索引，成功");
			m_logOutput(logBuff);
		}

		for (int j = 0; j < RX_GAIN_NUM; j++) {
			rxGain[j] = nvTable.info[nvIndex].agc.agcGain[j];
		}

		double centerFreqDL = 0;				// 待测频段下行中心频点
		double centerFreqUL = 0;				// 待测频段上行中心频点

		int rssiComp = 0;						// UE在补偿信道获取到的接收机信号能量值
		int rxGainComp = 0;						// 根据UE获取的补偿信道接收机能量值，换算出具体的功率，再减去下行输入信号，得到补偿信道接收机增益
		double compRssi[RX_GAIN_NUM] = { 0 };

		// 根据下行信道号获取对应的上下行频点
		ret = NurCommonFunc::GetFreqFromDLChan(config->centerChannel[i], centerFreqDL, centerFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "中心信道信道号为非法数据！信道号为 %d", config->centerChannel[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据下行信道号获取对应的上下行频点成功！下行信道号为 %d，下行中心频点为 %.1f MHz，上行中心频点为 %.1f MHz", config->centerChannel[i], centerFreqDL, centerFreqUL);
			m_logOutput(logBuff);
		}

		//----------------------------------------------------------------- 高频点频率补偿校准	---------------------------------------------//
		double compFreqDL = 0;					// 待测频段下行的某个频点
		double compFreqUL = 0;					// 待测频段上行的某个频点
		// 根据下行信道号获取对应的上下行频点
		ret = NurCommonFunc::GetFreqFromDLChan(config->compChannelHigh[i], compFreqDL, compFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "频率补偿信道信道号为非法数据！信道号为 %d", config->compChannelHigh[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据下行信道号获取对应的上下行频点成功！下行信道号为 %d，下行中心频点为 %.1f MHz，上行中心频点为 %.1f MHz", config->compChannelHigh[i], compFreqDL, compFreqUL);
			m_logOutput(logBuff);
		}

		// 从UE获取RSSI
		ret = GetRSSI(compFreqDL, config->compFreqPathLossHighDL[i], config->agcPower[RX_GAIN_NUM - 1], rssiComp);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "从UE获取RSSI失败！");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "从UE获取RSSI成功！RSSI为 %d", rssiComp);
			m_logOutput(logBuff);
		}

		// 从UE侧获取接收机捕获的信号强度，rssi和功率的换算关系需要物理层给出，这里只是给出一个范例
		rxGainComp = (int)rssiComp - config->agcPower[RX_GAIN_NUM - 1];

		// 计算接收机增益补偿斜率，单位是dB / 100kHz，由于AGC码表的精度是1dB，因此补偿值和AGC码表数组的下标是一致的
		agcSlope_High[i] = (rxGainComp - rxGain[RX_GAIN_NUM - 1]) / (config->compChannelHigh[i] - config->centerChannel[i]);
		for (int k = RFC_FREQ_NUM / 2; k < RFC_FREQ_NUM; k++)
		{
			nvTable.info[i].rfc.info[k].frequency = short((freqDLMax - centerFreqDL) * 10 * (k - double(RFC_FREQ_NUM / 2) + 1) / (RFC_FREQ_NUM / 2));
			nvTable.info[i].rfc.info[k].comp = char(nvTable.info[nvIndex].rfc.info[k].frequency * agcSlope_High[i]);
		}

		//----------------------------------------------------------------- 低频点频率补偿校准	---------------------------------------------//

		// 根据下行信道号获取对应的上下行频点
		ret = NurCommonFunc::GetFreqFromDLChan(config->compChannelLow[i], compFreqDL, compFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "频率补偿信道信道号为非法数据！信道号为 %d", config->compChannelLow[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "根据下行信道号获取对应的上下行频点成功！下行信道号为 %d，下行中心频点为 %.1f MHz，上行中心频点为 %.1f MHz", config->compChannelLow[i], compFreqDL, compFreqUL);
			m_logOutput(logBuff);
		}

		// 从UE获取RSSI
		ret = GetRSSI(compFreqDL, config->compFreqPathLossLowDL[i], config->agcPower[RX_GAIN_NUM - 1], rssiComp);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "从UE获取RSSI失败！");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "从UE获取RSSI成功！RSSI为 %d", rssiComp);
			m_logOutput(logBuff);
		}

		// 从UE侧获取接收机捕获的信号强度，rssi和功率的换算关系需要物理层给出，这里只是给出一个范例
		rxGainComp = (int)rssiComp - config->agcPower[RX_GAIN_NUM - 1];

		// 计算接收机增益补偿斜率，单位是dB / 100kHz，由于AGC码表的精度是1dB，因此补偿值和AGC码表数组的下标是一致的
		agcSlope_Low[i] = (rxGainComp - rxGain[RX_GAIN_NUM - 1]) / (config->compChannelLow[i] - config->centerChannel[i]);
		for (int k = 0; k < RFC_FREQ_NUM / 2; k++)
		{
			nvTable.info[nvIndex].tfc.info[k].frequency = short((freqDLMin - centerFreqDL) * 10 * (1 - double(k + 1) / (RFC_FREQ_NUM / 2)));
			nvTable.info[i].rfc.info[k].comp = char(nvTable.info[nvIndex].rfc.info[k].frequency * agcSlope_Low[i]);
		}
	}
	return ret;
}

/********************************************************************
* Function：InitTransmitter
* Description: Init instrument measurement and UE to make UE transmitter initialization successful
* Input:
* APCConf* : config
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History：
Name	Date		Contents Reason
Jason	2019-3-29	Add Iniransmitter initialization
.
.
********************************************************************/
int NurBLCal::InitTransmitter(APCConf* config)
{
	int ret = FAIL;											// 校准结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log打印内容

	// 初始化仪表配置参数
	ret = m_pInstru->InitInstrument();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "初始化仪表配置参数失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "初始化仪表配置参数成功！");
		m_logOutput(logBuff);
	}

	// 初始化UE发射机，参数频点、APC控制字和PA档位需要从config页面读取
	if (m_pDUT->OpenTransmitter(config->freq, config->paMode, config->initApcData))
	{
		sprintf_s(logBuff, "打开UE发射机成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "打开UE发射机失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	return PASS;
}

/********************************************************************
* Function：InitReceiver
* Description: Init instrument generator and UE to make UE receiver initialization successful
* Input:
* AGCConf* : config
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History：
Name	Date		Contents Reason
Jason	2019-3-29	Add receiver initialization
.
.
********************************************************************/
int NurBLCal::InitReceiver(AGCConf* config)
{
	int ret = 0;											// 校准结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log打印内容

	// 初始化仪表配置参数
	ret = m_pInstru->InitInstrument();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "初始化仪表配置参数失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "初始化仪表配置参数成功！");
		m_logOutput(logBuff);
	}

	// 设置仪表参数中的下行功率频点
	ret = m_pInstru->SetGeneratorFreq(config->freq);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表参数中的下行功率频点失败！频率为 %.1f MHz", config->freq);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表参数中的下行功率频点成功！频率为 %.1f MHz", config->freq);
		m_logOutput(logBuff);
	}

	// 设置仪表参数中的下行功率
	ret = m_pInstru->SetGeneratorPower(config->cellPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表参数中的下行功率失败！功率为 %.1f dBm", config->cellPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表参数中的下行功率成功！功率为 %.1f dBm", config->cellPower);
		m_logOutput(logBuff);
	}

	// 打开小区功率
	ret = m_pInstru->TurnOnGenerator();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "打开小区功率失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "打开小区功率成功！");
		m_logOutput(logBuff);
	}


	// 打开UE接收机，接收机频点和接收机增益状态从config页面中读取
	if (m_pDUT->OpenReceiver(config->freq, config->rxGainMode))
	{
		sprintf_s(logBuff, "打开UE接收机成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "打开UE接收机失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	return PASS;
}

/********************************************************************
* Function：GetRSSI
* Description: set instrument generator and UE to get RSSI
* Input:
* AGCConf* : config
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History：
Name	Date		Contents Reason
Jason	2019-3-30	Add function to get RSSI
.
.
********************************************************************/
int NurBLCal::GetRSSI(double compFreqDL, double pathLoss, double cellPower, int& rssi)
{
	int ret = 0;											// 校准结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log打印内容

	// 设置仪表参数中的下行功率频率
	ret = m_pInstru->SetGeneratorFreq(compFreqDL);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表参数中的下行功率频率失败！频率为 %.1f MHz", compFreqDL);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表参数中的下行功率频率成功！频率为 %.1f MHz", compFreqDL);
		m_logOutput(logBuff);
	}

	// 设置仪表下行线损
	ret = m_pInstru->SetGeneratorLoss(pathLoss);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表参数中的下行线损失败！线损为 %.1f dB", pathLoss);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表参数中的下行线损成功！线损为 %.1f dB", pathLoss);
		m_logOutput(logBuff);
	}

	// 设置仪表参数中的下行功率
	ret = m_pInstru->SetGeneratorPower(cellPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表参数中的下行功率失败！功率为 %.1f dBm", cellPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表参数中的下行功率成功！功率为 %.1f dBm", cellPower);
		m_logOutput(logBuff);
	}

	// 设置接收机下行频点
	if (m_pDUT->SetReceiverFrequency(compFreqDL))
	{
		sprintf_s(logBuff, "设置UE接受频点成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "设置UE接受频点失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 设置UE接收机增益状态
	if (m_pDUT->SetReceiverGain(RX_GAIN_NUM - 1))
	{
		sprintf_s(logBuff, "设置UE接收机增益状态成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "设置UE接收机增益状态失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 获取RSSI


	return PASS;
}