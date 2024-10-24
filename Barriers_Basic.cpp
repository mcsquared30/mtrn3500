#using <System.dll>

using namespace System;
using namespace System::Threading;

ref class OurThreads {
	static Barrier^ ThreadBarrier;
public:
	static bool Quit;
	OurThreads()
	{
		ThreadBarrier = gcnew Barrier(2);
		Quit = false;
	}

	static void Lidar() {
		Console::WriteLine("Entered Lidar routine");
		Thread::Sleep(5000);
		Console::WriteLine("Lidar ready");
		ThreadBarrier->SignalAndWait();

		while (!Quit) {
			//for(int i=0;i<200;i++) {
			Console::WriteLine("Laser scan acquired");
			Thread::Sleep(50); // Delay
		}
	}

	static void GNSS() {
		Console::WriteLine("Entered GNSS routine");
		Thread::Sleep(1000);
		Console::WriteLine("GNSS ready");
		ThreadBarrier->SignalAndWait();
		while (!Quit) {
			//for (int i = 0; i < 200; i++) {
			Console::WriteLine("GNSS acquisition completed");
			Thread::Sleep(50);
		}
	}
};


int main()
{
	OurThreads^ VehicleThreads = gcnew OurThreads;
	Thread^ LidarThread = gcnew Thread(gcnew ThreadStart(VehicleThreads->Lidar));
	Thread^ GNSSThread = gcnew Thread(gcnew ThreadStart(VehicleThreads->GNSS));
	LidarThread->Start();
	GNSSThread->Start();

	Console::ReadKey();
	VehicleThreads->Quit = true;
	Console::ReadKey();
	return 0;
}
