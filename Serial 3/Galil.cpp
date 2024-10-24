#include "Galil.h"

using namespace System::Text;

Galil::Galil(String^ portname, int baudrate, int databits, IO::Ports::StopBits stopbits,
	IO::Ports::Parity parity, IO::Ports::Handshake handshake)
{
	Port = gcnew SerialPort();
	Port->PortName = portname;
	Port->BaudRate = baudrate;
	Port->DataBits = databits;
	Port->StopBits = stopbits;
	Port->Parity = parity;
	Port->Handshake = handshake;

	Port->ReadTimeout = 500;
	Port->WriteTimeout = 500;
	Port->ReadBufferSize = 1024;
	Port->WriteBufferSize = 1024;

	SendData = gcnew array<unsigned char>(64);
	RecvData = gcnew array<unsigned char>(64);

	Port->Open();
}

ErrorCode Galil::DigitalByteOutput(int bank, Byte data)
{
	String^ CommandHeader;
	String^ Command;

	if (bank < 0 || bank > 1)
	{
		return BANK_OUT_OF_RANGE;
	}
	if (bank == 0)
	{
		CommandHeader = gcnew String("OP ");
	}
	else
	{
		CommandHeader = gcnew String("OP ,");
	}

	Command = CommandHeader + data.ToString("D3") + ";";
	Port->Write(Command);
	System::Threading::Thread::Sleep(10);
	Port->Read(RecvData, 0, RecvData->Length);
	String^ Response = Encoding::ASCII->GetString(RecvData);
	if (Response->Contains(":"))
		return COMMS_OK;
	else
		return NO_RESPONSE;
}
ErrorCode Galil::AnalogOutput(int channel, double voltage)
{
	String^ CommandHeader;
	String^ Command;

	if (channel < 0 || channel > 7)
	{
		return CHANNEL_OUT_OF_RANGE;
	}

	CommandHeader = gcnew String("AO");


	Command = CommandHeader + channel.ToString("D1") + "," + voltage.ToString("F3") + ";";

	Port->Write(Command);
	System::Threading::Thread::Sleep(10);
	Port->Read(RecvData, 0, RecvData->Length);
	String^ Response = Encoding::ASCII->GetString(RecvData);
	if (Response->Contains(":"))
		return COMMS_OK;
	else
		return NO_RESPONSE;
}
Galil::~Galil()
{
	Port->Close();
}