#using <System.dll>
#include "GNSS.h"


using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

GNSS::GNSS(SM_ThreadManagement^ SM_TM, SM_GNSS^ SM_GNSS)
{
	SM_TM_ = SM_TM;
	SM_GNSS_ = SM_GNSS;
	Watch = gcnew Stopwatch;
}

error_state GNSS::setupSharedMemory() {
	return SUCCESS;
}

// thread function to run and look after the GNSS and GNSS threads
void GNSS::threadFunction() {
	Console::WriteLine("GNSS thread is starting");
	// setup stopwatch
	Watch = gcnew Stopwatch;
	// wait at the barrier
	SM_TM_->ThreadBarrier->SignalAndWait();
	// start the stopwatch
	Watch->Start();
	// start the thread loop
	while (!Console::KeyAvailable && !getShutdownFlag()) {
		Console::WriteLine("GNSS thread is running");
		processHeartbeats();
		// and in real life you need to connect to the GNSS, get the data from the GNSS, 
		// interpret, calculate and put into shared memory
		// The following is a dummy statement representing this:
		if (communicate() == SUCCESS && checkData() == SUCCESS) {
			processSharedMemory(); // putting the data into the GNSS shared memory
		}
		Thread::Sleep(20);
	}
	Console::WriteLine("GNSS thread is terminating");
}
// process heartbeats
error_state GNSS::processHeartbeats() {
	// Is the GNSS bit in the heartbeat byte down? - ie. has the Thread Management Thread put it down and is operating correctly?
	if ((SM_TM_->heartbeats && bit_GPS) == 0) {
		// put the bit up
		SM_TM_->heartbeats |= bit_GPS;
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
void GNSS::shutdownThreads() {
	SM_TM_->shutdown = 0xFF; // Set all shutdown bits to 1 so all the bits shut down
}
// get shutdownflag
bool GNSS::getShutdownFlag() {
	return (SM_TM_->shutdown & bit_GPS); // will tell us if the bit corresponding to thread management is high or low
	// in the shutdown byte
}

error_state GNSS::communicate() {
	return SUCCESS;
}
error_state GNSS::checkData() {
	return SUCCESS;
}

error_state GNSS::processSharedMemory() {
	return SUCCESS;
}