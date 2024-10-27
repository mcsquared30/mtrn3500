#include "Robot.h"
#include <vector>

Robot::Robot(Point base, std::vector<LinkProperties> data):Base(base)
{
	int n = data.size(); // Get the number of links
	Links.resize(n);
	int i = 0;
	for (std::vector<LinkProperties>::iterator it = data.begin(); it != data.end(); it++) {
		Links[i++] = new Link(Point(0, 0), it->Length, it->Angle, it->HubDiameter, it->Red, it->Green, it->Blue);
	}
}

void Robot::Draw(HDC h)
{
	Point P;
	Links[0]->SetStart(Base);
	Links[0]->Draw(h);
	for (std::vector<Link*>::iterator it = Links.begin() + 1; it != Links.end(); it++)
	{
		P = (*(it - 1))->GetEnd();
		(*it)->SetStart(P);
		(*it)->Draw(h);
	}
}

void Robot::Move(std::vector<double> deltaAngles)
{
	int i = 0;
	for (std::vector<double>::iterator it = deltaAngles.begin(); it != deltaAngles.end(); it++)
	{
		Links[i++]->Move(0, 0, *it);
	}
}

Robot::~Robot()
{
	for (std::vector<Link*>::iterator it = Links.begin(); it != Links.end(); it++)
	{
		delete (*it);
	}
}

// Non-class function since we don't have access to the LHS
std::ostream& operator<<(std::ostream& os, const Robot& r) // Need to pass by reference, because we need it to go to the screen using cout, if we pass a copy of os it will not go to the screen
// Passing by reference means that any changes made to os in this function will be reflected everywhere, including outside the function
// The const prevents the Robot being changed inside the function
{
	Point P;
	int n = r.Links.size();
	P = r.Links[n - 1]->GetEnd(); // Access the last link
	os << P.GetX() << " " << P.GetY() << " ";
	return os;
}

// 1. Save time by not making copies
// 2. No stack operations so stack space and time is saved
// 3. The life of the reference object exists after the function call, hence destructor is not called