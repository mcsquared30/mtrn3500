#using <System.dll>

using namespace System;

int main()
{
	int i = 5;
	double a = 0.0243253;
	// Printing to screen
	Console::WriteLine("Execution completed"); // Prints with enter, while Write() doesn't
	// Colour control
	Console::ForegroundColor = ConsoleColor::Red;
	Console::BackgroundColor = ConsoleColor::White;
	// Formatted printing|
	do
	{
		Console::SetCursorPosition(10, 10); // Makes it print everything in one line rather than printing line after line
		Console::WriteLine("{0,12:D}      {1,12:F3}", i, a); // Put index of which variable you want there, then field width, display type and number of digits to be displayed
	} while (!Console::KeyAvailable);
	Console::ForegroundColor = ConsoleColor::White;
	// Wait for a keypress
	Console::ReadKey();
	return 0;
}