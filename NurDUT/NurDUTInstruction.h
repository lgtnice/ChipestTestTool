#pragma once
#include "NurSerialPort.h"
#include "NurBaseDUT.h"
#include "NurRFNVTable.h"
#include "NurCommonFunc.h"


//PC��UEͨ�ŵ���Ϣ��ǰ����
typedef struct {
	unsigned char Begin ;
	unsigned char Len[2];

	unsigned char ID;
	unsigned char ACK;
	unsigned char Index;
	//���������
	//��β��
}NurMessageHalf;

//-----------------------------------------------------------------------------

class DUTInstruction : public CNurDUTInterface
{
private:
	CSerialPortService m_serialPort;
public:
	DUTInstruction();
	~DUTInstruction(void);

	bool OpenSerialPort(DWORD BaudRate, LPCTSTR portName);
	bool CloseSerialPort();
	//ͨ��ָ��
	bool SetDUTMode(unsigned char mode); 
	bool WriteAFCCodeTable(NurRFNVCalAfc codeTable);
	bool WriteBandCodeTable(unsigned char band, NurRFNVCalInfo codeTable);
	bool WriteAllCodeTable(NurRFNVCal codeTable);
	bool WriteTagBit(unsigned char flagID, unsigned char flag);
	bool UpdateCodeTableToRAM();
	bool ReadAFCCodeTable(NurRFNVCalAfc& codeTable);
	bool ReadSpecialBandCodeTable(unsigned char& band, NurRFNVCalInfo& codeTable);
	bool ReadAllCodeTable(NurRFNVCal& codeTable);
	bool ReadTagBit(unsigned char& flagID, unsigned char& flag);
	//��ͨУ׼ָ��
	bool OpenTransmitter(unsigned short freq,unsigned char pa,unsigned short vga);
	bool CloseTransmitter();
	bool SetTransmitterFrequency(unsigned short freq );
	bool SetTransmitterPower(unsigned char pa,unsigned short vga);
	bool SetTransmitterFrequencyOffset(unsigned short afc);
	bool OpenReceiver(unsigned short freq, unsigned char gain);
	bool CloseReceiver();
	bool SetReceiverFrequency(unsigned short freq);
	bool SetReceiverGain(unsigned short gain);
	bool GetReceiverSignalStrength(unsigned short& rxpower);
	//�������۲�
	bool EnterNSTMode();
	bool ExitNSTMode();
	bool NSTDownlinkSync(unsigned int channel,unsigned char mcs, unsigned char toneNum, unsigned char toneStart, unsigned char subCarrierSpacing,double power);
	bool NSTSetUplinkPower(double power);
	bool NSTNoticeUEPrepareSwitchFrequency();
	bool NSTSwitchChannel(unsigned char band,unsigned int channel);
	bool NSTSetBLERArg(unsigned short subFrameNumber,unsigned short time);
	bool InquireConnectStateUEInstr(unsigned char& status);
	bool GetBLERValue(unsigned short& totalBlock, unsigned short& errorBlock);
	bool GetRSRPValue(unsigned short& rsrp);
	//GNSS����
	bool OpenGNSS(unsigned char tech, char band, unsigned short bandwidth, unsigned short time);
	bool CloseGNSS();
	bool GetSNR(unsigned short& rsrp);

	//��Ԫ����
	friend bool InterAction(BYTE* readData, BYTE* data, const int dataLength, unsigned char ID, DUTInstruction* dut);
	friend bool InterActionEX(BYTE* readData, BYTE* data, const int dataLength, unsigned char ID, int beginIndex, int cycleLength);
};

