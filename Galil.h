/*****************
Galil.h
ALEXANDER CUNIO 2024
*****************/

/*
This object class outlines all the required methods for mtrn3500 Assignment 1
The functions declared in this file should be completed by the student in a file of their own making called "Galil.cpp"
DO NOT call Galil functions directly. They must all be called through EmbeddedFunctions, for example
EmbeddedFunctions *F, F->GCcommand.
Read the comments for each funciton for more information on the required implementation.
You are allowed to add extra members, data structures, and functions, but you must not modify the existing members.
Galil commands are sent as ASCII. All commands will be sent to the board as a string of characters, and must be
terminated by a semicolon ';'.
*/

#pragma once
#include "EmbeddedFunctions.h"
#include <iostream>
#include <stdint.h>

#include <string>
#include <bitset>

class Galil {
public:
	// TODO: complete this function.
	Galil();												// Default constructor. Initialize variables, open Galil connection and allocate memory.
															// Should assign a default embedded functions that works with physical hardware and a 
															// default Galil address as described in the assignment spec.
	// TODO: complete this function.
	Galil(EmbeddedFunctions* Funcs, GCStringIn address);	// Constructor with EmbeddedFunciton pre-initialised and passed in.
	// TODO: complete this function.
	Galil(const Galil& other);								// Copy constructor to copy the state of all elements within the object other.
															// It should construct a new EmbeddedFunctions object and open a separate connection
															// (i.e., each class will have a unique value of the GCon g). All other data members
															// should be transferred.
	Galil(Galil&& other) = delete;							// Move constructor deleted and should not be implemented.
	// TODO: complete this function.
	~Galil();												// Default destructor. Deallocate memory and close Galil connection.

	// DIGITAL OUTPUTS
	// TODO: complete this function.
	void DigitalOutput(uint16_t value);						// Write to all 16 bits of digital output, 1 command to the Galil
	// TODO: complete this function.
	void DigitalByteOutput(bool bank, uint8_t value);		// Write to one byte, either high or low byte, as specified by user in 'bank'
															// 0 = low, 1 = high
	// TODO: complete this function.
	void DigitalBitOutput(bool val, uint8_t bit);			// Write single bit to digital outputs. 'bit' specifies which bit


	// DIGITAL INPUTS
	// TODO: complete this function.
	uint16_t DigitalInput();								// Return the 16 bits of input data
															// Query the digital inputs of the GALIL, See Galil command library @IN
	// TODO: complete this function.
	uint8_t DigitalByteInput(bool bank);					// Read either high or low byte, as specified by user in 'bank'
															// 0 = low, 1 = high
															// A bank is one byte (8 bits). The low bank (0) is the first 8
															// bits (DI0-DI7) and the high bank (1) is the upper 8 bits
															// (DI8-DI15).
	// TODO: complete this function.
	bool DigitalBitInput(uint8_t bit);						// Read single bit from current digital inputs. Above functions
															// may use this function

	// TODO: complete this function.
	bool CheckSuccessfulWrite();							// Check the string response from the Galil to check that the last
															// command executed correctly. 1 = succesful.
															// A successful write indicates that a write command (sending a 
															// message to the Galil that does not have a response -- e.g., 
															// digitalOutput, analogOutput) has completed without errors.
															// This should validate some part of the Galil's response (it is 
															// up to you how this is completed) but should validly
															// differentiate when a write command has been completed 
															// successfully.
															// This will be called from your main function (do not call it
															// within your implementation functions of this Galil class.

	// ANALOG FUNCITONS
	// TODO: complete this function.
	float AnalogInput(uint8_t channel);						// Read Analog channel and return voltage			
	// TODO: complete this function.
	void AnalogOutput(uint8_t channel, double voltage);		// Write to any channel of the Galil, send voltages as
															// 2 decimal place in the command string
	// TODO: complete this function.
	void AnalogInputRange(uint8_t channel, uint8_t range);	// Configure the range of the input channel with
															// the desired range code

	// ENCODER
	// TODO: complete this function.
	void WriteEncoder();									// Manually Set the motor encoder value to zero (encoder channel 0)
	// TODO: complete this function.
	int ReadEncoder();										// Read from motor Encoder (encoder channel 0)

	// CONTROL FUNCTIONS
	// TODO: complete this function.
	void setSetPoint(int s);								// Set the desired setpoint for control loops, counts or counts/sec
															// This should set it within the class not on the actual Galil.
	// TODO: complete this function.
	double getSetPoint();									// Gets the current setpoint stored in the class
	// TODO: complete this function.
	void setKp(double gain);								// Set the proportional gain of the controller used in controlLoop() of Position/SpeedControl
															// This should set it within the class not on the actual Galil.
	// TODO: complete this function.
	double getKp();											// Gets the current proportional gain stored in the class
	// TODO: complete this function.
	void setKi(double gain);								// Set the integral gain of the controller used in controlLoop()  of Position/SpeedControl
															// This should set it within the class not on the actual Galil.
	// TODO: complete this function.
	double getKi();											// Gets the current integral gain stored in the class
	// TODO: complete this function.
	void setKd(double gain);								// Set the derivative gain of the controller used in controlLoop()  of Position/SpeedControl
															// This should set it within the class not on the actual Galil.
	// TODO: complete this function.
	double getKd();											// Gets the current derivative gain stored in the class
	
	void PositionControl(bool debug, int Motorchannel);		// Run the control loop. ReadEncoder() is the input to the loop. The motor is the output.
															// The loop will run using the PID values specified in the data of this object, and has an 
															// automatic timeout of 10s. You do NOT need to implement this function, it is defined in
															// GalilControl.lib
	void SpeedControl(bool debug, int Motorchannel);		// same as above. Setpoint interpreted as counts per second


	// OPERATOR OVERLOADS
	// TODO: complete this function.
	friend std::ostream& operator<<(std::ostream& output, Galil& galil);	// Operator overload for '<<' operator. So the user can say cout << Galil;
																			// This function should print out the output of GInfo and GVersion, with
																			// two newLines after each.
	Galil& operator=(const Galil& other);									// Copy assignment operator. This acts in the same way as the copy constructor
																			// (refer above for details).
	
protected:
	EmbeddedFunctions* Functions;	// Pointer to EmbeddedFunctions, through which all Galil Function calls will be made
	GCon g;							// Connection handle for the Galil, passed through most Galil function calls
	double ControlParameters[3];	// Contains the controller gain values: K_p, K_i, K_d in that order 
	int setPoint;					// Control Setpoint

	// TODO: Add any new data members or functions BELOW (DO NOT ADD THEM ABOVE THIS LINE)
	GCStringIn IPAddress = "192.168.0.120 -d";
	GBufOut ReadBuffer;
	GReturn GResult;
	void SendGCommand(const std::string& command);
};
