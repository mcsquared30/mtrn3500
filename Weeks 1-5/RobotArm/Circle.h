#pragma once
#include "Shapes.h"

class Circle : public Shapes
{
	double Radius;
public:
	Circle() = default;
	Circle(Point c, double radius, int r, int g, int b);
	void Draw(HDC h);
	virtual ~Circle() {}; // must call the destructor in Shapes 
	// too so that everything that was inherited can be destroyed, not just the radius of the circle
};