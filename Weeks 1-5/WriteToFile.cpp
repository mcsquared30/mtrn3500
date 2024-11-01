#using <System.dll>

using namespace System;
using namespace System::IO;

value struct GNSS {
	double Lattitude;
	double Longitude;
	GNSS(double lat, double lng) :Lattitude(lat), Longitude(lng) {}

};

int main()
{
	String^ Filename = "GNSS.m";
	StreamWriter^ sw = gcnew StreamWriter(Filename);
	array<GNSS>^ GNSSData = { GNSS(1,2), GNSS(3,4),GNSS(5,6),GNSS(7,8) };
	for (int i = 0; i<(GNSSData->Length); i++)
	{
		sw->WriteLine("Lattitude: {0,10:F3}\n"   "Longitude: {1,10:F3}\n", GNSSData[i].Lattitude, GNSSData[i].Longitude);
	}
	sw->Close();
	Console::ReadKey();
	return 0;
}