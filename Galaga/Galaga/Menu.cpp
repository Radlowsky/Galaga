#include "Menu.h"

Menu::Menu(std::shared_ptr<sf::Font> _font, int _font_size, int _width, int _height, int _x, int _y)
	: size(_width, _height), where_to_start(_x,_y), font_size(_font_size), font(_font)
{

}

void Menu::move_up()
{
	if (selected_index - 1 >= 0) {
		menu[selected_index].setFillColor(color);
		selected_index--;
		menu[selected_index].setFillColor(color_selected);
	}
	else {
		menu[selected_index].setFillColor(color_selected);
	}
}

void Menu::move_down()
{
	if ( selected_index + 1 < menu.size() ) {
		menu[selected_index].setFillColor(color);
		selected_index++;
		menu[selected_index].setFillColor(color_selected);
	}
	else {
		menu[selected_index].setFillColor(color_selected);
	}
}

void Menu::add_item(std::string text)
{
	sf::Text temp;
	temp.setString(text);
	temp.setFont(*(font.get()));
	temp.setFillColor(color);
	temp.setCharacterSize(font_size);
	menu.push_back(temp);
}

int Menu::item_selected()
{
	return selected_index;
}

void Menu::draw(sf::RenderWindow* _window)
{
	sf::RectangleShape r;
	r.setPosition(where_to_start);
	r.setSize(size);
	r.setFillColor(sf::Color::Black);
	(*_window).draw(r);
	for (int i = 0; i < menu.size(); i++) {
		menu[i].setPosition(sf::Vector2f(where_to_start.x + (size.x / 2) - ((menu[i].getGlobalBounds().width) / 2), where_to_start.y + size.y / (menu.size() + 1) * (i + 1) - ((menu[i].getGlobalBounds().height/2))));
		(*_window).draw(menu[i]);
	}
}
