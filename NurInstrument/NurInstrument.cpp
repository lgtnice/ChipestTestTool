// NurInstrument.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "NurInstrument.h"


/**
* Constructor
* @return void
*/
CNurCMW500::CNurCMW500(void)
{
	m_viDefaultRM = NULL;
	m_viInstruHandle = NULL;
	m_viStatus = -1;
	memset(m_szCmdBuf, 0x0, sizeof(m_szCmdBuf));
	memset(m_szRetValue, 0x0, sizeof(m_szRetValue));
}

/**
* Destructor
* @return void
*/
CNurCMW500::~CNurCMW500(void)
{
}

/**
* Function: Initial Instrument via LAN
* Preconditions: NULL
* Expectation: Initial instrument complete
* @return int 0:success, otherwise:fail
* @param [in] pInstrumentIP: Instrument IP
*/
ViStatus CNurCMW500::OpenInstru(const char* pInstrumentIP)
{
	//VISA function
	//Returns a call with the default resource manager
	//m_viDefaultRM: Unique logical identifier for a call to the default resource manager
	if (VI_SUCCESS != viOpenDefaultRM(&m_viDefaultRM))
	{
		return -1;
	}

	//LAN mode
	//Get instrument device ID
	sprintf(m_szCmdBuf, "TCPIP::%s::inst0::INSTR", pInstrumentIP);

	//VISA function
	//Open the call to the specified instrument
	//m_viInstruHandle: Instrument handle
	m_viStatus = viOpen(m_viDefaultRM, m_szCmdBuf, VI_NULL, VI_NULL, &m_viInstruHandle);
	if (m_viStatus != VI_SUCCESS)
	{
		viClose(m_viDefaultRM);
		return m_viStatus;
	}
	else
	{
		//VISA function
		//Set VISA property
		//VI_ATTR_TMO_VALUE: Timeout period, Unit: ms
		m_viStatus = viSetAttribute(m_viInstruHandle, VI_ATTR_TMO_VALUE, 20000);
		memset(m_szCmdBuf, 0, BUF_LEN);

		//Query identification
		//Return the serial number of the UXM and the current revision of the LTE/LTE-A application
		sprintf(m_szCmdBuf, "*IDN?\n");
		m_viStatus = viQueryf(m_viInstruHandle, m_szCmdBuf, "%t", m_szRetValue);
		if (m_viStatus != VI_SUCCESS)
		{
			//VISA function
			//Close the specified call
			viClose(m_viInstruHandle);
			return m_viStatus;
		}
	}

	//Clear status
	sprintf(m_szCmdBuf, "*CLS\n");
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	//Reset instrument
	sprintf(m_szCmdBuf, "*RST\n");
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	//Operation complete
	sprintf(m_szCmdBuf, "*OPC?\n");
	m_viStatus = viQueryf(m_viInstruHandle, m_szCmdBuf, "%t", m_szRetValue);

	return m_viStatus;
}

/**
* Function: Close Instrument
* Preconditions: Initial Instrument via  LAN
* Expectation: Release instrument via  LAN complete
* @return int 0: success, otherwise: fail
*/
ViStatus CNurCMW500::CloseInstru()
{
	if (m_viInstruHandle != NULL)
	{
		//VISA function
		//Instrument clear
		m_viStatus = viClear(m_viInstruHandle);

		//VISA function
		//Close the specified call
		m_viStatus = viClose(m_viInstruHandle);
		m_viInstruHandle = NULL;
	}
	if (m_viDefaultRM != NULL)
	{
		//VISA function
		//Close the specified call
		m_viStatus = viClose(m_viDefaultRM);
		m_viDefaultRM = NULL;
	}
	return m_viStatus;
}


ViStatus CNurCMW500::InitInstrument()
{
	m_viStatus = -1;
	
	// NB-IoT普通校准仪表配置项
	// route
	m_viStatus = viPrintf(m_viInstruHandle, "ROUT:GPRF:GEN:SCEN:SAL RFAC, TX1\n");
//	m_viStatus |= viPrintf(m_viInstruHandle, "ROUT:GPRF:MEAS:SCEN:SAL RFAC,RX1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "ROUT:NIOT:MEAS:SCEN:SAL RFAC, RX1\n");

	// NB-IoT measure, for AFC
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:DMOD FDD\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:PLCid 0\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:MOD:MSCH QPSK\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:REP SING\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEAS:MEV:NPFORmat F1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SCSPacing S15K\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:RFSettings:ENPower 23\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SCSPacing S3K75\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SCON NONE\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:MOEX OFF\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:MOD:MSCH BPSK\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SUBCarrier 1, 0\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:NREPetitions NR1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SCOunt:MODulation 30\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:NSLots 30\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:NRUNits NRU02\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:RESult:TXM ON\n");

	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:SCO:POW 5 \n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:SCO:PDYN 1 \n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:RES:ALL OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:RFS:UMAR 5\n");

	// 常发时设为'Free Run (Fast Sync)',按时隙发时设为'IF Power'
	m_viStatus |= viPrintf(m_viInstruHandle, "TRIG:NIOT:MEAS:POW:SOUR 'IF Power'\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "TRIG:NIOT:MEAS:POW:SLOP REDG\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "TRIG:NIOT:MEAS:MEV:TOUT 0.5\n");

	// GPRF generator, for AGC
	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:STAT OFF\n");
	// AGC使用CW波校准即可
	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:BBM CW\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:ARB:REPetition CONTINUOUS\n");


	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:STATe OFF\n");
	m_viStatus |= viQueryf(m_viInstruHandle, "*OPC?\n", "%t", m_szRetValue);
	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:RFSettings:LEVel -110\n");

	// 常发时设为0,按时隙发时设为非0

	return m_viStatus;
}

ViStatus CNurCMW500::SetMeasurementFreq(double Freq, BOOL bCWSignal)
{
	m_viStatus = -1;

	if (bCWSignal)
	{
		sprintf(m_szCmdBuf, "CONFigure:GPRF:MEAS:RFSettings:FREQuency %fMHz\n", Freq);
	}
	else
	{
		// 设置仪表界面的LTE配置界面中“Frequency”选项
		sprintf(m_szCmdBuf, "CONFigure:NIOT:MEAS:RFSettings:FREQuency %fMHz\n", Freq);
	}
	
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	return m_viStatus;
}

ViStatus CNurCMW500::SetGeneratorFreq(double Freq)
{
	m_viStatus = -1;

	// 设置信号源配置界面中的“Frequency”选项
	sprintf(m_szCmdBuf, "SOURce:GPRF:GEN:RFSettings:FREQuency %fMHz\n", Freq);
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	return m_viStatus;
}

ViStatus CNurCMW500::SetMeasurementLoss(double loss, BOOL bCWSignal)
{
	m_viStatus = -1;

	if (bCWSignal)
	{
		sprintf(m_szCmdBuf, "CONFigure:GPRF:MEAS:RFSettings:EATTenuation %f\n", loss);
	}
	else
	{
		// 设置仪表界面的LTE配置界面中“External Attenuation (Input)”选项
		sprintf(m_szCmdBuf, "CONFigure:NIOT:MEAS:RFSettings:EATTenuation %f\n", loss);
	}
	

	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	return m_viStatus;
}

ViStatus CNurCMW500::SetGeneratorLoss(double loss)
{
	m_viStatus = -1;

	// 设置信号源配置界面中的“Ext.Att. (Output)”选项
	sprintf(m_szCmdBuf, "SOURce:GPRF:GEN:RFSettings:EATTenuation %f\n", loss);
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	return m_viStatus;
}

ViStatus CNurCMW500::SetExpPower(double power, BOOL bCWSignal)
{
	m_viStatus = -1;

	if (power > 40)
	{
		power = 40;
	}
	if (power < -40)
	{
		power = -40;
	}

	if (bCWSignal)
	{
		sprintf(m_szCmdBuf, "CONFigure:GPRF:MEAS:RFSettings:ENPower %f\n", power);
	}
	else
	{
		// 设置仪表界面的LTE配置界面中“Expected Nominal Power”选项
		sprintf(m_szCmdBuf, "CONFigure:NIOT:MEAS:RFSettings:ENPower %f\n", power);
	}

	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	return m_viStatus;
}

ViStatus CNurCMW500::SetGeneratorPower(double power)
{
	m_viStatus = -1;

	// 设置信号源配置界面中的“Level (RMS)”选项
	sprintf(m_szCmdBuf, "SOURce:GPRF:GEN:RFSettings:LEVel %f\n", power);
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	return m_viStatus;
}

ViStatus CNurCMW500::GetFreqErr(double& freqErr, double& power)
{
	m_viStatus = -1;
	double data[30] = { 0 };

	for (int i = 0; i < 10; i++)
	{
		memset(m_szRetValue, 0, sizeof(m_szRetValue));
		m_viStatus = viQueryf(m_viInstruHandle, "READ:NIOT:MEAS:MEV:MOD:CURRent?\n", "%t", m_szRetValue);

		(void)StringToDouble(data, m_szRetValue);

		if (data[0] == 0)
		{
			break;
		}
		else if (data[0] == 3)
		{
			m_viStatus = viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:RFS:ENP %f\n", power + i * 3);
		}
		else if (data[0] == 4)
		{
			m_viStatus = viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:RFS:ENP %f\n", power - i * 3);
		}
		else
		{
			return (ViStatus)data[0];
		}

	}
	if (data[0] != 0)
	{
		return (ViStatus)data[0];
	}
	else
	{
		freqErr = data[15];
		power = data[17];
	}
	return m_viStatus;
}

ViStatus CNurCMW500::GetTXPower(double &fTXPower, BOOL bCWSignal)
{
	m_viStatus = -1;
	double data[100] = { 0 };

	memset(m_szRetValue, 0, sizeof(m_szRetValue));

	if (bCWSignal)
	{
		m_viStatus = viQueryf(m_viInstruHandle, "READ:GPRF:MEAS:POW:AVER?\n", "%t", m_szRetValue);
	}
	else
	{
		m_viStatus |= viQueryf(m_viInstruHandle, "READ:NIOT:MEAS:MEV:TRACe:PMONitor?\n", "%t", m_szRetValue);
	}

	(void)StringToDouble(data, m_szRetValue);
	if (data[0] != 0)
	{
		return int(data[0]);
	}
	fTXPower = data[1];

	return m_viStatus;
}

/**
* Function:  Turn on generator
* Preconditions: Generator initialization complete
* Expectation: Generator status is ON
* @return	int 0: success, otherwise: fail
* @param [in] bOn					Expectation of generator status
* @param [in/out] iOK				Turn on or generator result:TRUE(1)is success, FALSE(0) is fail
*/
ViStatus CNurCMW500::TurnOnGenerator()
{
	m_viStatus = -1;

	//Open or close cell output
	sprintf(m_szCmdBuf, "SOURce:GPRF:GENerator:STATe ON\n");
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	for (int i = 0; i < 30; i++)
	{
		m_viStatus = viQueryf(m_viInstruHandle, "SOURce:GPRF:GENerator:STATe?\n", "%t", m_szRetValue);

		//OFF: generator switched off
		//ON: generator has been turned
		//PENDing: the generator has been turned on (off) but the signal is not yet (still) available
		if (_strnicmp(m_szRetValue, "ON", 2) == 0)
		{
			return 0;
		}
		else
		{
			Sleep(1000);
			continue;
		}
	}
	
	return -1;
}

/**
* Function:  Turn off generator
* Preconditions: Generator initialization complete
* Expectation: Generator status is Off
* @return	int 0: success, otherwise: fail
* @param [in] bOn					Expectation of generator status
* @param [in/out] iOK				Turn off generator result:TRUE(1)is success, FALSE(0) is fail
*/
ViStatus CNurCMW500::TurnOffGenerator()
{
	m_viStatus = -1;

	//Open or close cell output
	sprintf(m_szCmdBuf, "SOURce:GPRF:GENerator:STATe OFF\n");
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	for (int i = 0; i < 30; i++)
	{
		m_viStatus = viQueryf(m_viInstruHandle, "SOURce:GPRF:GENerator:STATe?\n", "%t", m_szRetValue);

		//OFF: generator switched off
		//ON: generator has been turned
		//PENDing: the generator has been turned on (off) but the signal is not yet (still) available
		if (_strnicmp(m_szRetValue, "OFF", 3) == 0)
		{
			return 0;
		}
		else
		{
			Sleep(1000);
			continue;
		}
	}

	return -1;
}

ViStatus CNurCMW500::InitNSFT()
{
	m_viStatus = -1;

	// NB-IoT普通校准仪表配置项
	// route
	m_viStatus = viPrintf(m_viInstruHandle, "ROUT:GPRF:GEN:SCEN:SAL RFAC, TX1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "ROUT:NIOT:MEAS:SCEN:SAL RFAC, RX1\n");

	// NB-IoT measure, for AFC
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:DMOD FDD\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:PLCid 0\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:MOD:MSCH QPSK\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:REP SING\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEAS:MEV:NPFORmat F1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SCSPacing S15K\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:RFSettings:ENPower 23\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SCSPacing S3K75\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SCON NONE\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:MOEX OFF\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:MOD:MSCH BPSK\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SUBCarrier 1, 0\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:NREPetitions NR1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:SCOunt:MODulation 30\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:NSLots 30\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:NRUNits NRU02\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONFigure:NIOT:MEASurement:MEValuation:RESult:TXM ON\n");

	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:SCO:POW 5 \n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:SCO:PDYN 1 \n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:MEV:RES:ALL OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "CONF:NIOT:MEAS:RFS:UMAR 5\n");

	// 常发时设为'Free Run (Fast Sync)',按时隙发时设为'IF Power'
	m_viStatus |= viPrintf(m_viInstruHandle, "TRIG:NIOT:MEAS:POW:SOUR 'IF Power'\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "TRIG:NIOT:MEAS:POW:SLOP REDG\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "TRIG:NIOT:MEAS:MEV:TOUT 0.5\n");

	// GPRF generator, for AGC
	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:STAT OFF\n");

	// NSFT使用调制波
	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:BBM ARB\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "SOURce:GPRF:GENerator:ARB:FILE 'C:\ProgramData\Rohde-Schwarz\CMW\Data\waveform\library1\KV110_NB_IOT_DCI_160_DCI_N1_40_ms.wv'\n");
	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:ARB:REPetition CONTINUOUS\n");


	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:STATe OFF\n");
	m_viStatus |= viQueryf(m_viInstruHandle, "*OPC?\n", "%t", m_szRetValue);
	m_viStatus |= viPrintf(m_viInstruHandle, "SOUR:GPRF:GEN:RFSettings:LEVel -110\n");

	// 常发时设为0,按时隙发时设为非0

	return m_viStatus;
}

ViStatus CNurCMW500::NSFTHandover(double FreqDL, double FreqUL)
{
	m_viStatus = -1;

	
	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:RFSettings:FREQuency %.1f\n", FreqUL);
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "SOURce:GPRF:GENerator:RFSettings:FREQuency %.1f\n", FreqDL);
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	return m_viStatus;
}

ViStatus CNurCMW500::NSFTSetMeasurement(unsigned char mcs, unsigned char toneNum, unsigned char toneStart, unsigned char subCarrierSpacing, bool onlyPowerMeas)
{
	m_viStatus = -1;

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:RFSettings:ENPower 30\n");
	m_viStatus = viPrintf(m_viInstruHandle, m_szCmdBuf);

	if (SCSP_15K == subCarrierSpacing)
	{
		sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:SCSPacing S15K\n");
	}
	else
	{
		sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:SCSPacing S3K75\n");
	}
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	if (onlyPowerMeas)
	{
		sprintf(m_szCmdBuf, "CONFigure:NIOT:MEAS:MEValuation:RESult:ALL OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF\n");
	}
	else
	{
		sprintf(m_szCmdBuf, "CONFigure:NIOT:MEAS:MEValuation:RESult:ALL ON,ON,ON,ON,OFF,OFF,OFF,OFF,OFF,OFF\n");
	}
	
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:REPetition SINGleshot\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "TRIG:NIOT:MEAS:MEV:TOUT 0.5\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:SCON NONE\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:MOEX OFF\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	if (BPSK == mcs)
	{
		sprintf(m_szCmdBuf, "CONF:NIOT:MEAS:MEV:MOD:MSCH BPSK\n");
	}
	else
	{
		sprintf(m_szCmdBuf, "CONF:NIOT:MEAS:MEV:MOD:MSCH QPSK\n");
	}
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEAS:MEV:NPFORmat F1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:SUBCarrier %d, %d\n", toneNum, toneStart);
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:SCOunt:POWer 1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:NREPetitions NR1\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "TRIG:NIOT:MEAS:MEV:SOUR 'GPRF Gen1: Waveform Marker 3'\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:SCOunt:MODulation 30\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:NSLots 30\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:NRUNits NRU01\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	sprintf(m_szCmdBuf, "CONFigure:NIOT:MEASurement:MEValuation:RESult:TXM ON\n");
	m_viStatus |= viPrintf(m_viInstruHandle, m_szCmdBuf);

	return m_viStatus;
}

ViStatus CNurCMW500::NSFTTXMeasurement()
{
	m_viStatus = -1;
	double dataExtreme[30] = { 0 };
	double dataAverage[30] = { 0 };
	for (int i = 0; i < 30; i++)
	{
		m_viStatus = viQueryf(m_viInstruHandle, "FETCh:NIOT:MEASurement:MEValuation:STATe?\n", "%t", m_szRetValue);

		//OFF: generator switched off
		//ON: generator has been turned
		//PENDing: the generator has been turned on (off) but the signal is not yet (still) available
		if (_strnicmp(m_szRetValue, "RDY", 3) == 0)
		{
			break;
		}
		else
		{
			if (i == 29) {
				return -1;
			}
			Sleep(1000);
			continue;
		}
	}

	return m_viStatus;
}

ViStatus CNurCMW500::ReadPower(double& power)
{
	m_viStatus = -1;
	double tempData[128] = { 0 };

	m_viStatus = viQueryf(m_viInstruHandle, "FETCh:LTE:MEASurement:MEValuation:SEMask:AVERage?\n", "%t", m_szRetValue);

	(void)StringToDouble(tempData, m_szRetValue);

	m_viStatus = (int)tempData[0];
	if (0 != m_viStatus)
	{
		return m_viStatus;
	}

	power = tempData[3];

	return m_viStatus;
}

ViStatus CNurCMW500::ReadACLR(double alcr[])
{
	m_viStatus = -1;
	double tempData[128] = { 0 };

	m_viStatus = viQueryf(m_viInstruHandle, "FETCh:LTE:MEAS:MEValuation:ACLR:AVERage?\n", "%t", m_szRetValue);

	if (NULL != strstr(m_szRetValue, "INV"))
	{
		return -1;

	}
	(void)StringToDouble(tempData, m_szRetValue);

	//The test result is available only when the first returned data is zero.
	if (tempData[0] > 0.000001 || tempData[0] < -0.000001)
	{
		return -1;
	}

	//对读出来的结果取反，和其他仪表兼容
	alcr[0] = 0 - tempData[3];  //-10M的值
	alcr[1] = 0 - tempData[5];  //+10M的值
	alcr[2] = 0 - tempData[2];  //-5M的值
	alcr[3] = 0 - tempData[6];  //+5M的值

	return m_viStatus;
}

ViStatus CNurCMW500::ReadMODulation(double& evm, double& freqErr)
{
	m_viStatus = -1;
	double tempData[128] = { 0 };

	m_viStatus = viQueryf(m_viInstruHandle, "FETCh:LTE:MEASurement:MEValuation:MODulation:AVERage?\n", "%t", m_szRetValue);

	(void)StringToDouble(tempData, m_szRetValue);

	m_viStatus = (int)tempData[0];
	if (0 != m_viStatus)
	{
		return m_viStatus;
	}

	//如果出现连续无效值，则直接返回
	if (NULL != strstr(m_szRetValue, "INV,INV,INV,INV,INV"))
	{
		return -1;
	}

	evm = tempData[3];
	freqErr = tempData[15];

	return m_viStatus;
}

ViStatus CNurCMW500::ReadSEMask(double& power, bool& sem, double& obw)
{
	m_viStatus = -1;
	double tempData[128] = { 0 };

	m_viStatus = viQueryf(m_viInstruHandle, "FETCh:LTE:MEASurement:MEValuation:SEMask:AVERage?\n", "%t", m_szRetValue); //同步PC和Test

	if (NULL != strstr(m_szRetValue, "INV"))
	{
		return -1;
	}

	(void)StringToDouble(tempData, m_szRetValue);

	m_viStatus = (int)tempData[0];
	if (0 != m_viStatus)
	{
		return m_viStatus;
	}

	if (tempData[1] > 0.000001 || tempData[1] < -0.000001)
	{
		sem = FALSE;
	}
	else
	{
		sem = TRUE;
	}

	power = tempData[3];
	obw = tempData[2] / 1000000;  //Hz to MHz

	return m_viStatus;
}

ViStatus CNurCMW500::ReadTXFreq(double &freq)
{
	m_viStatus = -1;
	double tempData[128] = { 0 };

	m_viStatus = viQueryf(m_viInstruHandle, "CONFigure:NIOT:MEAS:RFSettings:FREQuency?\n", "%t", m_szRetValue);
	(void)StringToDouble(tempData, m_szRetValue);

	if (0 != (int)tempData[0])
	{
		return -1;
	}

	freq = tempData[0];

	return m_viStatus;
}

void CNurCMW500::StringToDouble(double *doubleArray, char *stringData)
{
	int Lc = 0;
	int	Len = 0;
	char *Begb = NULL;
	char *Endb = NULL;
	char *ptr = NULL;
	char s1[262144] = "";
	char s2[50] = "";
	memset(s1, 0, sizeof(s1));
	strcpy(s1, stringData);
	strcat(s1, ",");
	Begb = s1;						//起始地址
	Endb = strchr(s1, ',');			//第一个逗号位置
	Lc = 0;
	do
	{
		Len = 0;
		Len = Endb - Begb;				//长度
		memset(s2, 0, sizeof(s2));		//截取两个逗号间的字符串
		strncpy(s2, Begb, Len);			//截取两个逗号间的字符串
		doubleArray[Lc] = atof(s2);		//string to double
		ptr = Endb + 1;					//指向下个数的起始地址
		strcpy(s1, ptr);				//去掉已转化的字符串
		Begb = s1;						//起始地址更新
		Endb = strchr(s1, ',');			//下个逗号位置
		Lc++;
	} while (Endb != NULL);
}

extern "C" __declspec(dllexport) CNurInstrumentInterface*  createInstrument()
{
	return new CNurCMW500();
}