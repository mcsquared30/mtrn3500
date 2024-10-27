#pragma once
#using <System.dll>
#include "SMObjects.h"

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

ref struct ThreadProperties {
	ThreadStart^ ThreadStart_;
	bool Critical;
	String^ ThreadName;
	uint8_t BitID; // for in the shutdown byte

	ThreadProperties(ThreadStart^ start, bool crit, uint8_t bit_id, String^ name) {
		ThreadStart_ = start;
		Critical = crit;
		BitID = bit_id;
		ThreadName = name;
	};
};


ref class ThreadManagement
{
public:
	// set up shared memory
	error_state setupSharedMemory();
	// thread function to run and look after the GNSS and laser threads
	void threadFunction();
	// process heartbeats
	error_state processHeartbeats();
	// shutdown threads
	void shutdownThreads();
	// get shutdownflag
	bool getShutdownFlag();
private:
	SM_ThreadManagement^ SM_TM_;
	SM_GNSS^ SM_GNSS_;
	SM_Laser^ SM_Laser_;
	array<Stopwatch^>^ StopwatchList;
	array<Thread^>^ ThreadList;
	array<ThreadProperties^>^ ThreadPropertiesList;
};