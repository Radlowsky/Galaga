#include "Shield.h"

bool Shield::inside_collision(Entity* colider)
{
    for (int i = 0; i < inside_boxes.size(); i++) {
		if (inside_boxes[i]) {

			int X = x + ((i % 3) * size.x) / 3;
			int Y = y + ((i / 3) * size.y) / 3;

			int _1lx = X;
			int _1ly = Y;
			int _1rx = X + this->size.x/3;
			int _1ry = Y + this->size.y/3;
			int _2lx = colider->x;
			int _2ly = colider->y;
			int _2rx = colider->x + colider->size.x;
			int _2ry = colider->y + colider->size.y;

			if (!(_1lx > _2rx || _2lx > _1rx)) {
				if (!(_1ly > _2ry || _2ly > _1ry)) {
					inside_boxes[i] = false;
					return true;
				}
			}
		}
    }
	return false;
}
