#include "stdafx.h"
#include "NurDUTInstruction.h"
#include "stdlib.h"

#define MESSAGEMAXCAPACITY 512  //һ����Ϣ�������Ƕ��ٸ��ֽ�
#define CHIPCACHEMAXCAPACITY 4096  //оƬ������������������ֽ�


using namespace std;


DUTInstruction::DUTInstruction() {}
DUTInstruction::~DUTInstruction() {}


bool DUTInstruction::OpenSerialPort(DWORD BaudRate, LPCTSTR portName)
{
	DWORD result = m_serialPort.open(BaudRate, portName);
	if (0 == result)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DUTInstruction::CloseSerialPort()
{
	DWORD result = m_serialPort.close();
	if (0 == result)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//UEģʽ�л���modeΪ0��ʾ��ͨģʽ��Ϊ1��ʾ����ģʽ
bool DUTInstruction::SetDUTMode(unsigned char mode)
{
	//data����
	const int dataLength = sizeof(mode);
	unsigned char data[dataLength];
	memcpy(data,&mode,dataLength);

	//���ؽ�������
	BYTE* readData = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readData,data, dataLength, 0x00, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readData);
		return false;
	}
	free(readData);
	return true;
}

//д��AFC���
bool DUTInstruction::WriteAFCCodeTable(NurRFNVCalAfc codeTable)
{
	//data����
	const int dataLength = sizeof(codeTable);
	unsigned char data[dataLength];
	memcpy(data,&codeTable,dataLength);

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x01, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//д��һ��Ƶ�ε����
bool DUTInstruction::WriteBandCodeTable(unsigned char band, NurRFNVCalInfo codeTable)
{
	//band���ֺ�data����
	const int dataLength = sizeof(codeTable) + sizeof(band);
	BYTE data[dataLength];
	memcpy(data,&band,sizeof(band));
	memcpy(data + sizeof(band), &codeTable, sizeof(codeTable));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x02, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//д��ȫ���
bool DUTInstruction::WriteAllCodeTable(NurRFNVCal codeTable)
{
	//data����
	const int dataLength = sizeof(codeTable);
	unsigned char data[dataLength];
	memcpy(data, &codeTable, dataLength);

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x03, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//д���־λ
bool DUTInstruction::WriteTagBit(unsigned char flagID, unsigned char flag)
{
	//data����
	const int dataLength = sizeof(flagID) + sizeof(flag);
	unsigned char data[dataLength];
	memcpy(data, &flagID, sizeof(flagID));
	memcpy(data + sizeof(flagID), &flag, sizeof(flag));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x04, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//���������RAM
bool DUTInstruction::UpdateCodeTableToRAM()
{
	//data����
	const int dataLength = 0;
	unsigned char* data = nullptr;
	
	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x05, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//��ȡAFC���
bool DUTInstruction::ReadAFCCodeTable(NurRFNVCalAfc& codeTable)
{
	//data����
	const int dataLength = sizeof(codeTable);
	unsigned char data[dataLength];
	memcpy(data, &codeTable, dataLength);

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x81, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&codeTable,readResult,dataLength);
	free(readResult);
	return true;
}

//��ȡһ��Ƶ�ε����
bool DUTInstruction::ReadSpecialBandCodeTable(unsigned char& band, NurRFNVCalInfo& codeTable)
{
	//band���ֺ�data����
	const int dataLength = sizeof(codeTable) + sizeof(band);
	BYTE data[dataLength];
	memcpy(data, &band, sizeof(band));
	memcpy(data + sizeof(band), &codeTable, sizeof(codeTable));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x82, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&band, readResult, sizeof(band));
	memcpy(&codeTable,readResult + sizeof(band),sizeof(codeTable));
	free(readResult);
	return true;
}

//��ȡȫ���
bool DUTInstruction::ReadAllCodeTable(NurRFNVCal& codeTable)
{
	//data����
	const int dataLength = sizeof(codeTable);
	unsigned char data[dataLength];
	memcpy(data, &codeTable, dataLength);

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x83, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&codeTable, readResult, dataLength);
	free(readResult);
	return true;
}

// ��ȡ��־λ
bool DUTInstruction::ReadTagBit(unsigned char& flagID, unsigned char& flag)
{
	//band���ֺ�data����
	const int dataLength = sizeof(flagID) + sizeof(flag);
	BYTE data[dataLength];
	memcpy(data, &flagID, sizeof(flagID));
	memcpy(data + sizeof(flagID), &flag, sizeof(flag));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x84, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&flagID, readResult, sizeof(flagID));
	memcpy(&flag, readResult + sizeof(flagID), sizeof(flag));
	free(readResult);
	return true;

}

//�򿪷����
bool DUTInstruction::OpenTransmitter(unsigned short freq, unsigned char pa, unsigned short vga)
{
	//data����
	const int dataLength = sizeof(freq) + sizeof(pa) + sizeof(vga);
	unsigned char data[dataLength];
	memcpy(data, &freq, sizeof(freq));
	memcpy(data + sizeof(freq), &pa, sizeof(pa));
	memcpy(data + sizeof(freq) + sizeof(pa), &vga, sizeof(vga));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x10, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;

}

//�رշ����
bool DUTInstruction::CloseTransmitter()
{
	//data����
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x11, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;

}

//���÷����Ƶ��
bool DUTInstruction::SetTransmitterFrequency(unsigned short freq)
{
	//data����
	const int dataLength = sizeof(freq);
	unsigned char data[dataLength];
	memcpy(data, &freq, sizeof(freq));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x12, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;

}

//���÷��������
bool DUTInstruction::SetTransmitterPower(unsigned char pa, unsigned short vga)
{
	//data����
	const int dataLength =sizeof(pa) + sizeof(vga);
	unsigned char data[dataLength];
	memcpy(data , &pa, sizeof(pa));
	memcpy(data + sizeof(pa), &vga, sizeof(vga));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x13, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//���÷����Ƶƫ������
bool DUTInstruction::SetTransmitterFrequencyOffset(unsigned short afc)
{
	//data����
	const int dataLength = sizeof(afc);
	unsigned char data[dataLength];
	memcpy(data, &afc, sizeof(afc));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x14, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//�򿪽��ջ�
bool DUTInstruction::OpenReceiver(unsigned short freq, unsigned char gain)
{
	//data����
	const int dataLength = sizeof(freq) + sizeof(gain);
	unsigned char data[dataLength];
	memcpy(data, &freq, sizeof(freq));
	memcpy(data + sizeof(freq), &gain, sizeof(gain));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x20, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//�رս��ջ�
bool DUTInstruction::CloseReceiver()
{
	//data����
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x21, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//���ý��ջ�Ƶ��
bool DUTInstruction::SetReceiverFrequency(unsigned short freq)
{
	//data����
	const int dataLength = sizeof(freq);
	unsigned char data[dataLength];
	memcpy(data, &freq, sizeof(freq));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x22, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//���ý��ջ�����״̬
bool DUTInstruction::SetReceiverGain(unsigned short gain)
{
	//data����
	const int dataLength = sizeof(gain);
	unsigned char data[dataLength];
	memcpy(data, &gain, sizeof(gain));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x23, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//��ȡ���ջ��ź�ǿ��
bool DUTInstruction::GetReceiverSignalStrength(unsigned short& rxpower)
{
	//data����
	const int dataLength = sizeof(rxpower);
	unsigned char data[dataLength];
	memcpy(data, &rxpower, sizeof(rxpower));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xA4, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&rxpower, readResult, dataLength);
	free(readResult);
	return true;
}

//����NSTģʽ
bool DUTInstruction::EnterNSTMode()
{
	//data����
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x40, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//�˳�NSTģʽ
bool DUTInstruction::ExitNSTMode()
{
	//data����
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x41, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST����ͬ��
bool DUTInstruction::NSTDownlinkSync(unsigned int channel, unsigned char mcs, unsigned char toneNum, unsigned char toneStart, unsigned char subCarrierSpacing, double power)
{
	//data����
	const int dataLength = sizeof(channel) + sizeof(mcs) + sizeof(toneNum) + sizeof(toneStart) + sizeof(subCarrierSpacing) + sizeof(power);
	unsigned char data[dataLength];
	memcpy(data, &channel, sizeof(channel));
	memcpy(data + sizeof(channel), &mcs, sizeof(mcs));
	memcpy(data + sizeof(channel) + sizeof(mcs), &toneNum, sizeof(toneNum));
	memcpy(data + sizeof(channel) + sizeof(mcs) + sizeof(toneNum), &toneStart, sizeof(toneStart));
	memcpy(data + sizeof(channel) + sizeof(mcs) + sizeof(toneNum) + sizeof(toneStart), &subCarrierSpacing, sizeof(subCarrierSpacing));
	memcpy(data + sizeof(channel) + sizeof(mcs) + sizeof(toneNum) + sizeof(toneStart) + sizeof(subCarrierSpacing), &power, sizeof(power));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x42, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST�������й���
bool DUTInstruction::NSTSetUplinkPower(double power)
{
	//data����
	const int dataLength = sizeof(power);
	unsigned char data[dataLength];
	memcpy(data, &power, sizeof(power));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x43, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST֪ͨUE׼���л�Ƶ��
bool DUTInstruction::NSTNoticeUEPrepareSwitchFrequency()
{
	//data����
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x44, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST�л��ŵ�
bool DUTInstruction::NSTSwitchChannel(unsigned char band, unsigned int channel)
{
	//data����
	const int dataLength = sizeof(band) + sizeof(channel);
	unsigned char data[dataLength];
	memcpy(data, &band, sizeof(band));
	memcpy(data + sizeof(band), &channel, sizeof(channel));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x45, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST����BLER���Բ���
bool DUTInstruction::NSTSetBLERArg(unsigned short subFrameNumber, unsigned short time)
{
	//data����
	const int dataLength = sizeof(subFrameNumber) + sizeof(time);
	unsigned char data[dataLength];
	memcpy(data, &subFrameNumber, sizeof(subFrameNumber));
	memcpy(data + sizeof(subFrameNumber), &time, sizeof(time));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x46, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//��ѯUE���Ǳ������״̬
bool DUTInstruction::InquireConnectStateUEInstr(unsigned char& status)
{
	//data����
	const int dataLength = sizeof(status);
	unsigned char data[dataLength];
	memcpy(data, &status, sizeof(status));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xC2, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&status, readResult, dataLength);
	free(readResult);
	return true;
}

//NST��ȡBLER
bool DUTInstruction::GetBLERValue(unsigned short& totalBlock, unsigned short& errorBlock)
{
	//data����
	const int dataLength = sizeof(totalBlock) + sizeof(errorBlock);
	unsigned char data[dataLength];
	memcpy(data, &totalBlock, sizeof(totalBlock));
	memcpy(data + sizeof(totalBlock), &errorBlock, sizeof(errorBlock));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xC6, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&totalBlock, readResult, sizeof(totalBlock));
	memcpy(&errorBlock, readResult + sizeof(totalBlock), sizeof(errorBlock));
	free(readResult);
	return true;
}

//NST��ȡRSRP
bool DUTInstruction::GetRSRPValue(unsigned short& rsrp)
{
	//data����
	const int dataLength = sizeof(rsrp);
	unsigned char data[dataLength];
	memcpy(data, &rsrp, sizeof(rsrp));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xC7, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&rsrp, readResult, sizeof(rsrp));
	free(readResult);
	return true;
}

//��GNSS
bool DUTInstruction::OpenGNSS(unsigned char tech, char band, unsigned short bandwidth, unsigned short time)
{
	//data����
	const int dataLength = sizeof(tech) + sizeof(band) + sizeof(bandwidth) + sizeof(time);
	unsigned char data[dataLength];
	memcpy(data, &tech, sizeof(tech));
	memcpy(data + sizeof(tech), &band, sizeof(band));
	memcpy(data + sizeof(tech) + sizeof(band), &bandwidth, sizeof(bandwidth));
	memcpy(data + sizeof(tech) + sizeof(band) + sizeof(bandwidth), &time, sizeof(time));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x60, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//�ر�GNSS
bool DUTInstruction::CloseGNSS()
{
	//data����
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x61, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//��ȡSNR
bool DUTInstruction::GetSNR(unsigned short& rsrp)
{
	//data����
	const int dataLength = sizeof(rsrp);
	unsigned char data[dataLength];
	memcpy(data, &rsrp, sizeof(rsrp));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xE1, this);
	if (false == result)
	{
		//���������г��ִ���
		free(readResult);
		return false;
	}
	memcpy(&rsrp, readResult, dataLength);
	free(readResult);
	return true;
}

//-----------------------------------------------------------------------------


//PC�ʹ����豸�Ľ���
bool InterAction(BYTE* readData, BYTE* data, const int dataLength, unsigned char ID, DUTInstruction* dut)
{
	//end����
	unsigned char end[1];
	end[0] = 0x00;

	//Data��������ֵ
	const int MaxDataLen = (MESSAGEMAXCAPACITY - sizeof(NurMessageHalf) - sizeof(end));

	int forCount = 0;

	for (int i = dataLength / MaxDataLen; i >= 0;i--, forCount++)
	{
		/*�ȷ��͸�UE*/
		NurMessageHalf messageHalf;
		messageHalf.Begin = '\n';
		const int MessageBodyLength = (  //��Ϣ��ĳ���
			(i == 0) 
			? (dataLength % MaxDataLen + sizeof(messageHalf.ID) + sizeof(messageHalf.ACK) + sizeof(messageHalf.Index) + sizeof(end))
			: (MESSAGEMAXCAPACITY - sizeof(messageHalf.Begin) - sizeof(messageHalf.Len))
			);
		messageHalf.Len[1] = (MessageBodyLength % 256) & 0xFF;
		messageHalf.Len[0] = (MessageBodyLength / 256) & 0xFF;
		messageHalf.ID = ID;
		messageHalf.ACK = 0x00;
		messageHalf.Index = i;

		BYTE FrontByteArray[sizeof(messageHalf)];
		memcpy(FrontByteArray, &messageHalf, sizeof(FrontByteArray));

		const int currentDataLen = MessageBodyLength - sizeof(messageHalf.ID) - sizeof(messageHalf.ACK) - sizeof(messageHalf.Index) - sizeof(end);
		BYTE* message = (BYTE*)malloc(sizeof(FrontByteArray) + currentDataLen + sizeof(end));
		NurCommonFunc::CombineByte(message, FrontByteArray, data + i * MaxDataLen, end, sizeof(FrontByteArray), currentDataLen, sizeof(end));
		DWORD writeReturn = dut->m_serialPort.write(message, (sizeof(FrontByteArray) + currentDataLen + sizeof(end)));
		if (writeReturn != 0)
		{
			free(message);
			//��Ϣд���豸ʧ��
			return false;
		}

		/*ȡ�豸����������ķ�����Ϣ*/
		BYTE* receiveMessage = (BYTE*)malloc(MessageBodyLength + sizeof(messageHalf.Begin) + sizeof(messageHalf.Len));
		DWORD readReturn = dut->m_serialPort.read(receiveMessage, MessageBodyLength + sizeof(messageHalf.Begin) + sizeof(messageHalf.Len));
		if (readReturn != 0)
		{
			free(message);
			free(receiveMessage);
			//��Ϣ��ȡʧ��
			return false;
		}
		NurMessageHalf SetDUTModeMessageReturn;
		memcpy(&SetDUTModeMessageReturn , receiveMessage ,sizeof(SetDUTModeMessageReturn));
		if (SetDUTModeMessageReturn.ACK != 0x00) 
		{
			free(message);
			free(receiveMessage);
			//������Ϣ��ack��0
			return false;
		}
		memcpy(readData + forCount * MaxDataLen, receiveMessage + sizeof(SetDUTModeMessageReturn), currentDataLen);
		free(message);
		free(receiveMessage);
	}
	return true;
}

//PC�ʹ����豸�Ľ���,�ر��
bool InterActionEX(BYTE* readData, BYTE* data, const int dataLength, unsigned char ID, int beginIndex, int cycleLength)
{
	//end����
	unsigned char end[1];
	end[0] = 0x00;

	//Data��������ֵ
	const int MaxDataLen = (MESSAGEMAXCAPACITY - sizeof(NurMessageHalf) - sizeof(end));

	int* divideStrategy = (int*)malloc(1*sizeof(int)); //data���ֵĻ��ֲ���
	int partCount = 1;  //��ʾ�����ָ����˶��ٸ�ֵ��
	int limit = 0;   //оƬ�����������ޣ�Ҳ��������оƬ������data���ֵĴ���
	for (BYTE* currentDataPtr = data ; (currentDataPtr - data) < dataLength ; partCount++)
	{
		int currentDataLength = MaxDataLen;   //data����Ϣ��󳤶ȵ�����
		bool dataRestrictOne = ((dataLength - (currentDataPtr - data)) < MaxDataLen);   //�Ѿ���data��β����
		bool dataRestrictTwo = (((currentDataPtr - data) + MaxDataLen) / CHIPCACHEMAXCAPACITY >= limit);    //�ٷ�һ�����data��Ҫ����оƬ������
		if (dataRestrictOne && (!dataRestrictTwo))
		{

		}
	}
	return true;
}


extern "C" __declspec(dllexport) CNurDUTInterface*  createDUT()
{
	return new DUTInstruction();
}