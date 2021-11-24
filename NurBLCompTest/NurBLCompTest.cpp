#include "stdafx.h"
#include "stdio.h"
#include "NurBLCompTest.h"
#include "comdef.h"



NurBLTest::NurBLTest()
{
}


NurBLTest::~NurBLTest()
{
}


int NurBLTest::SetLogPtr(SEND_LOG_TO_UI logOutputEX)
{
	m_logOutput = logOutputEX;
	return PASS;
}



int NurBLTest::InitDUTAPI()
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

int NurBLTest::ReleaseDUTAPI()
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

int NurBLTest::InitInstrumentAPI()
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

int NurBLTest::ReleaseInstrumentAPI()
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

int NurBLTest::OpenSerialPortEX(int baudRate, const wchar_t* serialPort)
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

int NurBLTest::CloseSerialPortEX()
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

int NurBLTest::OpenInstrument(const wchar_t* intrumentIP)
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

int NurBLTest::ReleaseInstrument()
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

int NurBLTest::NSFTConn(NSFTConf* config)
{
	int ret = FAIL;					// 仪表操作结果
	char logBuff[LOG_BUFF_LEN] = { '\0' };		// log打印内容

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
	ret = m_pInstru->InitNSFT();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "非信令综测初始化仪表配置参数失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "非信令综测初始化仪表配置参数成功！");
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
	if (m_pDUT->EnterNSTMode())
	{
		sprintf_s(logBuff, "UE进入非信令综测模式成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "UE进入非信令综测模式失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	// NSFT下行同步
	if (m_pDUT->NSTDownlinkSync((unsigned short)(config->freq * 10), config->mcs, config->toneNum, config->toneStart, config->subCarrierSpacing, config->cellPower))
	{
		sprintf_s(logBuff, "NSFT下行同步成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "NSFT下行同步失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	return ret;
}


int NurBLTest::NSFTHandover(int band, int channel)
{
	int ret = FAIL;					// 仪表操作结果
	char logBuff[LOG_BUFF_LEN];		// log打印内容

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

	// PC通知UE准备切换频点
	if (m_pDUT->NSTNoticeUEPrepareSwitchFrequency())
	{
		sprintf_s(logBuff, "PC通知UE准备切换频点成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "PC通知UE准备切换频点失败");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 设置仪表切换的频点
	double FreqDL = 0;				// 待切换下行频点
	double FreqUL = 0;				// 待切换上行频点
	ret = NurCommonFunc::GetFreqFromDLChan(channel, FreqDL, FreqUL);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "中心信道信道号为非法数据！信道号为 %d", channel);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "根据下行信道号获取对应的上下行频点成功！下行信道号为 %d，下行中心频点为 %.1f MHz，上行中心频点为 %.1f MHz", channel, FreqDL, FreqUL);
		m_logOutput(logBuff);
	}

	// 设置仪表待切换的上下行频点
	ret = m_pInstru->NSFTHandover(FreqDL, FreqUL);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表待切换的上下行频点失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表待切换的上下行频点成功！");
		m_logOutput(logBuff);
	}

	// PC通知UE切换到对应的band和channel
	if (m_pDUT->NSTSwitchChannel((unsigned char)band, channel))
	{
		sprintf_s(logBuff, "PC通知UE准备切换频点成功");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "PC通知UE准备切换频点失败");
		m_logOutput(logBuff);
		return FAIL;
	}
}

int NurBLTest::NSFTTXMeasurement(NSFTConf* config)
{
	TXResult m_txResult;
	int ret = FAIL;					// 仪表操作结果
	char logBuff[LOG_BUFF_LEN];		// log打印内容

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

	// 设置仪表上行测量参数, 仅仅测量功率
	ret = m_pInstru->NSFTSetMeasurement(config->mcs, config->toneNum, config->toneStart, config->subCarrierSpacing, true);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表上行测量参数(仅测量功率)失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表上行测量参数(仅测量功率)成功！");
		m_logOutput(logBuff);
	}

	// NST设置上行最小功率，需要补齐，DUT没有

	// 完成TX测量
	ret = m_pInstru->NSFTTXMeasurement();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "TX指标测量失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "TX指标测量成功！");
		m_logOutput(logBuff);
	}

	// 获取上行最小功率
	ret = m_pInstru->ReadPower(m_txResult.minPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "获取上行最小功率失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "获取上行最小功率成功！minPower = %.1f", m_txResult.minPower);
		m_logOutput(logBuff);
	}

	// NST设置上行最大功率，需要补齐，DUT没有
	
	// 设置仪表上行测量参数，测量功率、频率误差、ACLR、EVM、OBW、SEM
	ret = m_pInstru->NSFTSetMeasurement(config->mcs, config->toneNum, config->toneStart, config->subCarrierSpacing);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表上行测量参数(测量功率、频率误差、ACLR、EVM、OBW、SEM)失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表上行测量参数(测量功率、频率误差、ACLR、EVM、OBW、SEM)成功！");
		m_logOutput(logBuff);
	}

	// 完成TX测量
	ret = m_pInstru->NSFTTXMeasurement();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "TX指标测量失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "TX指标测量成功！");
		m_logOutput(logBuff);
	}

	// 获取EVM和FreqErr
	ret = m_pInstru->ReadMODulation(m_txResult.evm, m_txResult.freqErr);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "获取EVM和FreqErr失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "获取EVM和FreqErr成功！evm = %.2f, freqErr = %.1f", m_txResult.evm, m_txResult.freqErr);
		m_logOutput(logBuff);
	}

	// 获取上行最大功率、SEM和OBW
	ret = m_pInstru->ReadSEMask(m_txResult.maxPower, m_txResult.sem, m_txResult.obw);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "获取上行最大功率、SEM和OBW失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "获取上行最大功率、SEM和OBW成功！maxPower = %.1f, obw = %.2f, SEM测量结果 = %s", m_txResult.maxPower, m_txResult.obw, m_txResult.sem? "pass":"fail");
		m_logOutput(logBuff);
	}

	// 获取ACLR
	ret = m_pInstru->ReadACLR(m_txResult.alcr);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "获取ACLR失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "获取ACLR成功！ACLR = %.2f, %.2f, %.2f, %.2f", m_txResult.alcr[0], m_txResult.alcr[1], m_txResult.alcr[2], m_txResult.alcr[3]);
		m_logOutput(logBuff);
	}

	ret = CheckTXMeasurementResult(config, m_txResult);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "TX非信令测试FAIL！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "TX非信令测试PASS！");
		m_logOutput(logBuff);
	}

	return ret;
}

int NurBLTest::NSFTRXMeasurement(NSFTConf* config)
{
	RXResult rxResult;
	int ret = FAIL;					// 仪表操作结果
	char logBuff[LOG_BUFF_LEN];		// log打印内容

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

	// 设置仪表下行功率, 测量最大输入电平
	ret = m_pInstru->SetGeneratorPower(config->RXMaxPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表小区功率失败！小区功率 = %.1fdBm", config->RXMaxPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表小区功率成功！小区功率 = %.1fdBm", config->RXMaxPower);
		m_logOutput(logBuff);
	}

	//从终端获取RSRP值，并换算成小区功率

	// 设置仪表下行功率, 保证终端不会失同步
	ret = m_pInstru->SetGeneratorPower(config->cellPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表小区功率失败！小区功率 = %.1fdBm", config->cellPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表小区功率成功！小区功率 = %.1fdBm", config->cellPower);
		m_logOutput(logBuff);
	}

	for (int i = 0; i < RX_GAIN_NUM; i++)
	{
		// 设置仪表下行功率, 测量RSRP
		ret = m_pInstru->SetGeneratorPower(config->RXRSCPPower[i]);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "设置仪表小区功率失败！小区功率 = %.1fdBm", config->RXRSCPPower[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "设置仪表小区功率成功！小区功率 = %.1fdBm", config->RXRSCPPower[i]);
			m_logOutput(logBuff);
		}

		//从终端获取RSRP值
	}

	// 设置仪表下行功率, 保证终端不会失同步
	ret = m_pInstru->SetGeneratorPower(config->cellPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表小区功率失败！小区功率 = %.1fdBm", config->cellPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表小区功率成功！小区功率 = %.1fdBm", config->cellPower);
		m_logOutput(logBuff);
	}

	// 设置手机参数，需要设置subFrameNumber和timeout

	// 设置仪表小区功率，测试BLER
	ret = m_pInstru->SetGeneratorPower(config->RXBlerPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "设置仪表小区功率失败！小区功率 = %.1fdBm", config->RXBlerPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "设置仪表小区功率成功！小区功率 = %.1fdBm", config->RXBlerPower);
		m_logOutput(logBuff);
	}

	// 从手机获取totalBlock和errorBlock


	// 判断下行接收测量结果
	ret = CheckRXMeasurementResult(config, rxResult);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "RX非信令测试FAIL！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "RX非信令测试PASS！");
		m_logOutput(logBuff);
	}

	return ret;
}

int NurBLTest::CheckTXMeasurementResult(NSFTConf* config, TXResult m_txResult)
{
	int ret = FAIL;	
	double txFreq = 0;
	char logBuff[LOG_BUFF_LEN];		// log打印内容

	// 首先判定仪表类对象指针和DUT类对象指针，如果类对象指针不为空，继续校准
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "仪表类对象指针为空，无法调用仪表库函数！");
		m_logOutput(logBuff);
		return FAIL;
	}

	// 获取当前的上行测量频点
	ret = m_pInstru->ReadTXFreq(txFreq);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "获取当前的上行测量频点失败！");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "获取当前的上行测量频点成功！上行测量频点为 %.1f", txFreq);
		m_logOutput(logBuff);
	}

	// 首先判定仪表类对象指针和DUT类对象指针，如果类对象指针不为空，继续校准
	for (int i = 0; i < TXTESTCASE_NUM; i++)
	{
		switch (i) {
		case MAXPOWER:
			if (config->TXTestCaseEnable[MAXPOWER])
			{
				if (abs(m_txResult.maxPower - config->TXMaxPower) < config->TXMaxPowerLimit)
				{
					sprintf_s(logBuff, "上行最大功率PASS！最大功率 = %.1fdBm, 最大功率范围%.1f-%.1fdBm", m_txResult.maxPower, config->TXMaxPower - config->TXMaxPowerLimit, config->TXMaxPower + config->TXMaxPowerLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "上行最大功率FAIL！最大功率 = %.1fdBm, 最大功率范围%.1f-%.1fdBm", m_txResult.maxPower, config->TXMaxPower - config->TXMaxPowerLimit, config->TXMaxPower + config->TXMaxPowerLimit);
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		case MINPOWER:
			if (config->TXTestCaseEnable[MINPOWER])
			{
				if (m_txResult.minPower < config->TXMinPowerLimit)
				{
					sprintf_s(logBuff, "上行最小功率PASS！最小功率 = %.1fdBm, 最小功率上限%.1fdBm", m_txResult.minPower, config->TXMinPowerLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "上行最小功率FAIL！最小功率 = %.1fdBm, 最小功率上限%.1fdBm", m_txResult.minPower, config->TXMinPowerLimit);
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		case FREQERR:
			if (config->TXTestCaseEnable[FREQERR])
			{
				if (abs(m_txResult.freqErr) < txFreq * config->FreqErrLimit)
				{
					sprintf_s(logBuff, "频率误差PASS！频率误差 = %.1fHz, 频率误差上限%.1fHz", m_txResult.freqErr, config->FreqErrLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "频率误差FAIL！频率误差 = %.1fHz, 频率误差上限%.1fHz", m_txResult.freqErr, config->FreqErrLimit);
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		case EVM:
			if (config->TXTestCaseEnable[EVM])
			{
				if (m_txResult.evm < config->EVMLimit)
				{
					sprintf_s(logBuff, "EVM PASS！EVM = %.1f, EVM上限%.1f", m_txResult.evm, config->EVMLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "EVM FAIL！EVM = %.1f, EVM上限%.1f", m_txResult.evm, config->EVMLimit);
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		case ACLR:
			if (config->TXTestCaseEnable[ACLR])
			{
				if ((m_txResult.alcr[0] < config->ACLRLimit[0]) && (m_txResult.alcr[1] < config->ACLRLimit[0]) && (m_txResult.alcr[2] < config->ACLRLimit[1]) && (m_txResult.alcr[3] < config->ACLRLimit[1]))
				{
					sprintf_s(logBuff, "ACLR PASS！");
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "ACLR FAIL！");
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		case SEM:
			if (config->TXTestCaseEnable[SEM])
			{
				if (m_txResult.sem)
				{
					sprintf_s(logBuff, "SEM PASS！");
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "SEM FAIL！");
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		case OBW:
			if (config->TXTestCaseEnable[OBW])
			{
				if (m_txResult.obw < config->OBWLimit)
				{
					sprintf_s(logBuff, "OBW PASS！OBW = %.1f, OBW上限%.1f", m_txResult.obw, config->OBWLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "OBW FAIL！OBW = %.1f, OBW上限%.1f", m_txResult.obw, config->OBWLimit);
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		default:
			return -1;
			break;
		}
	}

	return ret;
}

int NurBLTest::CheckRXMeasurementResult(NSFTConf* config, RXResult rxResult)
{
	int ret = FAIL;
	char logBuff[LOG_BUFF_LEN];		// log打印内容

	
	// 首先判定仪表类对象指针和DUT类对象指针，如果类对象指针不为空，继续校准
	for (int i = 0; i < RXTESTCASE_NUM; i++)
	{
		switch (i) {
		case MAXINPUT:
			if (config->RXTestCaseEnable[MAXINPUT])
			{
				if (rxResult.maxInput < config->MaxInputLimit)
				{
					sprintf_s(logBuff, "最大输入电平PASS！最大输入电平 = %.1fdBm, 最大输入电平门限 = %.1fdBm", rxResult.maxInput, config->MaxInputLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "最大输入电平FAIL！最大输入电平 = %.1fdBm, 最大输入电平门限 = %.1fdBm", rxResult.maxInput, config->MaxInputLimit);
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		case RSRP:
			if (config->TXTestCaseEnable[RSRP])
			{
				for (int j = 0; j < RX_GAIN_NUM; j++)
				{
					if (abs(rxResult.rsrp[j] - config->RXRSCPPower[j]) < config->RXRSCPLimit[j])
					{
						sprintf_s(logBuff, "RSRP PASS！RSRP = %.1fdBm, 范围%.1fdBm - %.1fdBm", rxResult.rsrp[j], config->RXRSCPPower[j] + config->RXRSCPLimit[j], config->RXRSCPPower[j] - config->RXRSCPLimit[j]);
						m_logOutput(logBuff);
					}
					else
					{
						sprintf_s(logBuff, "RSRP FAIL！RSRP = %.1fdBm, 范围%.1fdBm - %.1fdBm", rxResult.rsrp[j], config->RXRSCPPower[j] + config->RXRSCPLimit[j], config->RXRSCPPower[j] - config->RXRSCPLimit[j]);
						m_logOutput(logBuff);
						return -1;
					}
				}
			}
			break;
		case BLER:
			if (config->RXTestCaseEnable[BLER])
			{
				if ((double)(rxResult.errorBlock) * 100 / rxResult.totalBlock < config->BLERLimit)
				{
					sprintf_s(logBuff, "BLER PASS！errorBlock = %d, totalBlock = %d, BLER门限 = %.1f%", rxResult.errorBlock, rxResult.totalBlock, config->BLERLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "BLER FAIL！errorBlock = %d, totalBlock = %d, BLER门限 = %.1f%", rxResult.errorBlock, rxResult.totalBlock, config->BLERLimit);
					m_logOutput(logBuff);
					return -1;
				}
			}
			break;
		default:
			return -1;
			break;
		}
	}

	return ret;
}