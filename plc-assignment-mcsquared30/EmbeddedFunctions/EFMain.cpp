#include "EmbeddedFunctions.h"

int main(void) {
	//while (!_kbhit) { // Do we need this? It keeps running until key press -> should we make this loop run determined by a different factor?
		
	//}

	EmbeddedFunctions^ funcs = gcnew EmbeddedFunctions();

	funcs->GOpen("127.0.0.1", 26000);

	
	funcs->GCommand("OP 1,0");
	funcs->GCommand("OP 7,0");
	funcs->GCommand("OP 3,0");

	funcs->GCommand("AO 0,3");
	Threading::Thread::Sleep(5000);
	funcs->GCommand("AO 0,0");

	//funcs->GCommand("MG "Hello";");

	//funcs->~EmbeddedFunctions();

	Console::ReadKey();
	
	funcs->GClose();

	Console::ReadKey();

	funcs->~EmbeddedFunctions();

	Console::ReadKey();

	return 0;
}
