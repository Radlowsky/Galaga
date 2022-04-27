#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
/*
recommended font_size = 25;
*/
class Menu
{
	int selected_index = 0;
public:
	std::vector<sf::Text> menu;
	sf::Color color				= sf::Color::White;
	sf::Color color_selected	= sf::Color::Red;
	sf::Vector2f size;
	sf::Vector2f where_to_start;
	std::shared_ptr<sf::Font> font;
	int font_size;

	Menu(std::shared_ptr<sf::Font> _font,int _font_size, int _width, int _height, int _x, int _y);

	void move_up();
	void move_down();
	void add_item(std::string text);
	int  item_selected();
	void draw(sf::RenderWindow* _window);
};

