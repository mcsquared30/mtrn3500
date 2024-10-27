#pragma once
#include "Point.h"
#include <vector>
#include "Link.h"
#include <iostream>
#include <fstream>

struct LinkProperties {
	double Length;
	double Angle;
	double HubDiameter;
	int Red;
	int Green;
	int Blue;

};

class Robot 
{
	Point Base;
	std::vector<Link*> Links;
public:
	Robot() = default;
	Robot(Point base, std::vector<LinkProperties> data);
	void Draw(HDC h);
	void Move(std::vector<double> angles);
	~Robot();
	friend std::ostream& operator<<(std::ostream& os, const Robot& r); // need this so that we can access the private data member Links in this non-member function
};

