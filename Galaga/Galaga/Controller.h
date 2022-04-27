#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <time.h>
#include <iomanip>

#include "Entity.h"
#include "Menu.h"

class Controller
{
public:
	std::string path_config = "./config/config.txt";
	std::string path_scores = "";
	std::shared_ptr<sf::Font> font;
	std::map<std::string, std::shared_ptr<sf::Texture>> textures;
	Menu menu_scores=Menu(font, 20, 1280, 720, 0, 0);
	std::vector<std::string> list_scores;
	bool ready=true;

	Controller();
	~Controller();

	bool load_font();
	bool load_scores();
	bool add_sort_save_scores(int _score);
	bool load_textures();
};

