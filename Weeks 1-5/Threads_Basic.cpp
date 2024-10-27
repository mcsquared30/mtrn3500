#using <System.dll>

using namespace System;
using namespace System::Threading;

bool Quit = false;

void Lidar();
void GNSS();

int main()
{
	Thread^ GNSSThread = gcnew Thread(gcnew ThreadStart(GNSS));
	Thread^ LidarThread = gcnew Thread(gcnew ThreadStart(Lidar));
	GNSSThread->Start();
	LidarThread->Start();
	Console::ReadKey();
	Quit = true;
	Console::ReadKey();
	return 0;
}

void Lidar() {
	while (!Quit) {
	//for(int i=0;i<200;i++) {
		Console::WriteLine("Laser scan acquired");
		Thread::Sleep(50); // Delay
	}

}

void GNSS() {
	while (!Quit) {
	//for (int i = 0; i < 200; i++) {
		Console::WriteLine("GNSS acquisition completed");
		Thread::Sleep(50);
	}
}