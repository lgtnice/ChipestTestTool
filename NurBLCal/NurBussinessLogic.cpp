// NurBussinessLogic.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "NurBLCal.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ColloCalNamespace
{
	public delegate void PrintLogDelegate(String^);
	public ref class ColloCal
	{
	private:
		NurBLCal* m_ptr;
	public:
		ColloCal() :m_ptr(new NurBLCal) {}
		~ColloCal()
		{
			delete m_ptr;
			m_ptr = nullptr;
		}

		int SetLogPtr(PrintLogDelegate ^func)
		{
			IntPtr p = Marshal::GetFunctionPointerForDelegate(func);
			return m_ptr->SetLogPtr((SEND_LOG_TO_UI)p.ToInt32());
		}

		int InitDUTAPI()
		{
			return m_ptr->InitDUTAPI();
		}
		int ReleaseDUTAPI()
		{
			return m_ptr->ReleaseDUTAPI();
		}
		int InitInstrumentAPI()
		{
			return m_ptr->InitInstrumentAPI();
		}
		int ReleaseInstrumentAPI()
		{
			return m_ptr->ReleaseInstrumentAPI();
		}
		int OpenSerialPortEX(int baudRate,String^ serialPort)
		{
			pin_ptr<const wchar_t> serialPortEX = PtrToStringChars(serialPort);
			return m_ptr->OpenSerialPortEX(baudRate,serialPortEX);
		}
		int CloseSerialPortEX()
		{
			return m_ptr->CloseSerialPortEX();
		}
		int OpenInstrument(String^ intrumentIP )
		{
			pin_ptr<const wchar_t> intrumentIPEX = PtrToStringChars(intrumentIP);
			return m_ptr->OpenInstrument(intrumentIPEX);
		}
		int ReleaseInstrument()
		{
			return m_ptr->ReleaseInstrument();
		}

		int AFCCal(IntPtr p)
		{
			return m_ptr->AFCCal((AFCConf*)p.ToInt32());
		}
		int APCCal(IntPtr p)
		{
			return m_ptr->APCCal((APCConf*)p.ToInt32());
		}
		int AGCCal(IntPtr p)
		{
			return m_ptr->AGCCal((AGCConf*)p.ToInt32());
		}

	};
}