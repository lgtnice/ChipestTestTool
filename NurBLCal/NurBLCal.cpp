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

int NurBLCal::ReleaseDUTAPI()
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

int NurBLCal::ReleaseInstrumentAPI()
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

int NurBLCal::OpenInstrument(const wchar_t* intrumentIP)
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

int NurBLCal::ReleaseInstrument()
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

//-------------------------------------------------------У׼����----------------------------------//


/********************************************************************
* Function��AFCCal
* Description: frequency calibration
* Input:
* SEND_LOG_TO_UI m_logOutput: the contents of log
* NurRFNVCal nvTable: nv table
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History��
Name	Date		Contents Reason
Jason	2019-1-21	Add frequency calibration
.
.
********************************************************************/
int NurBLCal::AFCCal(AFCConf* config)
{
	NurRFNVCal nvTable;

	/*----------------------------------------�����ָ���---------------------------------------------*/

	int ret = PASS;					// У׼���
	char logBuff[LOG_BUFF_LEN] = { '\0' };		// log��ӡ����

	int afcData = 0;				// Ƶ�ʿ����ֵ�ƫ��ֵ����ֵ��У׼�����п�������Ƶ�����Ҫ�󣬱�����AFC�����
	int slope = 0;					// Ƶ�ʿ��Ƶ�б�ʣ���ֵΪƵ�� - ������֮��ı仯��ϵ��������AFC�����

	double freqErr = 0;				// Ƶ������λHz�����Ǳ��ȡ�����ڱ������ձ�������Ƶ�ʿ����ֶ�Ӧ��Ƶƫ
	double freqErr1 = 0;				// Ƶ������λHz�����Ǳ��ȡ�����ڱ����һ��AFC�����ֶ�Ӧ��Ƶƫ
	double freqErr2 = 0;				// Ƶ������λHz�����Ǳ��ȡ�����ڱ���ڶ���AFC�����ֶ�Ӧ��Ƶƫ

	int limit = config->freq;				// Ƶ��������ޣ���λHz��Ϊ����У׼Ƶ�ʵ�1ppm�������޻����ʵ�ʲ���Ƶ��仯
	double power = 20.0;			// ��λdBm��Ƶ��У׼�����й��ʣ����ڴ��Ǳ��ȡUE���й���

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
	ret = m_pInstru->InitInstrument();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ʼ���Ǳ����ò���ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ʼ���Ǳ����ò����ɹ���");
		m_logOutput(logBuff);
	}

	// �����Ǳ�����е����в���Ƶ��
	ret = m_pInstru->SetMeasurementFreq(config->freq, config->bCWSinaling);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�����е����в���Ƶ��ʧ�ܣ�Ƶ��Ϊ %.1f MHz", config->freq);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�����е����в���Ƶ�ʳɹ���Ƶ��Ϊ %.1f MHz", config->freq);
		m_logOutput(logBuff);
	}

	// �����Ǳ�����е�������������
	ret = m_pInstru->SetExpPower(config->expPower, config->bCWSinaling);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�����е�������������ʧ�ܣ���������Ϊ %.1f dBm", config->expPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�����е������������ʳɹ�����������Ϊ %.1f dBm", config->expPower);
		m_logOutput(logBuff);
	}

	// �����Ǳ���������
	ret = m_pInstru->SetMeasurementLoss(config->pathLoss, config->bCWSinaling);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�����е���������ʧ�ܣ�����Ϊ %.1f dB", config->pathLoss);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�����е���������ɹ�������Ϊ %.1f dB", config->pathLoss);
		m_logOutput(logBuff);
	}

	// ��UE���������Ҫ��configҳ���л�ȡƵ�ʡ�APC�����ֺ�PA��λ
	if (m_pDUT->OpenTransmitter(config->freq, config->paMode, config->apcData))
	{
		sprintf_s(logBuff, "��UE������ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "��UE�����ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	// ����UE�����AFC������1
	if (m_pDUT->SetTransmitterFrequencyOffset(config->afcData1))
	{
		sprintf_s(logBuff, "����UE�����AFC������1�ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "����UE�����AFC������1ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	// ���Ǳ��ȡ��һ��AFC�����ֶ�Ӧ��Ƶ�����
	ret = m_pInstru->GetFreqErr(freqErr1, power);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "���Ǳ��ȡ��һ��AFC�����ֶ�Ӧ��Ƶ�����ʧ�ܣ�AFC������Ϊ %d", config->afcData1);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "���Ǳ��ȡ��һ��AFC�����ֶ�Ӧ��Ƶ�����ɹ���AFC������Ϊ %d��Ƶ�����Ϊ %d Hz�����й���Ϊ %.1f dBm", config->afcData1, freqErr1, power);
		m_logOutput(logBuff);
	}

	// ����UE�����AFC������2
	if (m_pDUT->SetTransmitterFrequencyOffset(config->afcData2))
	{
		sprintf_s(logBuff, "����UE�����AFC������2�ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "����UE�����AFC������2ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	// ���Ǳ��ȡ�ڶ���AFC�����ֶ�Ӧ��Ƶ�����
	ret = m_pInstru->GetFreqErr(freqErr2, power);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "���Ǳ��ȡ�ڶ���AFC�����ֶ�Ӧ��Ƶ�����ʧ�ܣ�AFC������Ϊ %d", config->afcData2);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "���Ǳ��ȡ�ڶ���AFC�����ֶ�Ӧ��Ƶ�����ɹ���AFC������Ϊ %d��Ƶ�����Ϊ %d Hz�����й���Ϊ %.1f dBm", config->afcData2, freqErr2, power);
		m_logOutput(logBuff);
	}

	// ����AFC������б�ʺ�ƫ��ֵ��б�ʵ�λΪ Hz����ʾAFC������ÿ�仯1��Ƶ�ʵı仯ֵ
	slope = (freqErr1 - freqErr2) / (config->afcData1 - config->afcData2);
	afcData = config->afcData2 + freqErr2 / slope;

	sprintf_s(logBuff, "����AFC������б�ʺ�ƫ��ֵ��AFC������б��Ϊ %d��AFCƫ��ֵΪ %d", slope, afcData);
	m_logOutput(logBuff);

	// ȷ�ϼ������AFC������ƫ�����Ƿ�����Ҫ�󣬲�����Ҫ����Ҫ����б�ʽ���΢��
	for (int actualCycleNum = 0; actualCycleNum <= config->cycleNum; actualCycleNum++)
	{
		// ����UE�����AFC�����֣��ÿ�����Ϊ�ոռ������AFC������ƫ��ֵ
		if (m_pDUT->SetTransmitterFrequencyOffset(afcData))
		{
			sprintf_s(logBuff, "����UE�����AFC�����ֳɹ�");
			m_logOutput(logBuff);
		}
		else
		{
			sprintf_s(logBuff, "����UE�����AFC������ʧ��");
			m_logOutput(logBuff);
			return FAIL;
		}


		// ���Ǳ��ȡ��AFC������ƫ��ֵ��Ӧ��Ƶ�����
		ret = m_pInstru->GetFreqErr(freqErr, power);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "���Ǳ��ȡAFC������ƫ��ֵ��Ӧ��Ƶ�����ʧ�ܣ�AFC������Ϊ %d", afcData);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "���Ǳ��ȡAFC������ƫ��ֵ��Ӧ��Ƶ�����ɹ���AFC������Ϊ %d��Ƶ�����Ϊ %d Hz�����й���Ϊ %.1f dBm", afcData, freqErr, power);
			m_logOutput(logBuff);
		}

		// �ж�AFC������ƫ��ֵ��Ӧ��Ƶƫ�Ƿ�����Ҫ��
		// �������Ҫ����ƫ��ֵ��б��д��AFC���
		// ���������Ҫ����Ҫ����б�ʽ���΢��
		// �������΢��ѭ���������򱨴��˳�
		if (abs(freqErr) < limit)
		{
			// ��ֵ�����
			sprintf_s(logBuff, "���Ǳ��ȡ��Ƶ���������Ҫ��AFC������Ϊ %d��б��Ϊ %d", afcData, slope);
			m_logOutput(logBuff);
			nvTable.afc.afcSlope = (short)slope;
			nvTable.afc.afcInit = (unsigned short)afcData;
			ret = PASS;
			break;
		}
		else if (actualCycleNum < config->cycleNum)
		{
			afcData = afcData + freqErr / slope;
			sprintf_s(logBuff, "���Ǳ��ȡ��Ƶ��������Ҫ��AFC���������ڽ��е�%d��΢�����������AFC������Ϊ %d", actualCycleNum + 1, afcData);
			m_logOutput(logBuff);
		}
		else
		{
			sprintf_s(logBuff, "���Ǳ��ȡ��Ƶ��������Ҫ���Ѵﵽ���΢��������AFCУ׼ʧ�ܣ�");
			m_logOutput(logBuff);
			ret = FAIL;
			return ret;
		}
	}

	// ��UEͨ�ţ���AFC���д��NV
	if (m_pDUT->WriteAFCCodeTable(nvTable.afc))
	{
		sprintf_s(logBuff, "AFC���д��NV�ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "AFC���д��NVʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	// ��UEͨ�ţ�����AFC�����RAM
	if (m_pDUT->UpdateCodeTableToRAM())
	{
		sprintf_s(logBuff, "����AFC�����RAM�ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "����AFC�����RAMʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	return ret;
}

/********************************************************************
* Function��APCCal
* Description: tx power calibration
* Input:
* NurRFNVCal nvTable: nv table
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History��
Name	Date		Contents Reason
Jason	2019-1-22	Add tx power calibration
.
.
********************************************************************/
int NurBLCal::APCCal(APCConf* config)
{
	NurRFNVCal nvTable;

	int ret = PASS;											// У׼���
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log��ӡ����

	int apcData[BAND_NUM][TX_PA_NUM] = { 0 };				// ÿ��У׼Ƶ��ÿ��PA��λУ׼��������APC������ƫ��ֵ
	double apcSlope[BAND_NUM][TX_PA_NUM] = { 0 };			// ÿ��У׼Ƶ��ÿ��PA��λУ׼��������APC������б��

	double apcSlope_High[BAND_NUM][TX_PA_NUM] = { 0 };		// ÿ��У׼Ƶ��ÿ��PA��λУ׼�������ĸ�Ƶ�㲹����б��
	double apcSlope_Low[BAND_NUM][TX_PA_NUM] = { 0 };		// ÿ��У׼Ƶ��ÿ��PA��λУ׼�������ĵ�Ƶ�㲹����б��
	double expPower1[TX_PA_NUM] = { 0 };					// ÿ��PA��λУ׼ʱʹ�õĵ�һ�����ʵ��Ӧ���������ʣ��������궨���ȡ
	double expPower2[TX_PA_NUM] = { 0 };					// ÿ��PA��λУ׼ʱʹ�õĵڶ������ʵ��Ӧ���������ʣ��������궨���ȡ

	// �Ǳ����ò����������ʸ�ֵ�������궨���ȡ���Χ�����ݷ�Χ���ж���
	expPower1[0] = APC_HIGH_START + 10;
	expPower1[1] = APC_MID_START + 10;
	expPower1[2] = APC_LOW_START + 10;
	expPower2[0] = APC_HIGH_END + 20;
	expPower2[1] = APC_MID_END + 20;
	expPower2[2] = APC_LOW_END + 20;

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

	// ��ʼ�����������
	ret = InitTransmitter(config);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ʼ���ն˷��������ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ʼ���ն˷�������̳ɹ���");
		m_logOutput(logBuff);
	}

	// ѭ����ʼÿ������Ƶ�ε�У׼
	for (int i = 0; i < BAND_NUM; i++)
	{
		// ����3GPP��׼��һ��18������Ƶ�Σ����ηֱ�Ϊ1��2��3��5��8��11��12��13��17��18��19��20��25��26��28��31��66��70
		// ��18��Ƶ�ζ�Ӧ������bandCalEnable[]���±��0-17
		// ���ڳ�����Ƶ�㷶Χ���ظ���band12����band17��band25����band2��band26����band2/18/19�����NV���ȥ����band17/2/5/18/19
		// NV���һ��13��Ƶ�Σ��ֱ���1��3��8��11��12��13��20��25��26��28��31��66��70
		// ��13��Ƶ�ζ�Ӧ��NV��������NurRFNVCal.Info[]���±��0-12
		// ��ˣ����band12/17���⣬band2/25���⣬band5/18/19���⣬�����ý����У�����Ƶ���е�һ��Ƶ�α�ʹ�ܣ�������Ƶ�α���ֹ
		// ������У׼�У�band17���Զ�ӳ�䵽band12��band2���Զ�ӳ�䵽band25��band2/18/19���Զ�ӳ�䵽band26
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
		15			31				452.5 �C 457.5 MHz			462.5 �C 467.5 MHz			10
		16			66				1710 - 1780 MHz				2110 - 2200 MHz				11
		17			70				1695 - 1710 MHz				1995 - 2020 MHz				12
		*/

		// �����ж�����Ƶ���Ƿ�ʹ�ܣ����û��ʹ�ܣ���ֱ����ת����һ��Ƶ��
		if (false == config->bandCalEnable[i])
		{
			continue;
		}

		// i�����ϱ��е�index��������Ҫӳ�䵽NV����ж�Ӧ��NV index�����õ���NVƵ�ζ�Ӧ�����Ƶ�����СƵ��
		int nvIndex = 0;						// ����Ƶ����NV����ж�Ӧ�����
		double freqDLMax = 0;					// ����Ƶ�ζ�Ӧ��NV���Ƶ�ε����Ƶ�㣬����Ƶ�ʲ�������
		double freqDLMin = 0;					// ����Ƶ�ζ�Ӧ��NV���Ƶ�ε���СƵ�㣬����Ƶ�ʲ�������
		ret = NurCommonFunc::GetNVIndexFromBandIndex(i, nvIndex, freqDLMax, freqDLMin);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "����Ƶ����������ȡNV��������ʧ��");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "����Ƶ����������ȡNV���������ɹ�");
			m_logOutput(logBuff);
		}

		double centerFreqDL = 0;				// ����Ƶ����������Ƶ��
		double centerFreqUL = 0;				// ����Ƶ����������Ƶ��

		// ���������ŵ��Ż�ȡ��Ӧ��������Ƶ��
		ret = NurCommonFunc::GetFreqFromDLChan(config->centerChannel[i], centerFreqDL, centerFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����ŵ��ŵ���Ϊ�Ƿ����ݣ��ŵ���Ϊ %d", config->centerChannel[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "���������ŵ��Ż�ȡ��Ӧ��������Ƶ��ɹ��������ŵ���Ϊ %d����������Ƶ��Ϊ %.1f MHz����������Ƶ��Ϊ %.1f MHz", config->centerChannel[i], centerFreqDL, centerFreqUL);
			m_logOutput(logBuff);
		}

		// �����Ǳ�����е����в���Ƶ��
		ret = m_pInstru->SetMeasurementFreq(centerFreqUL, config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�����е����в���Ƶ��ʧ�ܣ�Ƶ��Ϊ %.1f MHz", centerFreqUL);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�����е����в���Ƶ�ʳɹ���Ƶ��Ϊ %.1f MHz", centerFreqUL);
			m_logOutput(logBuff);
		}

		// �����Ǳ���������
		ret = m_pInstru->SetMeasurementLoss(config->centerFreqPathLossUL[i], config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�����е���������ʧ�ܣ�����Ϊ %.1f dB", config->centerFreqPathLossUL[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�����е���������ɹ�������Ϊ %.1f dB", config->centerFreqPathLossUL[i]);
			m_logOutput(logBuff);
		}

		// ѭ����������Ƶ�㲻ͬPA��λ�µĹ���
		for (int j = 0; j < TX_PA_NUM; j++)
		{
			// ��ͬPA��λ��Ӧ���������ʺ;���
			string paMode = "";				// PA��λ�����ַ�����������log�������ֱ��
			double apcNVPowerMax = 0;		// ��ͬPA��λ��Ӧ��APC��������
			double apcNVPowerStep = 0;		// ��ͬPA��λ��Ӧ��APC�����
			int apcNVLen = 0;
			switch (j)
			{
			case TX_PA_LOW:
				paMode = "�����浵";
				apcNVPowerMax = APC_LOW_START;
				apcNVPowerStep = APC_LOW_STEP;
				apcNVLen = APC_LOW_NUM;
				break;
			case TX_PA_MID:
				paMode = "�����浵";
				apcNVPowerMax = APC_MID_START;
				apcNVPowerStep = APC_MID_STEP;
				apcNVLen = APC_MID_NUM;
				break;
			default:
				paMode = "�����浵";
				apcNVPowerMax = APC_HIGH_START;
				apcNVPowerStep = APC_HIGH_STEP;
				apcNVLen = APC_HIGH_NUM;
				break;
			}

			// �����Ǳ�����е�������������
			ret = m_pInstru->SetExpPower(expPower1[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "�����Ǳ�����е�������������ʧ�ܣ���������Ϊ %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "�����Ǳ�����е������������ʳɹ�����������Ϊ %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
			}

			// ����UE�����Ƶ��

			// ����UE�����PA��λ�͸�PA��λ�µ�һ��APC������

			// ���Ǳ��ȡ��һ��APC�����ֶ�Ӧ�����й���
			ret = m_pInstru->GetTXPower(m_power1[nvIndex][j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "���Ǳ��ȡ��һ��APC�����ֶ�Ӧ�����й���ʧ�ܣ�APC������Ϊ %d, PA���浵λΪ%s", config->apcData1[j], paMode);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "���Ǳ��ȡ��һ��APC�����ֶ�Ӧ�����й��ʳɹ���APC������Ϊ %d��PA���浵λΪ%s�����й���Ϊ %.1f dBm", config->apcData1[j], paMode, m_power1[j]);
				m_logOutput(logBuff);
			}

			// ����UE�����PA��λ�͸�PA��λ�µڶ���APC������

			// �����Ǳ�����е�������������
			ret = m_pInstru->SetExpPower(expPower2[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "�����Ǳ�����е�������������ʧ�ܣ���������Ϊ %.1f dBm", expPower2[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "�����Ǳ�����е������������ʳɹ�����������Ϊ %.1f dBm", expPower2[j]);
				m_logOutput(logBuff);
			}

			// ���Ǳ��ȡ�ڶ���APC�����ֶ�Ӧ�����й���
			ret = m_pInstru->GetTXPower(m_power2[nvIndex][j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "���Ǳ��ȡ�ڶ���APC�����ֶ�Ӧ�����й���ʧ�ܣ�APC������Ϊ %d, PA���浵λΪ%s", config->apcData2[j], paMode);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "���Ǳ��ȡ�ڶ���APC�����ֶ�Ӧ�����й��ʳɹ���APC������Ϊ %d��PA���浵λΪ%s�����й���Ϊ %.1f dBm", config->apcData2[j], paMode, m_power1[j]);
				m_logOutput(logBuff);
			}

			// ���ݸ�PA��λ�µ��������ʵ����ݣ������ƫ��ֵ��б��
			// ����ƫ��ֵ��б�ʼ������PA��λ�������APC���ʶ�ӦAPC������

			// ����APC������б�ʺ�ƫ��ֵ��б�ʵ�λΪ /dB����ʾ���й���ÿ�仯1dB��APC�����ֵı仯ֵ
			apcSlope[i][j] = (config->apcData1[j] - config->apcData2[j]) / (m_power1[j] - m_power2[j]);

			// �������ƫ��ֵΪ���й����ڲ�ͬ���浵λ��������(APC_HIGH/MID/LOW_START)��Ӧ��APC������
			apcData[i][j] = (apcNVPowerMax - m_power1[nvIndex][j]) / apcSlope[i][j] + config->apcData1[j];

			sprintf_s(logBuff, "����APC������б�ʺ�ƫ��ֵ��APC������б��Ϊ %s��APCƫ��ֵΪ %d��PA���浵λΪ%s", apcSlope[i][j], apcData[i][j], paMode);
			m_logOutput(logBuff);
			for (int k = 0; k < apcNVLen; k++)
			{
				// ��PAĳ���浵λ��������(APC_HIGH/MID/LOW_START)��Ӧ��APC������Ϊƫ��ֵ����������±�0��ʾ�����Χ�ڵ������
				// ��������±�ÿ����1�����ʼ���һ������APC_HIGH/MID/LOW_STEP����Ӧ�Ŀ����ּ���k * ���� * б��
				nvTable.info[nvIndex].apc.apcHigh[k] = apcData[i][j] - int(k * apcNVPowerStep * apcSlope[i][j]);
			}
		}
	}
	return ret;
}

/********************************************************************
* Function��TFCCal
* Description: tx power frequency compensation calibration
* Input:
* NurRFNVCal nvTable: nv table
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History��
Name	Date		Contents Reason
Jason	2019-3-28	Add tx power frequency compensation calibration
.
.
********************************************************************/
int NurBLCal::TFCCal(APCConf* config)
{
	NurRFNVCal nvTable;

	int ret = PASS;											// У׼���
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log��ӡ����

	double apcSlope_High[BAND_NUM][TX_PA_NUM] = { 0 };		// ÿ��У׼Ƶ��ÿ��PA��λУ׼�������ĸ�Ƶ�㲹����б��
	double apcSlope_Low[BAND_NUM][TX_PA_NUM] = { 0 };		// ÿ��У׼Ƶ��ÿ��PA��λУ׼�������ĵ�Ƶ�㲹����б��
	double expPower1[TX_PA_NUM] = { 0 };					// ÿ��PA��λУ׼ʱʹ�õĵ�һ�����ʵ��Ӧ���������ʣ��������궨���ȡ

	// �Ǳ����ò����������ʸ�ֵ�������궨���ȡ���Χ�����ݷ�Χ���ж���
	expPower1[0] = APC_HIGH_START + 10;
	expPower1[1] = APC_MID_START + 10;
	expPower1[2] = APC_LOW_START + 10;

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

	// ��ʼ�����������
	ret = InitTransmitter(config);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ʼ���ն˷��������ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ʼ���ն˷�������̳ɹ���");
		m_logOutput(logBuff);
	}

	// ѭ����ʼÿ������Ƶ�ε�У׼
	for (int i = 0; i < BAND_NUM; i++)
	{
		// ����3GPP��׼��һ��18������Ƶ�Σ����ηֱ�Ϊ1��2��3��5��8��11��12��13��17��18��19��20��25��26��28��31��66��70
		// ��18��Ƶ�ζ�Ӧ������bandCalEnable[]���±��0-17
		// ���ڳ�����Ƶ�㷶Χ���ظ���band12����band17��band25����band2��band26����band2/18/19�����NV���ȥ����band17/2/5/18/19
		// NV���һ��13��Ƶ�Σ��ֱ���1��3��8��11��12��13��20��25��26��28��31��66��70
		// ��13��Ƶ�ζ�Ӧ��NV��������NurRFNVCal.Info[]���±��0-12
		// ��ˣ����band12/17���⣬band2/25���⣬band5/18/19���⣬�����ý����У�����Ƶ���е�һ��Ƶ�α�ʹ�ܣ�������Ƶ�α���ֹ
		// ������У׼�У�band17���Զ�ӳ�䵽band12��band2���Զ�ӳ�䵽band25��band2/18/19���Զ�ӳ�䵽band26
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
		15			31				452.5 �C 457.5 MHz			462.5 �C 467.5 MHz			10
		16			66				1710 - 1780 MHz				2110 - 2200 MHz				11
		17			70				1695 - 1710 MHz				1995 - 2020 MHz				12
		*/

		// �����ж�����Ƶ���Ƿ�ʹ�ܣ����û��ʹ�ܣ���ֱ����ת����һ��Ƶ��
		if (false == config->bandCalEnable[i])
		{
			continue;
		}

		// i�����ϱ��е�index��������Ҫӳ�䵽NV����ж�Ӧ��NV index�����õ���NVƵ�ζ�Ӧ�����Ƶ�����СƵ��
		int nvIndex = 0;						// ����Ƶ����NV����ж�Ӧ�����
		double freqDLMax = 0;					// ����Ƶ�ζ�Ӧ��NV���Ƶ�ε����Ƶ�㣬����Ƶ�ʲ�������
		double freqDLMin = 0;					// ����Ƶ�ζ�Ӧ��NV���Ƶ�ε���СƵ�㣬����Ƶ�ʲ�������
		ret = NurCommonFunc::GetNVIndexFromBandIndex(i, nvIndex, freqDLMax, freqDLMin);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "����Ƶ����������ȡNV��������ʧ��");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "����Ƶ����������ȡNV���������ɹ�");
			m_logOutput(logBuff);
		}

		double centerFreqDL = 0;				// ����Ƶ����������Ƶ��
		double centerFreqUL = 0;				// ����Ƶ����������Ƶ��
		double compPower[TX_PA_NUM] = { 0 };	// ����ÿ��PA��λУ׼ʱʹ�õ�Ƶ�ʲ������ʵ��APC�����ֶ�Ӧ�Ĳ�������

		// ��Ƶ��Ƶ�ʲ���У׼	
		double compFreqDL = 0;					// ����Ƶ���������Ƶ��
		double compFreqUL = 0;					// ����Ƶ��������СƵ��

		// ���������ŵ��Ż�ȡ��Ӧ��������Ƶ��
		ret = NurCommonFunc::GetFreqFromDLChan(config->compChannelHigh[i], compFreqDL, compFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "Ƶ�ʲ����ŵ��ŵ���Ϊ�Ƿ����ݣ��ŵ���Ϊ %d", config->compChannelHigh[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "���������ŵ��Ż�ȡ��Ӧ��������Ƶ��ɹ��������ŵ���Ϊ %d����������Ƶ��Ϊ %.1f MHz����������Ƶ��Ϊ %.1f MHz", config->compChannelHigh[i], compFreqDL, compFreqUL);
			m_logOutput(logBuff);
		}

		// �����Ǳ�����е����в���Ƶ��
		ret = m_pInstru->SetMeasurementFreq(compFreqUL, config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�����е����в���Ƶ��ʧ�ܣ�Ƶ��Ϊ %.1f MHz", compFreqUL);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�����е����в���Ƶ�ʳɹ���Ƶ��Ϊ %.1f MHz", compFreqUL);
			m_logOutput(logBuff);
		}

		// �����Ǳ���������
		ret = m_pInstru->SetMeasurementLoss(config->compFreqPathLossHighUL[i], config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�����е���������ʧ�ܣ�����Ϊ %.1f dB", config->compFreqPathLossHighUL[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�����е���������ɹ�������Ϊ %.1f dB", config->compFreqPathLossHighUL[i]);
			m_logOutput(logBuff);
		}

		// ѭ�����Բ����ŵ���ͬPA��λ�µĹ���
		for (int j = 0; j < TX_PA_NUM; j++)
		{
			// ��ͬPA��λ��Ӧ���������ʺ;���
			string paMode = "";				// PA��λ�����ַ�����������log�������ֱ��
			double apcNVPowerStep = 0;		// ��ͬPA��λ��Ӧ��APC�����
			switch (j)
			{
			case TX_PA_LOW:
				paMode = "�����浵";
				apcNVPowerStep = APC_LOW_STEP;
				break;
			case TX_PA_MID:
				paMode = "�����浵";
				apcNVPowerStep = APC_MID_STEP;
				break;
			default:
				paMode = "�����浵";
				apcNVPowerStep = APC_HIGH_STEP;
				break;
			}

			// �����Ǳ�����е�������������
			ret = m_pInstru->SetExpPower(expPower1[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "�����Ǳ�����е�������������ʧ�ܣ���������Ϊ %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "�����Ǳ�����е������������ʳɹ�����������Ϊ %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
			}

			// ����UE�����Ƶ��

			// ����UE�����PA��λ�͸�PA��λ�µ�һ��APC�����֣�������������ֶ�Ӧ�����й���ȥ������Ƶ���һ��APC�����ֶ�Ӧ�����й��ʱȽϣ��������ƽ̹��

			// ���Ǳ��ȡ�����ŵ�APC�����ֶ�Ӧ�����й���
			ret = m_pInstru->GetTXPower(compPower[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "���Ǳ��ȡ�����ŵ�APC�����ֶ�Ӧ�����й���ʧ�ܣ�APC������Ϊ %d, PA���浵λΪ%s", config->apcData1[j], paMode);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "���Ǳ��ȡ�����ŵ�APC�����ֶ�Ӧ�����й��ʳɹ���APC������Ϊ %d��PA���浵λΪ%s�����й���Ϊ %.1f dBm", config->apcData1[j], paMode, compPower[j]);
				m_logOutput(logBuff);
			}

			// ���ݸ�PA��λ�µ�����Ƶ��Ͳ���Ƶ���������ʵ����ݣ������б��
			// ����б�ʼ������PA��λ�������TFC����ֵ

			// ���㷢���Ƶ�ʲ���б�ʣ�б�ʵ�λΪAPC����±�ƫ�� / 100kHz����ʾƵ��ÿ�仯100kHz(��һ���ŵ�)����������APC����±��ƫ��ֵ
			// һ��Ҫע������Ĳ�����APC����±겹�������Ǿ����dBֵ���±겹���ľ��Ⱥ�APC�����һ��
			apcSlope_High[i][j] = (compPower[j] - m_power1[nvIndex][j]) / (config->compChannelHigh[i] - config->centerChannel[i]) / apcNVPowerStep;
			sprintf_s(logBuff, "���㷢���Ƶ�ʲ���б�ʣ�����б��Ϊ %.2f��PA��λΪ%s", apcSlope_High[i][j], paMode);
			m_logOutput(logBuff);

			// ����Ƶ�ʲ����еĸ�Ƶ���Ƶ�ʲ������Դ�������Ƶ��Ϊ�ֽ磬����Ƶ�����ΪTFC����ȵ�һ��
			for (int k = TFC_FREQ_NUM / 2; k < TFC_FREQ_NUM; k++)
			{
				nvTable.info[nvIndex].tfc.info[k].frequency = short((freqDLMax - centerFreqDL) * 10 * (k - double(TFC_FREQ_NUM / 2) + 1) / (TFC_FREQ_NUM / 2));
				nvTable.info[nvIndex].tfc.info[k].comp[j] = char(nvTable.info[nvIndex].tfc.info[k].frequency * apcSlope_High[i][j]);
			}
		}

		// ���������ŵ��Ż�ȡ��Ӧ��������Ƶ��
		ret = NurCommonFunc::GetFreqFromDLChan(config->compChannelLow[i], compFreqDL, compFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "Ƶ�ʲ����ŵ��ŵ���Ϊ�Ƿ����ݣ��ŵ���Ϊ %d", config->compChannelLow[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "���������ŵ��Ż�ȡ��Ӧ��������Ƶ��ɹ��������ŵ���Ϊ %d����������Ƶ��Ϊ %.1f MHz����������Ƶ��Ϊ %.1f MHz", config->compChannelLow[i], compFreqDL, compFreqUL);
			m_logOutput(logBuff);
		}

		// �����Ǳ�����е����в���Ƶ��
		ret = m_pInstru->SetMeasurementFreq(compFreqUL, config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�����е����в���Ƶ��ʧ�ܣ�Ƶ��Ϊ %.1f MHz", compFreqUL);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�����е����в���Ƶ�ʳɹ���Ƶ��Ϊ %.1f MHz", compFreqUL);
			m_logOutput(logBuff);
		}

		// �����Ǳ���������
		ret = m_pInstru->SetMeasurementLoss(config->compFreqPathLossLowUL[i], config->bCWSinaling);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�����е���������ʧ�ܣ�����Ϊ %.1f dB", config->compFreqPathLossLowUL[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�����е���������ɹ�������Ϊ %.1f dB", config->compFreqPathLossLowUL[i]);
			m_logOutput(logBuff);
		}

		// ѭ�����Բ����ŵ���ͬPA��λ�µĹ���
		for (int j = 0; j < TX_PA_NUM; j++)
		{
			// ��ͬPA��λ��Ӧ���������ʺ;���
			string paMode = "";				// PA��λ�����ַ�����������log�������ֱ��
			double apcNVPowerStep = 0;		// ��ͬPA��λ��Ӧ��APC�����
			switch (j)
			{
			case TX_PA_LOW:
				paMode = "�����浵";
				apcNVPowerStep = APC_LOW_STEP;
				break;
			case TX_PA_MID:
				paMode = "�����浵";
				apcNVPowerStep = APC_MID_STEP;
				break;
			default:
				paMode = "�����浵";
				apcNVPowerStep = APC_HIGH_STEP;
				break;
			}

			// �����Ǳ�����е�������������
			ret = m_pInstru->SetExpPower(expPower1[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "�����Ǳ�����е�������������ʧ�ܣ���������Ϊ %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "�����Ǳ�����е������������ʳɹ�����������Ϊ %.1f dBm", expPower1[j]);
				m_logOutput(logBuff);
			}

			// ����UE�����Ƶ��

			// ����UE�����PA��λ�͸�PA��λ�µ�һ��APC�����֣�������������ֶ�Ӧ�����й���ȥ������Ƶ���һ��APC�����ֶ�Ӧ�����й��ʱȽϣ��������ƽ̹��

			// ���Ǳ��ȡ�����ŵ�APC�����ֶ�Ӧ�����й���
			ret = m_pInstru->GetTXPower(compPower[j], config->bCWSinaling);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "���Ǳ��ȡ�����ŵ�APC�����ֶ�Ӧ�����й���ʧ�ܣ�APC������Ϊ %d, PA���浵λΪ%s", config->apcData1[j], paMode);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "���Ǳ��ȡ�����ŵ�APC�����ֶ�Ӧ�����й��ʳɹ���APC������Ϊ %d��PA���浵λΪ%s�����й���Ϊ %.1f dBm", config->apcData1[j], paMode, compPower[j]);
				m_logOutput(logBuff);
			}

			// ���ݸ�PA��λ�µ�����Ƶ��Ͳ���Ƶ���������ʵ����ݣ������б��
			// ����б�ʼ������PA��λ�������TFC����ֵ

			// ���㷢���Ƶ�ʲ���б�ʣ�б�ʵ�λΪAPC����±�ƫ�� / 100kHz����ʾƵ��ÿ�仯100kHz(��һ���ŵ�)����������APC����±��ƫ��ֵ
			// һ��Ҫע������Ĳ�����APC����±겹�������Ǿ����dBֵ���±겹���ľ��Ⱥ�APC�����һ��
			apcSlope_Low[i][j] = (compPower[j] - m_power1[nvIndex][j]) / (config->compChannelLow[i] - config->centerChannel[i]) / apcNVPowerStep;
			sprintf_s(logBuff, "���㷢���Ƶ�ʲ���б�ʣ�����б��Ϊ %.2f��PA��λΪ%s", apcSlope_Low[i][j], paMode);
			m_logOutput(logBuff);

			// ����Ƶ�ʲ����еĵ�Ƶ���Ƶ�ʲ������Դ�������Ƶ��Ϊ�ֽ磬����Ƶ�����ΪTFC����ȵ�һ��
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
* Function��AGCCal
* Description: rx gain calibration
* Input:
* NurRFNVCal nvTable: nv table
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History��
Name	Date		Contents Reason
Jason	2019-1-22	Add rx gain calibration
.
.
********************************************************************/
int NurBLCal::AGCCal(AGCConf* config)
{
	NurRFNVCal nvTable;
	int ret = 0;											// У׼���
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log��ӡ����

	int rssi[RX_GAIN_NUM] = { 0 };							// UE�ڲ�ͬ���ջ�����״̬�»�ȡ���Ľ��ջ��ź�����ֵ
	int rxGain[RX_GAIN_NUM] = { 0 };						// ����UE��ȡ�Ľ��ջ�����ֵ�����������Ĺ��ʣ��ټ�ȥ���������źţ��õ����ջ�����

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

	// ��ʼ�����ջ�����
	ret = InitReceiver(config);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ʼ���ն˽��ջ�����ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ʼ���ն˽��ջ����̳ɹ���");
		m_logOutput(logBuff);
	}

	// ѭ����ʼÿ������Ƶ�ε�У׼
	for (int i = 0; i < BAND_NUM; i++)
	{
		// ����3GPP��׼��һ��18������Ƶ�Σ����ηֱ�Ϊ1��2��3��5��8��11��12��13��17��18��19��20��25��26��28��31��66��70
		// ��18��Ƶ�ζ�Ӧ������bandCalEnable[]���±��0-17
		// ���ڳ�����Ƶ�㷶Χ���ظ���band12����band17��band25����band2��band26����band5/18/19�����NV���ȥ����band17/2/5/18/19
		// NV���һ��13��Ƶ�Σ��ֱ���1��3��8��11��12��13��20��25��26��28��31��66��70
		// ��13��Ƶ�ζ�Ӧ��NV��������NurRFNVCal.Info[]���±��0-12
		// ��ˣ����band12/17���⣬band2/25���⣬band5/18/19���⣬�����ý����У�����Ƶ���е�һ��Ƶ�α�ʹ�ܣ�������Ƶ�α���ֹ
		// ������У׼�У�band17���Զ�ӳ�䵽band12��band2���Զ�ӳ�䵽band25��band2/18/19���Զ�ӳ�䵽band26
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
		15			31				452.5 �C 457.5 MHz			462.5 �C 467.5 MHz			10
		17			66				1710 - 1780 MHz				2110 - 2200 MHz				11
		18			70				1695 - 1710 MHz				1995 - 2020 MHz				12
		*/
		// �����ж�����Ƶ���Ƿ�ʹ�ܣ����û��ʹ�ܣ���ֱ����ת����һ��Ƶ��
		if (false == config->bandCalEnable[i])
		{
			continue;
		}

		// i�����ϱ��е�index��������Ҫӳ�䵽NV����ж�Ӧ��NV index�����õ���NVƵ�ζ�Ӧ�����Ƶ�����СƵ��
		int nvIndex = 0;						// ����Ƶ����NV����ж�Ӧ�����
		double freqDLMax = 0;					// ����Ƶ�ζ�Ӧ��NV���Ƶ�ε����Ƶ�㣬����Ƶ�ʲ�������
		double freqDLMin = 0;					// ����Ƶ�ζ�Ӧ��NV���Ƶ�ε���СƵ�㣬����Ƶ�ʲ�������
		ret = NurCommonFunc::GetNVIndexFromBandIndex(i, nvIndex, freqDLMax, freqDLMin);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "����Ƶ����������ȡNV��������ʧ��");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "����Ƶ����������ȡNV���������ɹ�");
			m_logOutput(logBuff);
		}

		double centerFreqDL = 0;				// ����Ƶ����������Ƶ��
		double centerFreqUL = 0;				// ����Ƶ����������Ƶ��


		// ���������ŵ��Ż�ȡ��Ӧ��������Ƶ��
		ret = NurCommonFunc::GetFreqFromDLChan(config->centerChannel[i], centerFreqDL, centerFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����ŵ��ŵ���Ϊ�Ƿ����ݣ��ŵ���Ϊ %d", config->centerChannel[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "���������ŵ��Ż�ȡ��Ӧ��������Ƶ��ɹ��������ŵ���Ϊ %d����������Ƶ��Ϊ %.1f MHz����������Ƶ��Ϊ %.1f MHz", config->centerChannel[i], centerFreqDL, centerFreqUL);
			m_logOutput(logBuff);
		}

		// �����Ǳ�����е�����Ƶ��
		ret = m_pInstru->SetGeneratorFreq(centerFreqDL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�����е����й���Ƶ��ʧ�ܣ�Ƶ��Ϊ %.1f MHz", centerFreqDL);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�����е����й���Ƶ�ʳɹ���Ƶ��Ϊ %.1f MHz", centerFreqDL);
			m_logOutput(logBuff);
		}

		// �����Ǳ���������
		ret = m_pInstru->SetGeneratorLoss(config->centerFreqPathLossDL[i]);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����Ǳ�����е���������ʧ�ܣ�����Ϊ %.1f dB", config->centerFreqPathLossDL[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "�����Ǳ�����е���������ɹ�������Ϊ %.1f dB", config->centerFreqPathLossDL[i]);
			m_logOutput(logBuff);
		}

		// ѭ����������Ƶ�㲻ͬ���ջ�����״̬�µĽ��ջ�����
		for (int j = 0; j < RX_GAIN_NUM; j++)
		{
			// ��ͬ��������״̬����
			string rxGainMode = "";				// ���ջ�����״̬�����ַ�����������log�������ֱ��
			switch (j)
			{
			case RX_GAIN_LOW:
			{
				rxGainMode = "������״̬";
				sprintf_s(logBuff, "���ڽ��е�����״̬��AGCУ׼......");
				m_logOutput(logBuff);
				break;
			}
			case RX_GAIN_MID:
			{
				rxGainMode = "������״̬";
				sprintf_s(logBuff, "���ڽ���������״̬��AGCУ׼......");
				m_logOutput(logBuff);
				break;
			}
			case RX_GAIN_HIGH:
			{
				rxGainMode = "������״̬";
				sprintf_s(logBuff, "���ڽ��и�����״̬��AGCУ׼......");
				m_logOutput(logBuff);
				break;
			}
			default:
				break;
			}

			// �����Ǳ�����е����й���
			ret = m_pInstru->SetGeneratorPower(config->agcPower[j]);
			if (ret == FAIL)
			{
				sprintf_s(logBuff, "�����Ǳ�����е����й���ʧ�ܣ�����Ϊ %.1f dBm", config->agcPower[j]);
				m_logOutput(logBuff);
				return ret;
			}
			else
			{
				sprintf_s(logBuff, "�����Ǳ�����е����й��ʳɹ�������Ϊ %.1f dBm", config->agcPower[j]);
				m_logOutput(logBuff);
			}

			// ���ý��ջ�����Ƶ��
			if (m_pDUT->SetReceiverFrequency(centerFreqDL))
			{
				sprintf_s(logBuff, "����UE���ջ�Ƶ��ɹ�");
				m_logOutput(logBuff);
			}
			else
			{
				sprintf_s(logBuff, "����UE���ջ�Ƶ��ʧ��");
				m_logOutput(logBuff);
				return FAIL;
			}

			// ����UE���ջ�����״̬
			if (m_pDUT->SetReceiverGain(j))
			{
				sprintf_s(logBuff, "����UE���ջ�������״̬�ɹ�");
				m_logOutput(logBuff);
			}
			else
			{
				sprintf_s(logBuff, "����UE���ջ�������״̬ʧ��");
				m_logOutput(logBuff);
				return FAIL;
			}

			// ��UE���ȡ���ջ�������ź�ǿ�ȣ�rssi�͹��ʵĻ����ϵ��Ҫ��������������ֻ�Ǹ���һ������
			rxGain[j] = (int)rssi[j] - config->agcPower[j];

			// �����ջ�����ֵ��������
			nvTable.info[nvIndex].agc.agcGain[j] = (signed char)rxGain[j];
			sprintf_s(logBuff, "���ջ�����ֵΪ %d�����ջ�����״̬Ϊ %s", rxGain[i], rxGainMode);
			m_logOutput(logBuff);
		}
	}
	return ret;
}

/********************************************************************
* Function��RFCCal
* Description: rx gain frequency compensation calibration
* Input:
* NurRFNVCal nvTable: nv table
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History��
Name	Date		Contents Reason
Jason	2019-3-28	Add rx gain frequency compensation calibration
.
.
********************************************************************/
int NurBLCal::RFCCal(AGCConf* config)
{
	NurRFNVCal nvTable;
	int ret = 0;											// У׼���
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log��ӡ����

	int rssi[RX_GAIN_NUM] = { 0 };							// UE�ڲ�ͬ���ջ�����״̬�»�ȡ���Ľ��ջ��ź�����ֵ
	int rxGain[RX_GAIN_NUM] = { 0 };						// ����UE��ȡ�Ľ��ջ�����ֵ�����������Ĺ��ʣ��ټ�ȥ���������źţ��õ����ջ�����
	double agcSlope_High[BAND_NUM] = { 0 };					// ��Ƶ����ջ����油��б��
	double agcSlope_Low[BAND_NUM] = { 0 };					// ��Ƶ����ջ����油��б��


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

	// ��ʼ�����ջ�����
	ret = InitReceiver(config);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ʼ���ն˽��ջ�����ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ʼ���ն˽��ջ����̳ɹ���");
		m_logOutput(logBuff);
	}

	// ѭ����ʼÿ������Ƶ�ε�У׼
	for (int i = 0; i < BAND_NUM; i++)
	{
		// ����3GPP��׼��һ��18������Ƶ�Σ����ηֱ�Ϊ1��2��3��5��8��11��12��13��17��18��19��20��25��26��28��31��66��70
		// ��18��Ƶ�ζ�Ӧ������bandCalEnable[]���±��0-17
		// ���ڳ�����Ƶ�㷶Χ���ظ���band12����band17��band25����band2��band26����band5/18/19�����NV���ȥ����band17/2/5/18/19
		// NV���һ��13��Ƶ�Σ��ֱ���1��3��8��11��12��13��20��25��26��28��31��66��70
		// ��13��Ƶ�ζ�Ӧ��NV��������NurRFNVCal.Info[]���±��0-12
		// ��ˣ����band12/17���⣬band2/25���⣬band5/18/19���⣬�����ý����У�����Ƶ���е�һ��Ƶ�α�ʹ�ܣ�������Ƶ�α���ֹ
		// ������У׼�У�band17���Զ�ӳ�䵽band12��band2���Զ�ӳ�䵽band25��band2/18/19���Զ�ӳ�䵽band26
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
		15			31				452.5 �C 457.5 MHz			462.5 �C 467.5 MHz			10
		17			66				1710 - 1780 MHz				2110 - 2200 MHz				11
		18			70				1695 - 1710 MHz				1995 - 2020 MHz				12
		*/
		// �����ж�����Ƶ���Ƿ�ʹ�ܣ����û��ʹ�ܣ���ֱ����ת����һ��Ƶ��
		if (false == config->bandCalEnable[i])
		{
			continue;
		}

		// i�����ϱ��е�index��������Ҫӳ�䵽NV����ж�Ӧ��NV index�����õ���NVƵ�ζ�Ӧ�����Ƶ�����СƵ��
		int nvIndex = 0;						// ����Ƶ����NV����ж�Ӧ�����
		double freqDLMax = 0;					// ����Ƶ�ζ�Ӧ��NV���Ƶ�ε����Ƶ�㣬����Ƶ�ʲ�������
		double freqDLMin = 0;					// ����Ƶ�ζ�Ӧ��NV���Ƶ�ε���СƵ�㣬����Ƶ�ʲ�������
		ret = NurCommonFunc::GetNVIndexFromBandIndex(i, nvIndex, freqDLMax, freqDLMin);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "����Ƶ����������ȡNV��������ʧ��");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "����Ƶ����������ȡNV���������ɹ�");
			m_logOutput(logBuff);
		}

		for (int j = 0; j < RX_GAIN_NUM; j++) {
			rxGain[j] = nvTable.info[nvIndex].agc.agcGain[j];
		}

		double centerFreqDL = 0;				// ����Ƶ����������Ƶ��
		double centerFreqUL = 0;				// ����Ƶ����������Ƶ��

		int rssiComp = 0;						// UE�ڲ����ŵ���ȡ���Ľ��ջ��ź�����ֵ
		int rxGainComp = 0;						// ����UE��ȡ�Ĳ����ŵ����ջ�����ֵ�����������Ĺ��ʣ��ټ�ȥ���������źţ��õ������ŵ����ջ�����
		double compRssi[RX_GAIN_NUM] = { 0 };

		// ���������ŵ��Ż�ȡ��Ӧ��������Ƶ��
		ret = NurCommonFunc::GetFreqFromDLChan(config->centerChannel[i], centerFreqDL, centerFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "�����ŵ��ŵ���Ϊ�Ƿ����ݣ��ŵ���Ϊ %d", config->centerChannel[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "���������ŵ��Ż�ȡ��Ӧ��������Ƶ��ɹ��������ŵ���Ϊ %d����������Ƶ��Ϊ %.1f MHz����������Ƶ��Ϊ %.1f MHz", config->centerChannel[i], centerFreqDL, centerFreqUL);
			m_logOutput(logBuff);
		}

		//----------------------------------------------------------------- ��Ƶ��Ƶ�ʲ���У׼	---------------------------------------------//
		double compFreqDL = 0;					// ����Ƶ�����е�ĳ��Ƶ��
		double compFreqUL = 0;					// ����Ƶ�����е�ĳ��Ƶ��
		// ���������ŵ��Ż�ȡ��Ӧ��������Ƶ��
		ret = NurCommonFunc::GetFreqFromDLChan(config->compChannelHigh[i], compFreqDL, compFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "Ƶ�ʲ����ŵ��ŵ���Ϊ�Ƿ����ݣ��ŵ���Ϊ %d", config->compChannelHigh[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "���������ŵ��Ż�ȡ��Ӧ��������Ƶ��ɹ��������ŵ���Ϊ %d����������Ƶ��Ϊ %.1f MHz����������Ƶ��Ϊ %.1f MHz", config->compChannelHigh[i], compFreqDL, compFreqUL);
			m_logOutput(logBuff);
		}

		// ��UE��ȡRSSI
		ret = GetRSSI(compFreqDL, config->compFreqPathLossHighDL[i], config->agcPower[RX_GAIN_NUM - 1], rssiComp);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "��UE��ȡRSSIʧ�ܣ�");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "��UE��ȡRSSI�ɹ���RSSIΪ %d", rssiComp);
			m_logOutput(logBuff);
		}

		// ��UE���ȡ���ջ�������ź�ǿ�ȣ�rssi�͹��ʵĻ����ϵ��Ҫ��������������ֻ�Ǹ���һ������
		rxGainComp = (int)rssiComp - config->agcPower[RX_GAIN_NUM - 1];

		// ������ջ����油��б�ʣ���λ��dB / 100kHz������AGC���ľ�����1dB����˲���ֵ��AGC���������±���һ�µ�
		agcSlope_High[i] = (rxGainComp - rxGain[RX_GAIN_NUM - 1]) / (config->compChannelHigh[i] - config->centerChannel[i]);
		for (int k = RFC_FREQ_NUM / 2; k < RFC_FREQ_NUM; k++)
		{
			nvTable.info[i].rfc.info[k].frequency = short((freqDLMax - centerFreqDL) * 10 * (k - double(RFC_FREQ_NUM / 2) + 1) / (RFC_FREQ_NUM / 2));
			nvTable.info[i].rfc.info[k].comp = char(nvTable.info[nvIndex].rfc.info[k].frequency * agcSlope_High[i]);
		}

		//----------------------------------------------------------------- ��Ƶ��Ƶ�ʲ���У׼	---------------------------------------------//

		// ���������ŵ��Ż�ȡ��Ӧ��������Ƶ��
		ret = NurCommonFunc::GetFreqFromDLChan(config->compChannelLow[i], compFreqDL, compFreqUL);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "Ƶ�ʲ����ŵ��ŵ���Ϊ�Ƿ����ݣ��ŵ���Ϊ %d", config->compChannelLow[i]);
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "���������ŵ��Ż�ȡ��Ӧ��������Ƶ��ɹ��������ŵ���Ϊ %d����������Ƶ��Ϊ %.1f MHz����������Ƶ��Ϊ %.1f MHz", config->compChannelLow[i], compFreqDL, compFreqUL);
			m_logOutput(logBuff);
		}

		// ��UE��ȡRSSI
		ret = GetRSSI(compFreqDL, config->compFreqPathLossLowDL[i], config->agcPower[RX_GAIN_NUM - 1], rssiComp);
		if (ret == FAIL)
		{
			sprintf_s(logBuff, "��UE��ȡRSSIʧ�ܣ�");
			m_logOutput(logBuff);
			return ret;
		}
		else
		{
			sprintf_s(logBuff, "��UE��ȡRSSI�ɹ���RSSIΪ %d", rssiComp);
			m_logOutput(logBuff);
		}

		// ��UE���ȡ���ջ�������ź�ǿ�ȣ�rssi�͹��ʵĻ����ϵ��Ҫ��������������ֻ�Ǹ���һ������
		rxGainComp = (int)rssiComp - config->agcPower[RX_GAIN_NUM - 1];

		// ������ջ����油��б�ʣ���λ��dB / 100kHz������AGC���ľ�����1dB����˲���ֵ��AGC���������±���һ�µ�
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
* Function��InitTransmitter
* Description: Init instrument measurement and UE to make UE transmitter initialization successful
* Input:
* APCConf* : config
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History��
Name	Date		Contents Reason
Jason	2019-3-29	Add Iniransmitter initialization
.
.
********************************************************************/
int NurBLCal::InitTransmitter(APCConf* config)
{
	int ret = FAIL;											// У׼���
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log��ӡ����

	// ��ʼ���Ǳ����ò���
	ret = m_pInstru->InitInstrument();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ʼ���Ǳ����ò���ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ʼ���Ǳ����ò����ɹ���");
		m_logOutput(logBuff);
	}

	// ��ʼ��UE�����������Ƶ�㡢APC�����ֺ�PA��λ��Ҫ��configҳ���ȡ
	if (m_pDUT->OpenTransmitter(config->freq, config->paMode, config->initApcData))
	{
		sprintf_s(logBuff, "��UE������ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "��UE�����ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	return PASS;
}

/********************************************************************
* Function��InitReceiver
* Description: Init instrument generator and UE to make UE receiver initialization successful
* Input:
* AGCConf* : config
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History��
Name	Date		Contents Reason
Jason	2019-3-29	Add receiver initialization
.
.
********************************************************************/
int NurBLCal::InitReceiver(AGCConf* config)
{
	int ret = 0;											// У׼���
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log��ӡ����

	// ��ʼ���Ǳ����ò���
	ret = m_pInstru->InitInstrument();
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "��ʼ���Ǳ����ò���ʧ�ܣ�");
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "��ʼ���Ǳ����ò����ɹ���");
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
	if (m_pDUT->OpenReceiver(config->freq, config->rxGainMode))
	{
		sprintf_s(logBuff, "��UE���ջ��ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "��UE���ջ�ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	return PASS;
}

/********************************************************************
* Function��GetRSSI
* Description: set instrument generator and UE to get RSSI
* Input:
* AGCConf* : config
* int : thread index
* Output:
* Return: int. 0: pass. Otherwise: fail.
* History��
Name	Date		Contents Reason
Jason	2019-3-30	Add function to get RSSI
.
.
********************************************************************/
int NurBLCal::GetRSSI(double compFreqDL, double pathLoss, double cellPower, int& rssi)
{
	int ret = 0;											// У׼���
	char logBuff[LOG_BUFF_LEN] = { '\0' };								// log��ӡ����

	// �����Ǳ�����е����й���Ƶ��
	ret = m_pInstru->SetGeneratorFreq(compFreqDL);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�����е����й���Ƶ��ʧ�ܣ�Ƶ��Ϊ %.1f MHz", compFreqDL);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�����е����й���Ƶ�ʳɹ���Ƶ��Ϊ %.1f MHz", compFreqDL);
		m_logOutput(logBuff);
	}

	// �����Ǳ���������
	ret = m_pInstru->SetGeneratorLoss(pathLoss);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�����е���������ʧ�ܣ�����Ϊ %.1f dB", pathLoss);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�����е���������ɹ�������Ϊ %.1f dB", pathLoss);
		m_logOutput(logBuff);
	}

	// �����Ǳ�����е����й���
	ret = m_pInstru->SetGeneratorPower(cellPower);
	if (ret == FAIL)
	{
		sprintf_s(logBuff, "�����Ǳ�����е����й���ʧ�ܣ�����Ϊ %.1f dBm", cellPower);
		m_logOutput(logBuff);
		return ret;
	}
	else
	{
		sprintf_s(logBuff, "�����Ǳ�����е����й��ʳɹ�������Ϊ %.1f dBm", cellPower);
		m_logOutput(logBuff);
	}

	// ���ý��ջ�����Ƶ��
	if (m_pDUT->SetReceiverFrequency(compFreqDL))
	{
		sprintf_s(logBuff, "����UE����Ƶ��ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "����UE����Ƶ��ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	// ����UE���ջ�����״̬
	if (m_pDUT->SetReceiverGain(RX_GAIN_NUM - 1))
	{
		sprintf_s(logBuff, "����UE���ջ�����״̬�ɹ�");
		m_logOutput(logBuff);
	}
	else
	{
		sprintf_s(logBuff, "����UE���ջ�����״̬ʧ��");
		m_logOutput(logBuff);
		return FAIL;
	}

	// ��ȡRSSI


	return PASS;
}