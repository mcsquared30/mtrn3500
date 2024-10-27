#pragma once
#include "framework.h"
#include "Point.h"

class Shapes
{
protected: // Means that any inherited classes can have access to this data
	Point P;
	double Orientation;
	int R;
	int G;
	int B;
public:
	Shapes() = default;
	Shapes(Point p);
	Shapes(Point p, double orientation);
	Shapes(Point p, double orientation, int r, int g, int b);
	void Move(int deltaX, int deltaY, int deltaOrientation);
	virtual void Draw(HDC h) = 0; //pure virtual function => abstract class, don't need to define these in the cpp file
	virtual ~Shapes() {};
};