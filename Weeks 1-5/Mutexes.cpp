#using <System.dll>

using namespace System;
using namespace System::Threading;

ref class Mutexes
{
public:
	static Mutex^ ourMutex; 
	//static Object^ lockObject;
	static Barrier^ threadBarrier; // both classes defined under Threading 
	int numThreads; // not static because it will not be used inside the thread
	static bool Quit; // All of these need to be declared as static to prevent the Mutexes class being instantiated many 
	// times and creating many different data of this type, which would be a huge entanglement of these threads
	// All of this data has to be common
	Mutexes() {
		ourMutex = gcnew Mutex;
		//lockObject = gcnew Object;
		numThreads = 3;
		threadBarrier = gcnew Barrier(numThreads);
	}
	static void Lidar() {
		Console::WriteLine("Lidar thread started");
		Thread::Sleep(5000);
		Console::WriteLine("Lidar thread ready");
		threadBarrier->SignalAndWait();
		while(!Quit) {
			Console::WriteLine("Lidar scan completed");
			CriticalSection();
			Thread::Sleep(50);
		}
	}
	static void GNSS() {
		Console::WriteLine("GNSS thread started");
		Thread::Sleep(10000);
		Console::WriteLine("GNSS thread ready");
		threadBarrier->SignalAndWait();
		while (!Quit) {
			Console::WriteLine("GNSS scan completed");
			CriticalSection();
			Thread::Sleep(50);
		}
	}
	static void Vehicle() {
		Console::WriteLine("Vehicle thread started");
		Thread::Sleep(10000);
		Console::WriteLine("Vehicle thread ready");
		threadBarrier->SignalAndWait();
		while (!Quit) {
			Console::WriteLine("Vehicle scan completed");
			CriticalSection();
			Thread::Sleep(50);
		}
	}

	static void CriticalSection() {
		Console::WriteLine(Thread::CurrentThread->Name + "is requesting the Mutex");
		ourMutex->WaitOne();
		//Monitor::Enter(lockObject);
		Console::WriteLine(Thread::CurrentThread->Name + "is in CS");
		Thread::Sleep(300);
		//Monitor::Exit(lockObject);
		ourMutex->ReleaseMutex();
		Console::WriteLine(Thread::CurrentThread->Name + "has released the Mutex\n");

	}
};


int main() {
	Mutexes^ example = gcnew Mutexes;

	Thread^ LidarThread = gcnew Thread(gcnew ThreadStart(example->Lidar));
	LidarThread->Name = "Lidar thread";

	Thread^ GNSSThread = gcnew Thread(gcnew ThreadStart(example->GNSS));
	GNSSThread->Name = "GNSS thread";

	Thread^ VehicleThread = gcnew Thread(gcnew ThreadStart(example->Vehicle));
	VehicleThread->Name = "Vehicle thread";

	LidarThread->Start();
	GNSSThread->Start();
	VehicleThread->Start();

	Console::ReadKey();
	example->Quit = true;

	Console::ReadKey();
	return 0;
}