#include "stdafx.h"
#include "NurSerialPort.h"


CSerialPortService::CSerialPortService()
{
	m_hCom = INVALID_HANDLE_VALUE;
	m_bOpen = false;
	setAsync(false);
}

CSerialPortService::CSerialPortService(bool async)
{
	m_hCom = INVALID_HANDLE_VALUE;
	m_bOpen = false;
	setAsync(async);
}


CSerialPortService::~CSerialPortService(void)
{
	(void)close();
}

void CSerialPortService::setAsync(bool async)
{
	m_bAsync = async;
}


DWORD CSerialPortService::open(DWORD BaudRate, LPCTSTR portName)
{
	m_hCom = CreateFile(
		portName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		(m_bAsync ? FILE_FLAG_OVERLAPPED : 0),
		NULL
	);
	if (INVALID_HANDLE_VALUE == m_hCom)
	{
		return 1;
	}

	SetupComm(m_hCom, 1024, 1024);

	DCB dcb;
	GetCommState(m_hCom, &dcb);
	dcb.BaudRate = BaudRate;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	BOOL setResult = SetCommState(m_hCom, &dcb);
	if (false == setResult)
	{
		return 2;
	}

	COMMTIMEOUTS TimeOuts;
	GetCommTimeouts(m_hCom,&TimeOuts);
	TimeOuts.ReadIntervalTimeout = 1000;
	TimeOuts.ReadTotalTimeoutConstant = 5000;
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	SetCommTimeouts(m_hCom, &TimeOuts);

	//清空串口的输入输出缓冲区
	PurgeComm(m_hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	m_bOpen = true;
	return 0;
}

//返回1，代表设备是关闭的，返回2，代表读取设备失败
DWORD CSerialPortService::read(BYTE *pData, DWORD nDataLen)
{
	if (false == m_bOpen)
	{
		return 1;
	}

	COMSTAT comStat;
	DWORD ErrorCode = 0;

	ClearCommError(m_hCom, &ErrorCode, &comStat);

	//DWORD dataCount = comStat.cbInQue;
	//
	//DWORD timeStart = GetTickCount();
	//DWORD prevTime = timeStart;
	//
	//while (GetTickCount() - timeStart < 1500)
	//{
	//	DWORD currTime = GetTickCount();

	//	ClearCommError(m_hCom, &ErrorCode, &comStat);
	//	
	//	if (ErrorCode > 0)
	//	{
	//		PurgeComm(m_hCom, PURGE_RXABORT | PURGE_RXCLEAR);
	//	}
	//	if (comStat.cbInQue >= nDataLen)
	//	{
	//		break;
	//	}
	//	
	//	if( ((currTime - prevTime) > 400)  && (comStat.cbInQue == dataCount) )
	//	{
	//		break;
	//	}
	//	if (comStat.cbInQue != dataCount)
	//	{
	//		prevTime = currTime;
	//		dataCount = comStat.cbInQue;
	//	}
	//}

	DWORD nDataReceived = 0;
	BOOL ret = ReadFile(m_hCom, pData, nDataLen, &nDataReceived, NULL);
	if ( false == ret)
	{
		return 2;
	}
	return 0;
}

//返回1，设备未打开,返回2，向设备写入失败
DWORD CSerialPortService::write(BYTE *pData, DWORD TransferDataLen)
{
	if (false == m_bOpen)
	{
		return 1;
	}

	DWORD ErrorCode = 0;
	ClearCommError(m_hCom, &ErrorCode, NULL);
	if (ErrorCode > 0)
	{
		PurgeComm(m_hCom, PURGE_RXABORT | PURGE_RXCLEAR);
	}
	DWORD ActualTransferDataLen = 0;
	BOOL ret = WriteFile(m_hCom, pData, TransferDataLen, &ActualTransferDataLen, NULL);
	if (FALSE == ret) {
		return 2;
	}
	return 0;
}

DWORD CSerialPortService::close()
{
	if (INVALID_HANDLE_VALUE == m_hCom)
	{
		return 0;
	}
	CloseHandle(m_hCom);
	m_hCom = INVALID_HANDLE_VALUE;
	m_bOpen = false;
	return 0;
}
