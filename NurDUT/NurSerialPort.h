#pragma once
#include "stdafx.h"
class CSerialPortService
{
	HANDLE m_hCom;
	bool m_bOpen;
	bool m_bAsync;

public:
	CSerialPortService();
	CSerialPortService(bool async);
	~CSerialPortService(void);

	void setAsync(bool async);

	DWORD open(DWORD nBaudRate, LPCTSTR portName);
	DWORD close();

	DWORD write(BYTE *pData, DWORD nDataLen);
	DWORD read(BYTE *pData, DWORD nDataLen);
};