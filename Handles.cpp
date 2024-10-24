#using <System.dll>

using namespace System;

ref class Data // ref makes it so that any class of type Data will be stored on the Heap
// If we make this a struct instead of a class, then a, b and c are directly accessible without the getter functions
{
	double a;
	double b;
	double c;
public:
	Data(double a, double b, double c) : a(a), b(b), c(c) {}
	double GetA() { return a; }
	double GetB() { return b; }
	double GetC() { return c; }
};


int main()
{
	//int n = 50; // allocated on the stack
	//double a = 3.14159; // allocated on the stack
	//double b[100000]; // space allocated on the stack
	// Memory gets cleared in all the following cases
	//Line 26: creating an objecting using stack semantics
	Data MyData(2, 3, 4); // Space automatically allocated on the heap -- if we use 'value' instead of 'ref' above, then this is allocated on the stack instead
	// Exceptions to stack semantics => Strings and arrays
	
	// Extract the handle that is pointing to MyData on the heap:
	Data^ MyDataHndl = % MyData; // Stack-allocated (but heap-managed) Data object (handle is on the stack, data value is on the managed heap),
	// passing the MyData object by reference, which will be pointed to by the handle MyDataHndl
	Data^ DataHndl = gcnew Data(2, 3, 4); // New data handle which will point to the Data object on the managed heap
	int^ a = gcnew int(50); // A handle, similar to a pointer, allocated space on the managed HEAP
	//Printing to the screen
	Console::WriteLine("{0,12:F3}", DataHndl->GetA());
	Console::WriteLine("{0,12:F3}", MyData.GetA());
	Console::WriteLine("{0,12:F3}", MyDataHndl->GetA());
	Console::WriteLine("{0,12:d}", a);

	//Wait for keypress;
	Console::ReadKey();
	Console::ReadKey();
	return 0;
}
