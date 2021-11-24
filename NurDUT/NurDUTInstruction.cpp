#include "stdafx.h"
#include "NurDUTInstruction.h"
#include "stdlib.h"

#define MESSAGEMAXCAPACITY 512  //一个消息最大可以是多少个字节
#define CHIPCACHEMAXCAPACITY 4096  //芯片缓冲区的最大容量，字节


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

//UE模式切换，mode为0表示普通模式，为1表示工厂模式
bool DUTInstruction::SetDUTMode(unsigned char mode)
{
	//data部分
	const int dataLength = sizeof(mode);
	unsigned char data[dataLength];
	memcpy(data,&mode,dataLength);

	//返回结果存放区
	BYTE* readData = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readData,data, dataLength, 0x00, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readData);
		return false;
	}
	free(readData);
	return true;
}

//写入AFC码表
bool DUTInstruction::WriteAFCCodeTable(NurRFNVCalAfc codeTable)
{
	//data部分
	const int dataLength = sizeof(codeTable);
	unsigned char data[dataLength];
	memcpy(data,&codeTable,dataLength);

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x01, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//写入一个频段的码表
bool DUTInstruction::WriteBandCodeTable(unsigned char band, NurRFNVCalInfo codeTable)
{
	//band部分和data部分
	const int dataLength = sizeof(codeTable) + sizeof(band);
	BYTE data[dataLength];
	memcpy(data,&band,sizeof(band));
	memcpy(data + sizeof(band), &codeTable, sizeof(codeTable));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x02, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//写入全码表
bool DUTInstruction::WriteAllCodeTable(NurRFNVCal codeTable)
{
	//data部分
	const int dataLength = sizeof(codeTable);
	unsigned char data[dataLength];
	memcpy(data, &codeTable, dataLength);

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x03, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//写入标志位
bool DUTInstruction::WriteTagBit(unsigned char flagID, unsigned char flag)
{
	//data部分
	const int dataLength = sizeof(flagID) + sizeof(flag);
	unsigned char data[dataLength];
	memcpy(data, &flagID, sizeof(flagID));
	memcpy(data + sizeof(flagID), &flag, sizeof(flag));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x04, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//更新码表至RAM
bool DUTInstruction::UpdateCodeTableToRAM()
{
	//data部分
	const int dataLength = 0;
	unsigned char* data = nullptr;
	
	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x05, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//读取AFC码表
bool DUTInstruction::ReadAFCCodeTable(NurRFNVCalAfc& codeTable)
{
	//data部分
	const int dataLength = sizeof(codeTable);
	unsigned char data[dataLength];
	memcpy(data, &codeTable, dataLength);

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x81, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&codeTable,readResult,dataLength);
	free(readResult);
	return true;
}

//读取一个频段的码表
bool DUTInstruction::ReadSpecialBandCodeTable(unsigned char& band, NurRFNVCalInfo& codeTable)
{
	//band部分和data部分
	const int dataLength = sizeof(codeTable) + sizeof(band);
	BYTE data[dataLength];
	memcpy(data, &band, sizeof(band));
	memcpy(data + sizeof(band), &codeTable, sizeof(codeTable));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x82, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&band, readResult, sizeof(band));
	memcpy(&codeTable,readResult + sizeof(band),sizeof(codeTable));
	free(readResult);
	return true;
}

//读取全码表
bool DUTInstruction::ReadAllCodeTable(NurRFNVCal& codeTable)
{
	//data部分
	const int dataLength = sizeof(codeTable);
	unsigned char data[dataLength];
	memcpy(data, &codeTable, dataLength);

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x83, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&codeTable, readResult, dataLength);
	free(readResult);
	return true;
}

// 读取标志位
bool DUTInstruction::ReadTagBit(unsigned char& flagID, unsigned char& flag)
{
	//band部分和data部分
	const int dataLength = sizeof(flagID) + sizeof(flag);
	BYTE data[dataLength];
	memcpy(data, &flagID, sizeof(flagID));
	memcpy(data + sizeof(flagID), &flag, sizeof(flag));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x84, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&flagID, readResult, sizeof(flagID));
	memcpy(&flag, readResult + sizeof(flagID), sizeof(flag));
	free(readResult);
	return true;

}

//打开发射机
bool DUTInstruction::OpenTransmitter(unsigned short freq, unsigned char pa, unsigned short vga)
{
	//data部分
	const int dataLength = sizeof(freq) + sizeof(pa) + sizeof(vga);
	unsigned char data[dataLength];
	memcpy(data, &freq, sizeof(freq));
	memcpy(data + sizeof(freq), &pa, sizeof(pa));
	memcpy(data + sizeof(freq) + sizeof(pa), &vga, sizeof(vga));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x10, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;

}

//关闭发射机
bool DUTInstruction::CloseTransmitter()
{
	//data部分
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x11, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;

}

//设置发射机频点
bool DUTInstruction::SetTransmitterFrequency(unsigned short freq)
{
	//data部分
	const int dataLength = sizeof(freq);
	unsigned char data[dataLength];
	memcpy(data, &freq, sizeof(freq));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x12, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;

}

//设置发射机功率
bool DUTInstruction::SetTransmitterPower(unsigned char pa, unsigned short vga)
{
	//data部分
	const int dataLength =sizeof(pa) + sizeof(vga);
	unsigned char data[dataLength];
	memcpy(data , &pa, sizeof(pa));
	memcpy(data + sizeof(pa), &vga, sizeof(vga));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x13, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//设置发射机频偏控制字
bool DUTInstruction::SetTransmitterFrequencyOffset(unsigned short afc)
{
	//data部分
	const int dataLength = sizeof(afc);
	unsigned char data[dataLength];
	memcpy(data, &afc, sizeof(afc));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x14, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//打开接收机
bool DUTInstruction::OpenReceiver(unsigned short freq, unsigned char gain)
{
	//data部分
	const int dataLength = sizeof(freq) + sizeof(gain);
	unsigned char data[dataLength];
	memcpy(data, &freq, sizeof(freq));
	memcpy(data + sizeof(freq), &gain, sizeof(gain));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x20, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//关闭接收机
bool DUTInstruction::CloseReceiver()
{
	//data部分
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x21, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//设置接收机频点
bool DUTInstruction::SetReceiverFrequency(unsigned short freq)
{
	//data部分
	const int dataLength = sizeof(freq);
	unsigned char data[dataLength];
	memcpy(data, &freq, sizeof(freq));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x22, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//设置接收机增益状态
bool DUTInstruction::SetReceiverGain(unsigned short gain)
{
	//data部分
	const int dataLength = sizeof(gain);
	unsigned char data[dataLength];
	memcpy(data, &gain, sizeof(gain));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x23, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//获取接收机信号强度
bool DUTInstruction::GetReceiverSignalStrength(unsigned short& rxpower)
{
	//data部分
	const int dataLength = sizeof(rxpower);
	unsigned char data[dataLength];
	memcpy(data, &rxpower, sizeof(rxpower));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xA4, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&rxpower, readResult, dataLength);
	free(readResult);
	return true;
}

//进入NST模式
bool DUTInstruction::EnterNSTMode()
{
	//data部分
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x40, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//退出NST模式
bool DUTInstruction::ExitNSTMode()
{
	//data部分
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x41, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST下行同步
bool DUTInstruction::NSTDownlinkSync(unsigned int channel, unsigned char mcs, unsigned char toneNum, unsigned char toneStart, unsigned char subCarrierSpacing, double power)
{
	//data部分
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
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST设置上行功率
bool DUTInstruction::NSTSetUplinkPower(double power)
{
	//data部分
	const int dataLength = sizeof(power);
	unsigned char data[dataLength];
	memcpy(data, &power, sizeof(power));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x43, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST通知UE准备切换频点
bool DUTInstruction::NSTNoticeUEPrepareSwitchFrequency()
{
	//data部分
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x44, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST切换信道
bool DUTInstruction::NSTSwitchChannel(unsigned char band, unsigned int channel)
{
	//data部分
	const int dataLength = sizeof(band) + sizeof(channel);
	unsigned char data[dataLength];
	memcpy(data, &band, sizeof(band));
	memcpy(data + sizeof(band), &channel, sizeof(channel));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x45, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//NST设置BLER测试参数
bool DUTInstruction::NSTSetBLERArg(unsigned short subFrameNumber, unsigned short time)
{
	//data部分
	const int dataLength = sizeof(subFrameNumber) + sizeof(time);
	unsigned char data[dataLength];
	memcpy(data, &subFrameNumber, sizeof(subFrameNumber));
	memcpy(data + sizeof(subFrameNumber), &time, sizeof(time));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x46, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//查询UE和仪表的连接状态
bool DUTInstruction::InquireConnectStateUEInstr(unsigned char& status)
{
	//data部分
	const int dataLength = sizeof(status);
	unsigned char data[dataLength];
	memcpy(data, &status, sizeof(status));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xC2, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&status, readResult, dataLength);
	free(readResult);
	return true;
}

//NST获取BLER
bool DUTInstruction::GetBLERValue(unsigned short& totalBlock, unsigned short& errorBlock)
{
	//data部分
	const int dataLength = sizeof(totalBlock) + sizeof(errorBlock);
	unsigned char data[dataLength];
	memcpy(data, &totalBlock, sizeof(totalBlock));
	memcpy(data + sizeof(totalBlock), &errorBlock, sizeof(errorBlock));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xC6, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&totalBlock, readResult, sizeof(totalBlock));
	memcpy(&errorBlock, readResult + sizeof(totalBlock), sizeof(errorBlock));
	free(readResult);
	return true;
}

//NST获取RSRP
bool DUTInstruction::GetRSRPValue(unsigned short& rsrp)
{
	//data部分
	const int dataLength = sizeof(rsrp);
	unsigned char data[dataLength];
	memcpy(data, &rsrp, sizeof(rsrp));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xC7, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&rsrp, readResult, sizeof(rsrp));
	free(readResult);
	return true;
}

//打开GNSS
bool DUTInstruction::OpenGNSS(unsigned char tech, char band, unsigned short bandwidth, unsigned short time)
{
	//data部分
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
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//关闭GNSS
bool DUTInstruction::CloseGNSS()
{
	//data部分
	const int dataLength = 0;
	unsigned char* data = nullptr;

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0x61, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	free(readResult);
	return true;
}

//获取SNR
bool DUTInstruction::GetSNR(unsigned short& rsrp)
{
	//data部分
	const int dataLength = sizeof(rsrp);
	unsigned char data[dataLength];
	memcpy(data, &rsrp, sizeof(rsrp));

	BYTE* readResult = (dataLength > 0) ? (BYTE*)malloc(dataLength) : nullptr;
	bool result = InterAction(readResult, data, dataLength, 0xE1, this);
	if (false == result)
	{
		//交互过程中出现错误
		free(readResult);
		return false;
	}
	memcpy(&rsrp, readResult, dataLength);
	free(readResult);
	return true;
}

//-----------------------------------------------------------------------------


//PC和串口设备的交互
bool InterAction(BYTE* readData, BYTE* data, const int dataLength, unsigned char ID, DUTInstruction* dut)
{
	//end部分
	unsigned char end[1];
	end[0] = 0x00;

	//Data区域的最大值
	const int MaxDataLen = (MESSAGEMAXCAPACITY - sizeof(NurMessageHalf) - sizeof(end));

	int forCount = 0;

	for (int i = dataLength / MaxDataLen; i >= 0;i--, forCount++)
	{
		/*先发送给UE*/
		NurMessageHalf messageHalf;
		messageHalf.Begin = '\n';
		const int MessageBodyLength = (  //消息体的长度
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
			//消息写入设备失败
			return false;
		}

		/*取设备缓冲区里面的返回消息*/
		BYTE* receiveMessage = (BYTE*)malloc(MessageBodyLength + sizeof(messageHalf.Begin) + sizeof(messageHalf.Len));
		DWORD readReturn = dut->m_serialPort.read(receiveMessage, MessageBodyLength + sizeof(messageHalf.Begin) + sizeof(messageHalf.Len));
		if (readReturn != 0)
		{
			free(message);
			free(receiveMessage);
			//消息读取失败
			return false;
		}
		NurMessageHalf SetDUTModeMessageReturn;
		memcpy(&SetDUTModeMessageReturn , receiveMessage ,sizeof(SetDUTModeMessageReturn));
		if (SetDUTModeMessageReturn.ACK != 0x00) 
		{
			free(message);
			free(receiveMessage);
			//返回消息的ack非0
			return false;
		}
		memcpy(readData + forCount * MaxDataLen, receiveMessage + sizeof(SetDUTModeMessageReturn), currentDataLen);
		free(message);
		free(receiveMessage);
	}
	return true;
}

//PC和串口设备的交互,特别版
bool InterActionEX(BYTE* readData, BYTE* data, const int dataLength, unsigned char ID, int beginIndex, int cycleLength)
{
	//end部分
	unsigned char end[1];
	end[0] = 0x00;

	//Data区域的最大值
	const int MaxDataLen = (MESSAGEMAXCAPACITY - sizeof(NurMessageHalf) - sizeof(end));

	int* divideStrategy = (int*)malloc(1*sizeof(int)); //data部分的划分策略
	int partCount = 1;  //表示上面的指针跟了多少个值了
	int limit = 0;   //芯片缓冲区的容限，也代表了因芯片而调整data划分的次数
	for (BYTE* currentDataPtr = data ; (currentDataPtr - data) < dataLength ; partCount++)
	{
		int currentDataLength = MaxDataLen;   //data受消息最大长度的限制
		bool dataRestrictOne = ((dataLength - (currentDataPtr - data)) < MaxDataLen);   //已经到data的尾部了
		bool dataRestrictTwo = (((currentDataPtr - data) + MaxDataLen) / CHIPCACHEMAXCAPACITY >= limit);    //再发一个最长的data就要超过芯片限制了
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