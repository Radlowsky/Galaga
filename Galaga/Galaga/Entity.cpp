#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

Entity::Entity(int _sx, int _sy):
	size(_sx,_sy)
{
}

bool Entity::collision(Entity* colider)
{
	double _1lx = this->x;
	double _1ly = this->y;
	double _1rx = this->x + this->size.x;
	double _1ry = this->y + this->size.y;
	double _2lx = colider->x;
	double _2ly = colider->y;
	double _2rx = colider->x + colider->size.x;
	double _2ry = colider->y + colider->size.y;

	if (_1lx > _2rx || _2lx > _1rx)
		return false;
	if (_1ly > _2ry || _2ly > _1ry)
		return false;
	return true;
}

bool Entity::next_animation_frame()
{
	if (animation_frame == 1) {
		animation_frame = 2;
	}
	else {
		animation_frame = 1;
	}
	return false;
}
