#include "Galil.h"

using namespace System;

int main()
{
	Galil^ RIO47142 = gcnew Galil("COM3", 115200, 8, IO::Ports::StopBits::One,
		IO::Ports::Parity::None, IO::Ports::Handshake::RequestToSend);

	Byte Data = 0x55; //0b01010101

	while (!Console::KeyAvailable)
	{
		RIO47142->DigitalByteOutput(0, Data);
		Threading::Thread::Sleep(250);
		RIO47142->DigitalByteOutput(0, (Data << 1));
		Threading::Thread::Sleep(250);
	}
	return 0; 
}