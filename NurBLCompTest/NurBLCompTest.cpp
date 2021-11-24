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

			sprintf_s(logBuff, "��ʼ��DUT��APIʧ��");
			m_logOutput(logBuff);
			return FAIL;
		}
		else
		{
			m_pDUT = lpFunc();

			sprintf_s(logBuff, "��ʼ��DUT��API�ɹ�");
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
	sprintf_s(logBuff, "�ͷ�DUT��API�ɹ�");
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
			sprintf_s(logBuff, "��ʼ���Ǳ��APIʧ��");
			m_logOutput(logBuff);
			return FAIL;
		}
		else
		{
			m_pInstru = lpFunc();
			sprintf_s(logBuff, "��ʼ���Ǳ��API�ɹ�");
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
	sprintf_s(logBuff, "�ͷ��Ǳ��API�ɹ�");
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
		sprintf_s(logBuff, "�򿪴��ڳɹ�");
		m_logOutput(logBuff);
		ret = PASS;
	}
	else
	{
		sprintf_s(logBuff, "�򿪴���ʧ��");
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
		sprintf_s(logBuff, "�رմ��ڳɹ�");
		m_logOutput(logBuff);
		ret = PASS;
	}
	else
	{
		sprintf_s(logBuff, "�رմ���ʧ��");
		m_logOutput(logBuff);
		ret = FAIL;
	}
	return ret;
}

int NurBLTest::OpenInstrument(const wchar_t* intrumentIP)
{
	int ret = FAIL;					// �Ǳ�������
	char logBuff[LOG_BUFF_LEN] = { '\0' };		// log��ӡ����

												// ����Ǳ������ָ�벻Ϊ�գ����Ǳ�������ӣ���ȡ�Ǳ�������
	if (NULL != m_pInstru)
	{
		// ���Ǳ�������ӣ���ȡ�Ǳ�������
		ret = m_pInstru->OpenInstru((_bstr_t)intrumentIP);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�ʧ�ܣ��Ǳ�IP = %ws", intrumentIP);
			m_logOutput(logBuff);
			ret = FAIL;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�ɹ����Ǳ�IP = %s", intrumentIP);
			m_logOutput(logBuff);
			ret = PASS;
		}
	}
	else
	{
		sprintf_s(logBuff, "�Ǳ������ָ��Ϊ�գ��޷������Ǳ�⺯����");
		m_logOutput(logBuff);
		ret = FAIL;
	}
	return ret;
}

int NurBLTest::ReleaseInstrument()
{
	int ret = FAIL;					// �Ǳ�������
	char logBuff[LOG_BUFF_LEN] = { '\0' };		// log��ӡ����

												// ����Ǳ������ָ�벻Ϊ�գ������Ǳ�⺯���ͷ��Ǳ���Դ
	if (NULL != m_pInstru)
	{
		ret = m_pInstru->CloseInstru();
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�ͷ��Ǳ���Դʧ�ܣ�");
			m_logOutput(logBuff);
			ret = FAIL;
		}
		else
		{
			sprintf_s(logBuff, "�ͷ��Ǳ���Դʧ�ܣ�");
			m_logOutput(logBuff);
			ret = PASS;
		}
	}
	else
	{
		sprintf_s(logBuff, "�Ǳ������ָ��Ϊ�գ��޷������Ǳ�⺯����");
		m_logOutput(logBuff);
		ret = FAIL;
	}

	return ret;
}

int NurBLTest::NSFTConn(NSFTConf* config)
{
	int ret = FAIL;					// �Ǳ�������
	char logBuff[LOG_BUFF_LEN] = { '\0' };		// log��ӡ����

	// �����ж��Ǳ������ָ���DUT�����ָ�룬��������ָ�벻Ϊ�գ�����У׼
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "�Ǳ������ָ��Ϊ�գ��޷������Ǳ�⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "�ն�ͨ�������ָ��Ϊ�գ��޷������ն�ͨ�ſ⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}

	// ��ʼ���Ǳ����ò���
	ret = m_pInstru->InitNSFT();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�������۲��ʼ���Ǳ����ò���ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�������۲��ʼ���Ǳ����ò����ɹ���");
		m_logOutput(logBuff);
	}

	// �����Ǳ�����е����й���Ƶ��
	ret = m_pInstru->SetGeneratorFreq(config->freq);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�����е����й���Ƶ��ʧ�ܣ�Ƶ��Ϊ %.1f MHz", config->freq);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�����е����й���Ƶ��ɹ���Ƶ��Ϊ %.1f MHz", config->freq);
		m_logOutput(logBuff);
	}

	// �����Ǳ�����е����й���
	ret = m_pInstru->SetGeneratorPower(config->cellPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�����е����й���ʧ�ܣ�����Ϊ %.1f dBm", config->cellPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�����е����й��ʳɹ�������Ϊ %.1f dBm", config->cellPower);
		m_logOutput(logBuff);
	}

	// ��С������
	ret = m_pInstru->TurnOnGenerator();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��С������ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��С�����ʳɹ���");
		m_logOutput(logBuff);
	}

	// ��UE���ջ������ջ�Ƶ��ͽ��ջ�����״̬��configҳ���ж�ȡ
	if (m_pDUT->EnterNSTMode())
	{
		sprintf_s(logBuff, "UE����������۲�ģʽ�ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "UE����������۲�ģʽʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	// NSFT����ͬ��
	if (m_pDUT->NSTDownlinkSync((unsigned short)(config->freq * 10), config->mcs, config->toneNum, config->toneStart, config->subCarrierSpacing, config->cellPower))
	{
		sprintf_s(logBuff, "NSFT����ͬ���ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "NSFT����ͬ��ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	return ret;
}


int NurBLTest::NSFTHandover(int band, int channel)
{
	int ret = FAIL;					// �Ǳ�������
	char logBuff[LOG_BUFF_LEN];		// log��ӡ����

	// �����ж��Ǳ������ָ���DUT�����ָ�룬��������ָ�벻Ϊ�գ�����У׼
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "�Ǳ������ָ��Ϊ�գ��޷������Ǳ�⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "�ն�ͨ�������ָ��Ϊ�գ��޷������ն�ͨ�ſ⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}

	// PC֪ͨUE׼���л�Ƶ��
	if (m_pDUT->NSTNoticeUEPrepareSwitchFrequency())
	{
		sprintf_s(logBuff, "PC֪ͨUE׼���л�Ƶ��ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "PC֪ͨUE׼���л�Ƶ��ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	// �����Ǳ��л���Ƶ��
	double FreqDL = 0;				// ���л�����Ƶ��
	double FreqUL = 0;				// ���л�����Ƶ��
	ret = NurCommonFunc::GetFreqFromDLChan(channel, FreqDL, FreqUL);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����ŵ��ŵ���Ϊ�Ƿ����ݣ��ŵ���Ϊ %d", channel);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "���������ŵ��Ż�ȡ��Ӧ��������Ƶ��ɹ��������ŵ���Ϊ %d����������Ƶ��Ϊ %.1f MHz����������Ƶ��Ϊ %.1f MHz", channel, FreqDL, FreqUL);
		m_logOutput(logBuff);
	}

	// �����Ǳ���л���������Ƶ��
	ret = m_pInstru->NSFTHandover(FreqDL, FreqUL);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ���л���������Ƶ��ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ���л���������Ƶ��ɹ���");
		m_logOutput(logBuff);
	}

	// PC֪ͨUE�л�����Ӧ��band��channel
	if (m_pDUT->NSTSwitchChannel((unsigned char)band, channel))
	{
		sprintf_s(logBuff, "PC֪ͨUE׼���л�Ƶ��ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "PC֪ͨUE׼���л�Ƶ��ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}
}

int NurBLTest::NSFTTXMeasurement(NSFTConf* config)
{
	TXResult m_txResult;
	int ret = FAIL;					// �Ǳ�������
	char logBuff[LOG_BUFF_LEN];		// log��ӡ����

	// �����ж��Ǳ������ָ���DUT�����ָ�룬��������ָ�벻Ϊ�գ�����У׼
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "�Ǳ������ָ��Ϊ�գ��޷������Ǳ�⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "�ն�ͨ�������ָ��Ϊ�գ��޷������ն�ͨ�ſ⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}

	// �����Ǳ����в�������, ������������
	ret = m_pInstru->NSFTSetMeasurement(config->mcs, config->toneNum, config->toneStart, config->subCarrierSpacing, true);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ����в�������(����������)ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ����в�������(����������)�ɹ���");
		m_logOutput(logBuff);
	}

	// NST����������С���ʣ���Ҫ���룬DUTû��

	// ���TX����
	ret = m_pInstru->NSFTTXMeasurement();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "TXָ�����ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "TXָ������ɹ���");
		m_logOutput(logBuff);
	}

	// ��ȡ������С����
	ret = m_pInstru->ReadPower(m_txResult.minPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ȡ������С����ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ȡ������С���ʳɹ���minPower = %.1f", m_txResult.minPower);
		m_logOutput(logBuff);
	}

	// NST������������ʣ���Ҫ���룬DUTû��
	
	// �����Ǳ����в����������������ʡ�Ƶ����ACLR��EVM��OBW��SEM
	ret = m_pInstru->NSFTSetMeasurement(config->mcs, config->toneNum, config->toneStart, config->subCarrierSpacing);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ����в�������(�������ʡ�Ƶ����ACLR��EVM��OBW��SEM)ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ����в�������(�������ʡ�Ƶ����ACLR��EVM��OBW��SEM)�ɹ���");
		m_logOutput(logBuff);
	}

	// ���TX����
	ret = m_pInstru->NSFTTXMeasurement();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "TXָ�����ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "TXָ������ɹ���");
		m_logOutput(logBuff);
	}

	// ��ȡEVM��FreqErr
	ret = m_pInstru->ReadMODulation(m_txResult.evm, m_txResult.freqErr);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ȡEVM��FreqErrʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ȡEVM��FreqErr�ɹ���evm = %.2f, freqErr = %.1f", m_txResult.evm, m_txResult.freqErr);
		m_logOutput(logBuff);
	}

	// ��ȡ��������ʡ�SEM��OBW
	ret = m_pInstru->ReadSEMask(m_txResult.maxPower, m_txResult.sem, m_txResult.obw);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ȡ��������ʡ�SEM��OBWʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ȡ��������ʡ�SEM��OBW�ɹ���maxPower = %.1f, obw = %.2f, SEM������� = %s", m_txResult.maxPower, m_txResult.obw, m_txResult.sem? "pass":"fail");
		m_logOutput(logBuff);
	}

	// ��ȡACLR
	ret = m_pInstru->ReadACLR(m_txResult.alcr);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ȡACLRʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ȡACLR�ɹ���ACLR = %.2f, %.2f, %.2f, %.2f", m_txResult.alcr[0], m_txResult.alcr[1], m_txResult.alcr[2], m_txResult.alcr[3]);
		m_logOutput(logBuff);
	}

	ret = CheckTXMeasurementResult(config, m_txResult);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "TX���������FAIL��");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "TX���������PASS��");
		m_logOutput(logBuff);
	}

	return ret;
}

int NurBLTest::NSFTRXMeasurement(NSFTConf* config)
{
	RXResult rxResult;
	int ret = FAIL;					// �Ǳ�������
	char logBuff[LOG_BUFF_LEN];		// log��ӡ����

	// �����ж��Ǳ������ָ���DUT�����ָ�룬��������ָ�벻Ϊ�գ�����У׼
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "�Ǳ������ָ��Ϊ�գ��޷������Ǳ�⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}
	if (NULL == m_pDUT)
	{
		sprintf_s(logBuff, "�ն�ͨ�������ָ��Ϊ�գ��޷������ն�ͨ�ſ⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}

	// �����Ǳ����й���, ������������ƽ
	ret = m_pInstru->SetGeneratorPower(config->RXMaxPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�С������ʧ�ܣ�С������ = %.1fdBm", config->RXMaxPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�С�����ʳɹ���С������ = %.1fdBm", config->RXMaxPower);
		m_logOutput(logBuff);
	}

	//���ն˻�ȡRSRPֵ���������С������

	// �����Ǳ����й���, ��֤�ն˲���ʧͬ��
	ret = m_pInstru->SetGeneratorPower(config->cellPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�С������ʧ�ܣ�С������ = %.1fdBm", config->cellPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�С�����ʳɹ���С������ = %.1fdBm", config->cellPower);
		m_logOutput(logBuff);
	}

	for (int i = 0; i < RX_GAIN_NUM; i++)
	{
		// �����Ǳ����й���, ����RSRP
		ret = m_pInstru->SetGeneratorPower(config->RXRSCPPower[i]);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�С������ʧ�ܣ�С������ = %.1fdBm", config->RXRSCPPower[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�С�����ʳɹ���С������ = %.1fdBm", config->RXRSCPPower[i]);
			m_logOutput(logBuff);
		}

		//���ն˻�ȡRSRPֵ
	}

	// �����Ǳ����й���, ��֤�ն˲���ʧͬ��
	ret = m_pInstru->SetGeneratorPower(config->cellPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�С������ʧ�ܣ�С������ = %.1fdBm", config->cellPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�С�����ʳɹ���С������ = %.1fdBm", config->cellPower);
		m_logOutput(logBuff);
	}

	// �����ֻ���������Ҫ����subFrameNumber��timeout

	// �����Ǳ�С�����ʣ�����BLER
	ret = m_pInstru->SetGeneratorPower(config->RXBlerPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�С������ʧ�ܣ�С������ = %.1fdBm", config->RXBlerPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�С�����ʳɹ���С������ = %.1fdBm", config->RXBlerPower);
		m_logOutput(logBuff);
	}

	// ���ֻ���ȡtotalBlock��errorBlock


	// �ж����н��ղ������
	ret = CheckRXMeasurementResult(config, rxResult);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "RX���������FAIL��");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "RX���������PASS��");
		m_logOutput(logBuff);
	}

	return ret;
}

int NurBLTest::CheckTXMeasurementResult(NSFTConf* config, TXResult m_txResult)
{
	int ret = FAIL;	
	double txFreq = 0;
	char logBuff[LOG_BUFF_LEN];		// log��ӡ����

	// �����ж��Ǳ������ָ���DUT�����ָ�룬��������ָ�벻Ϊ�գ�����У׼
	if (NULL == m_pInstru)
	{
		sprintf_s(logBuff, "�Ǳ������ָ��Ϊ�գ��޷������Ǳ�⺯����");
		m_logOutput(logBuff);
		return FAIL;
	}

	// ��ȡ��ǰ�����в���Ƶ��
	ret = m_pInstru->ReadTXFreq(txFreq);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ȡ��ǰ�����в���Ƶ��ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ȡ��ǰ�����в���Ƶ��ɹ������в���Ƶ��Ϊ %.1f", txFreq);
		m_logOutput(logBuff);
	}

	// �����ж��Ǳ������ָ���DUT�����ָ�룬��������ָ�벻Ϊ�գ�����У׼
	for (int i = 0; i < TXTESTCASE_NUM; i++)
	{
		switch (i) {
		case MAXPOWER:
			if (config->TXTestCaseEnable[MAXPOWER])
			{
				if (abs(m_txResult.maxPower - config->TXMaxPower) < config->TXMaxPowerLimit)
				{
					sprintf_s(logBuff, "���������PASS������� = %.1fdBm, ����ʷ�Χ%.1f-%.1fdBm", m_txResult.maxPower, config->TXMaxPower - config->TXMaxPowerLimit, config->TXMaxPower + config->TXMaxPowerLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "���������FAIL������� = %.1fdBm, ����ʷ�Χ%.1f-%.1fdBm", m_txResult.maxPower, config->TXMaxPower - config->TXMaxPowerLimit, config->TXMaxPower + config->TXMaxPowerLimit);
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
					sprintf_s(logBuff, "������С����PASS����С���� = %.1fdBm, ��С��������%.1fdBm", m_txResult.minPower, config->TXMinPowerLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "������С����FAIL����С���� = %.1fdBm, ��С��������%.1fdBm", m_txResult.minPower, config->TXMinPowerLimit);
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
					sprintf_s(logBuff, "Ƶ�����PASS��Ƶ����� = %.1fHz, Ƶ���������%.1fHz", m_txResult.freqErr, config->FreqErrLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "Ƶ�����FAIL��Ƶ����� = %.1fHz, Ƶ���������%.1fHz", m_txResult.freqErr, config->FreqErrLimit);
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
					sprintf_s(logBuff, "EVM PASS��EVM = %.1f, EVM����%.1f", m_txResult.evm, config->EVMLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "EVM FAIL��EVM = %.1f, EVM����%.1f", m_txResult.evm, config->EVMLimit);
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
					sprintf_s(logBuff, "ACLR PASS��");
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "ACLR FAIL��");
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
					sprintf_s(logBuff, "SEM PASS��");
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "SEM FAIL��");
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
					sprintf_s(logBuff, "OBW PASS��OBW = %.1f, OBW����%.1f", m_txResult.obw, config->OBWLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "OBW FAIL��OBW = %.1f, OBW����%.1f", m_txResult.obw, config->OBWLimit);
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
	char logBuff[LOG_BUFF_LEN];		// log��ӡ����

	
	// �����ж��Ǳ������ָ���DUT�����ָ�룬��������ָ�벻Ϊ�գ�����У׼
	for (int i = 0; i < RXTESTCASE_NUM; i++)
	{
		switch (i) {
		case MAXINPUT:
			if (config->RXTestCaseEnable[MAXINPUT])
			{
				if (rxResult.maxInput < config->MaxInputLimit)
				{
					sprintf_s(logBuff, "��������ƽPASS����������ƽ = %.1fdBm, ��������ƽ���� = %.1fdBm", rxResult.maxInput, config->MaxInputLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "��������ƽFAIL����������ƽ = %.1fdBm, ��������ƽ���� = %.1fdBm", rxResult.maxInput, config->MaxInputLimit);
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
						sprintf_s(logBuff, "RSRP PASS��RSRP = %.1fdBm, ��Χ%.1fdBm - %.1fdBm", rxResult.rsrp[j], config->RXRSCPPower[j] + config->RXRSCPLimit[j], config->RXRSCPPower[j] - config->RXRSCPLimit[j]);
						m_logOutput(logBuff);
					}
					else
					{
						sprintf_s(logBuff, "RSRP FAIL��RSRP = %.1fdBm, ��Χ%.1fdBm - %.1fdBm", rxResult.rsrp[j], config->RXRSCPPower[j] + config->RXRSCPLimit[j], config->RXRSCPPower[j] - config->RXRSCPLimit[j]);
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
					sprintf_s(logBuff, "BLER PASS��errorBlock = %d, totalBlock = %d, BLER���� = %.1f%", rxResult.errorBlock, rxResult.totalBlock, config->BLERLimit);
					m_logOutput(logBuff);
				}
				else
				{
					sprintf_s(logBuff, "BLER FAIL��errorBlock = %d, totalBlock = %d, BLER���� = %.1f%", rxResult.errorBlock, rxResult.totalBlock, config->BLERLimit);
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