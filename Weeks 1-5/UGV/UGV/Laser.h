#pragma once
#using <System.dll>
#include "SMObjects.h"

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

ref class Laser {
public:
	Laser(SM_ThreadManagement^ SM_TM, SM_Laser^ SM_Laser);
	// thread function to run and look after the GNSS and laser threads
	void threadFunction();
	// process heartbeats
	error_state processHeartbeats();
	// shutdown threads
	void shutdownThreads();
	// get shutdownflag
	bool getShutdownFlag();
	error_state communicate();
	error_state checkData();
	error_state setupSharedMemory();
	error_state processSharedMemory();
	~Laser() {};

private:
	SM_ThreadManagement^ SM_TM_;
	SM_Laser^ SM_Laser_;
	Stopwatch^ Watch;
};