#pragma once
#include <iostream>

struct size {
	int x;
	int y;
	size():x(0),y(0){}
	size(int _x, int _y):x(_x),y(_y){}
};

class Entity
{
	virtual void dummy() = 0;
public:
	Entity();
	Entity(int _sx, int _sy);
	~Entity();

	float x = 0;
	float y = 0;
	size size;
	int direction = 1;
	int animation_frame = 1;

	//zwraca prawdę jeżeli jest kolizja
	bool collision(Entity* colider);
	bool next_animation_frame();
};

