#pragma once
#using <System.dll>

#include "Laser.h"
#include "GNSS.h"
#include "TMT.h"


// set up shared memory
error_state ThreadManagement::setupSharedMemory() 
{
	SM_GNSS_ = gcnew SM_GNSS;
	SM_TM_ = gcnew SM_ThreadManagement;
	SM_Laser_ = gcnew SM_Laser;
	return SUCCESS;
}
// thread function to run and look after the GNSS and laser threads
void ThreadManagement::threadFunction() {
	// make a list of thread properties
	ThreadPropertiesList = gcnew array<ThreadProperties^>{
	gcnew ThreadProperties(gcnew ThreadStart(gcnew Laser(SM_TM_, SM_Laser_), &Laser::threadFunction), true, bit_LASER, "Laser Thread"),
		gcnew ThreadProperties(gcnew ThreadStart(gcnew GNSS(SM_TM_, SM_GNSS_), &GNSS::threadFunction), true, bit_GPS, "GNSS Thread")};
	// make a list of threads
	ThreadList = gcnew array<Thread^>(ThreadPropertiesList->Length); // Allows new threads to be added and the length will be updated
	// make the stopwatch list
	StopwatchList = gcnew array<Stopwatch^>(ThreadPropertiesList->Length);
	// make a thread barrier which the GNSS, GPS and thread managment can all access -> we will do
	// this in SMObjects so they all have access
	SM_TM_->ThreadBarrier = gcnew Barrier(ThreadPropertiesList->Length + 1); // +1 since needs to take into account this thread

	// initialise and start all the threads, and initialise stopwatches (which are watching the 2 other threads)
	for (int i = 0; i < ThreadPropertiesList->Length; i++) {
		StopwatchList[i] = gcnew Stopwatch;
		ThreadList[i] = gcnew Thread(ThreadPropertiesList[i]->ThreadStart_);
		ThreadList[i]->Start();
	}
	// wait at the TMT thread barrier
	SM_TM_->ThreadBarrier->SignalAndWait();
	// start all the stopwatches
	for (int i = 0; i < ThreadPropertiesList->Length; i++) {
		StopwatchList[i]->Start();
	}
	// start the thread loop - shutdown flags should be down
	while (!Console::KeyAvailable && !getShutdownFlag()) // While there is no keypress and the shutdown flag is not up
	{
		// keep checking the heartbeats
		Console::WriteLine("TMT thread is running");
		processHeartbeats();
		Thread::Sleep(50);
	}
	// end of thread loop
	// shutdown threads
	shutdownThreads();
	// join all threads - since the TMT thread can only finish when the GNSS and laser thread have completely finished
	for (int i = 0; i < ThreadPropertiesList->Length; i++) {
		ThreadList[i]->Join();
	}
	Console::WriteLine("TMT thread is terminating...");
}
// process heartbeats
error_state ThreadManagement::processHeartbeats() {
	for (int i = 0; i < ThreadPropertiesList->Length; i++)
	{
		// check the heartbeat flag of ith thread (if it is high?)
		if (SM_TM_->heartbeats && ThreadPropertiesList[i]->BitID)
		{
			// put ith bit down
			SM_TM_->heartbeats ^= ThreadPropertiesList[i]->BitID; // XOR-ing will only put down the bit at the bitID if it is 1
			// reset the stopwatch
			StopwatchList[i]->Restart();
		}
		// else
		else {
			// Check the stopwatch - has the time exceeded the crashtime limit?
			if (StopwatchList[i]->ElapsedMilliseconds > CRASH_LIMIT) {
				// is ith process critical process?
				if (ThreadPropertiesList[i]->Critical) {
					// shutdown all
					Console::WriteLine(ThreadPropertiesList[i]->ThreadName + " failure. Shutting down all threads.");
					shutdownThreads();
					return ERR_CRITICAL_PROCESS_FAILURE;
				}
				else {
					// try to restart
					Console::WriteLine(ThreadPropertiesList[i]->ThreadName + " failed. Attempting to restart.");
					ThreadList[i]->Abort();
					ThreadList[i] = gcnew Thread(ThreadPropertiesList[i]->ThreadStart_);
					SM_TM_->ThreadBarrier = gcnew Barrier(1); // Tells the threadBarrier it only has to wait for this one
					// thread since the other threads have already started and gone through the barrier
					ThreadList[i]->Start();
				}
			}

		}
	}
	return SUCCESS;
}

// shutdown threads
void ThreadManagement::shutdownThreads() {
	SM_TM_->shutdown = 0xFF; // Set all shutdown bits to 1 so all the bits shut down
}
// get shutdownflag
bool ThreadManagement::getShutdownFlag() {
	return (SM_TM_->shutdown & bit_TM); // will tell us if the bit corresponding to thread management is high or low
	// in the shutdown byte
}