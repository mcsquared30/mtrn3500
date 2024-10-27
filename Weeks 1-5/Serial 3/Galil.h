#pragma once
#include <cstdint>
#using <System.dll>

using namespace System;
using namespace System::IO::Ports;

enum ErrorCode
{
	COMMS_OK,
	NO_RESPONSE,
	BANK_OUT_OF_RANGE,
	CHANNEL_OUT_OF_RANGE
};

ref class Galil
{
	SerialPort^ Port;
	array<uint8_t>^ SendData;
	array<uint8_t>^ RecvData;

public:
	Galil() {};
	Galil(String^ portname, int baudrate, int databits, IO::Ports::StopBits stopbits,
		IO::Ports::Parity parity, IO::Ports::Handshake handshake);
	ErrorCode DigitalByteOutput(int bank, Byte data);
	ErrorCode AnalogOutput(int channel, double voltage);
	~Galil();
};
