#using <System.dll>

using namespace System;
using namespace System::Timers;

void OnElapsed(System::Object^ sender, System::Timers::ElapsedEventArgs^ e);
int main()
{
	Timer^ MyTimer = gcnew Timer(500);
	MyTimer->AutoReset = true; // Will reset after 500ms back to 0
	MyTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(&OnElapsed); // Elapsed is an event,
	// += is adding an object to the event, which is passed the function OnElapsed, 
	// that is the actual event handler
	MyTimer->Start();
	Console::ReadKey();
	MyTimer->Stop();
	Console::CursorVisible = true;
	return 0;
}

void OnElapsed(System::Object^ sender, System::Timers::ElapsedEventArgs^ e)
{
	Console::CursorVisible = false;
	Console::SetCursorPosition(20, 2);
	// print the time on the screen every time the timer has elapsed
	Console::WriteLine(DateTime::Now);
}
