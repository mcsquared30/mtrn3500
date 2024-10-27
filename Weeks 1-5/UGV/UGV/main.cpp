#include "TMT.h"


int main() {
	ThreadManagement^ myTMT = gcnew ThreadManagement();
	myTMT->setupSharedMemory();
	myTMT->threadFunction();

	Console::ReadKey();
	Console::ReadKey();
	return 0;
}