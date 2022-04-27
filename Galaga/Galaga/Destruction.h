#pragma once
#include <SFML/Graphics.hpp>
#define FRAME_TIME 0.05f
class Destruction
{
public:
	double x;
	double y;
	int size_x;
	int size_y;
	int frame_s=4;
	int frame=1;

	Destruction(double _x,double _y,int _sx, int _sy);
	void animation();
};

