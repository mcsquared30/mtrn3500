#pragma once
#using <System.dll>
#include "SMObjects.h"

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

ref class GNSS {
public:
	GNSS(SM_ThreadManagement^ SM_TM, SM_GNSS^ SM_GNSS);
	// thread function to run and look after the GNSS and GNSS threads
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
	~GNSS() {};

private:
	SM_ThreadManagement^ SM_TM_;
	SM_GNSS^ SM_GNSS_;
	Stopwatch^ Watch;
};