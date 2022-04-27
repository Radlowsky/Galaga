#pragma once
#include "Entity.h"
#include <vector>
class Shield : public Entity
{
	void dummy() {};
public:
	std::vector<bool> inside_boxes =
	{
		true,true,true,
		true,true,true,
		true,false,true
	};

	//check collision inside a sheald
	bool inside_collision(Entity* colider);
};

