#using <System.dll>
#include "Laser.h"


using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

Laser::Laser(SM_ThreadManagement^ SM_TM, SM_Laser^ SM_Laser)
{
	SM_TM_ = SM_TM;
	SM_Laser_ = SM_Laser;
	Watch = gcnew Stopwatch;
}

error_state Laser::setupSharedMemory() {
	return SUCCESS;
}

// thread function to run and look after the GNSS and laser threads
void Laser::threadFunction() {
	Console::WriteLine("Laser thread is starting");
	// setup stopwatch
	Watch = gcnew Stopwatch;
	// wait at the barrier
	SM_TM_->ThreadBarrier->SignalAndWait();
	// start the stopwatch
	Watch->Start();
	// start the thread loop
	while (!Console::KeyAvailable && !getShutdownFlag()) {
		Console::WriteLine("Laser thread is running");
		processHeartbeats();
		// and in real life you need to connect to the laser, get the data from the laser, 
		// interpret, calculate and put into shared memory
		// The following is a dummy statement representing this:
		if (communicate() == SUCCESS && checkData() == SUCCESS) {
			processSharedMemory(); // putting the data into the laser shared memory
		}
		Thread::Sleep(20);
	}
	Console::WriteLine("Laser thread is terminating");
}
// process heartbeats
error_state Laser::processHeartbeats() {
	// Is the laser bit in the heartbeat byte down? - ie. has the Thread Management Thread put it down and is operating correctly?
	if ((SM_TM_->heartbeats && bit_LASER) == 0) {
		// put the bit up
		SM_TM_->heartbeats |= bit_LASER;
		// reset the stopwatch
		Watch->Restart();
	}
	else {
		// has the time exceeded the crash time limit?
		if (Watch->ElapsedMilliseconds > CRASH_LIMIT) {
			// shutdown all
			shutdownThreads();
			return ERR_TMT_FAILURE;
		}
	}

	return SUCCESS;
}

// shutdown threads
void Laser::shutdownThreads() {
	SM_TM_->shutdown = 0xFF; // Set all shutdown bits to 1 so all the bits shut down
}
// get shutdownflag
bool Laser::getShutdownFlag() {
	return (SM_TM_->shutdown & bit_LASER); // will tell us if the bit corresponding to thread management is high or low
	// in the shutdown byte
}

error_state Laser::communicate() {
	return SUCCESS;
}
error_state Laser::checkData() {
	return SUCCESS;
}

error_state Laser::processSharedMemory() {
	return SUCCESS;
}