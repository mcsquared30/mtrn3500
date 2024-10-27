#pragma once
#using <System.dll>
#include <iostream>
#include <bitset>
#include <cstdint>

using namespace System;
using namespace System::Diagnostics;
using namespace System::Threading;

int& Add(int a, int& b, int c) {
	a = b++ + c++;
	return a;
}

int main()
{
	int a = 1;
	int b = 2;
	int c = 3;
	Add(a, b, Add(a, b, Add(a, b, c)));

	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << c << std::endl;

	Console::ReadKey();
	return 0;
}