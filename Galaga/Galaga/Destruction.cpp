#include "Destruction.h"

Destruction::Destruction(double _x, double _y, int _sx, int _sy)
	:x(_x),y(_y),size_x(_sx),size_y(_sy)
{
}

void Destruction::animation()
{
	sf::Clock animation_frame_clock;
	while (this->frame < this->frame_s) {
		sf::Time animation_frame_time = animation_frame_clock.getElapsedTime();
		if (animation_frame_time.asSeconds() >= FRAME_TIME) {
			this->frame++;
			animation_frame_clock.restart();
		}
	}
	this->frame++;
}
