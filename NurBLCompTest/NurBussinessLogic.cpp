#include "stdafx.h"
#include "NurBLCompTest.h"


using namespace System;
using namespace System::Runtime::InteropServices;

namespace ColloTestNamespace
{
	public delegate void PrintLogDelegate(String^);


	public ref class ColloTest
	{
	private:
		NurBLTest* m_ptr;
	public:
		ColloTest() :m_ptr(new NurBLTest) {}
		~ColloTest()
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
		int OpenSerialPortEX(int baudRate, String^ serialPort)
		{
			pin_ptr<const wchar_t> serialPortEX = PtrToStringChars(serialPort);
			return m_ptr->OpenSerialPortEX(baudRate, serialPortEX);
		}
		int CloseSerialPortEX()
		{
			return m_ptr->CloseSerialPortEX();
		}
		int OpenInstrument(String^ intrumentIP)
		{
			pin_ptr<const wchar_t> intrumentIPEX = PtrToStringChars(intrumentIP);
			return m_ptr->OpenInstrument(intrumentIPEX);
		}
		int ReleaseInstrument()
		{
			return m_ptr->ReleaseInstrument();
		}
	};
}