#pragma once
/******************
EmbeddedFunctions.h
ALEXANDER CUNIO 2024
******************/

/*
This class should implement Galil interractions using TCP communications. It can be used to communicate using any standard command.
Implement the EmbeddedFunctions class within EmbeddedFunctions.cpp. Test this class using the provided main function in EFMain.cpp.
*/

#using <System.dll>
using namespace System;
using namespace System::Net::Sockets;
using namespace System::Text; // Convert bytes to text and text to bytes
#include <conio.h> 
#include <cstdint>

ref class EmbeddedFunctions {
public:
	// TODO: complete this function.
	EmbeddedFunctions();
	// TODO: complete this function.
	~EmbeddedFunctions();

	/**
	* Open a connection to a Galil Controller.
	*
	* @param address Null-terminated address string. Use direct connection (-d) to connect to hardware or simulator (e.g., "192.168.0.120 -d").
	* @param port Integer value for the port to connect to.
	*
	* @throws error if one occurs.
	*/
	// TODO: complete this function.
	void GOpen(String^ address, const int port);

	/**
	* Closes a connection to a Galil Controller.
	 `GClose()` should be called whenever a program is finished with a controller. This includes when a program closes. A rule of thumb is that for every `GOpen()` call on a given connection, a `GClose()` call should be found on every code path. Failing to call GClose() may cause controller resources to not be released or can hang the process if there are outstanding asynchronous operations. The latter can occur, for example, if a call to GRead() times out and the process exits without calling GClose(). In this case, GRead() still has an outstanding asynchronous read pending. GClose() will terminate this operation allowing the process to exit correctly.
	*
	*
	* @throws error if one occurs.
	*/
	// TODO: complete this function.
	void GClose();

	/**
	* Performs a *command-and-response* transaction on the connection.
	* IMPORTANT: Commands being sent to the galil should be in the form of strings based on the command reference (uploaded to Moodle). You should choose commands appropriate for a given task and avoid those that rely on logical expressions. Commands chosen should send data directly to the output or read directly from the inputs.
	* IMPORTANT: If the command string supplied is not terminated by a semicolon, you should append one.
	*
	* @param command Null-terminated command string to send to the controller.
	*
	* @return The reponse from the Galil.
	* @throws error if one occurs.
	*/
	// TODO: complete this function.
	String^ GCommand(String^ command);

private:
	// Add any additional member variables and functions as required

	//TCP client object
	TcpClient^ GALIL;
	NetworkStream^ GALILStream;
	// IP address
	String^ IPAddress;
	// Port number
	int Port;
	// Send data buffer size
	array<uint8_t>^ SendData;
	// Receive data buffer size
	array<uint8_t>^ ReceiveData;
	// Buffer size for sending data
	int SendBufferSize;
	// Buffer size for receiving data
	int RecvBufferSize;
	// Timeout values for sending and receiving
	int SendTimeout;
	int ReceiveTimeout;
	// Boolean for sending small packets immediately without buffering
	bool NoDelay;
};
