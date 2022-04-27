#include "Controller.h"

Controller::Controller()
{
	
	if (load_font()) {
		ready = false;
	}
	menu_scores.font = font;
	load_scores();
	load_textures();
}

Controller::~Controller()
{
}

bool Controller::load_font()
{
	try {
		std::ifstream config(path_config);
		if (config.is_open()) {
			std::string line;
			while (getline(config, line))
			{
				std::stringstream line_s(line);
				std::string type = "";
				std::string value = "";
				if (line_s >> type >> value) {
					if (type == "font:") {
						font.reset(new sf::Font);
						if (!( font.get()->loadFromFile(value)) ) {
							throw std::runtime_error("No font found!\n");
						}
					}
				}
			}
		}
		else {
			throw std::runtime_error("No config.txt file found!\n");
		}
		config.close();
		return false;
	}
	catch (std::runtime_error& e) {
		std::clog << e.what() << std::endl;
		return true;
	}
}

bool Controller::load_scores()
{
	try {
		list_scores.clear();
		menu_scores.menu.clear();
		std::ifstream config(path_config);
		if (config.is_open()) {
			std::string line;
			while (getline(config, line)){
				std::stringstream line_s(line);
				std::string type = "";
				std::string value = "";
				if (line_s >> type >> value) {
					if (type == "scores:") {
						path_scores = value;
					}
				}
			}
		}else {
			throw std::runtime_error("No config.txt file found!\n");
		}
		config.close();
		std::ifstream scores(path_scores);
		if (scores.is_open()) {
			std::string line;
			int limit = 10;//load only top 10 scores
			while (getline(scores, line) and limit) {
				list_scores.push_back(line);
				menu_scores.add_item(line);
				limit--;
			}
		}
		else {
			throw std::runtime_error("No scores.txt file found!\n");
		}
		scores.close();
	}
	catch (std::runtime_error& e) {
		std::clog << e.what() << std::endl;
		return true;
	}
	return false;
}

bool Controller::add_sort_save_scores(int _score)
{
	try {
		std::ofstream scores(path_scores, std::ofstream::out | std::ofstream::trunc);
		if (scores.is_open()) {

			//time \|
			auto end = std::chrono::system_clock::now();
			std::time_t end_time = std::chrono::system_clock::to_time_t(end);
			char buf[26];
			ctime_s(buf, sizeof(buf), &end_time);
			//time /|

			//add score \|
			std::string score;
			score += std::to_string(_score);
			score += " ";
			score += buf;
			score.pop_back();
			list_scores.push_back(score);
			//add score /|

			//sort by score \|
			std::map<int, std::string> to_sort;
			for (std::string e : list_scores) {
				std::stringstream line(e);
				int temp1;
				std::string temp2;
				std::string temp3;
				if (line >> temp1) {
					while(line >> temp3)temp2 += " " + temp3;
					to_sort.emplace(temp1, temp2);
				}
			}
			list_scores.clear();
			for (std::map<int, std::string>::iterator i = to_sort.begin(); i != to_sort.end(); i++)
			{
				std::string temp(std::to_string(i->first)+" "+i->second);
				list_scores.push_back(temp);
			}
			std::reverse(list_scores.begin(), list_scores.end());
			//sort by score /|

			for (std::string e : list_scores) {
				scores << e <<std::endl;
			}
		}
		else {
			throw std::runtime_error("No config.txt file found!\n");
		}
		scores.close();
		load_scores();
	}
	catch (std::runtime_error& e) {
		std::clog << e.what() << std::endl;
		return true;
	}
	return false;
}

bool Controller::load_textures()
{
	try {
		std::string path_textures;
		std::ifstream config(path_config);
		if (config.is_open()) {
			std::string line;
			while (getline(config, line))
			{
				std::stringstream line_s(line);
				std::string type = "";
				std::string value = "";
				if (line_s >> type >> value) {
					if (type == "textures:") {
						path_textures = value;
					}
				}
			}
		}
		else {
			throw std::runtime_error("No config.txt file found!\n");
		}
		config.close();
		std::ifstream tex(path_textures);
		if (tex.is_open()) {
			std::string line;
			while (getline(tex, line))
			{
				std::stringstream line_s(line);
				std::string name_of_texture = "";
				std::string path_to_texture = "";
				std::shared_ptr<sf::Texture> temp(new sf::Texture);
				if (line_s >> name_of_texture >> path_to_texture) {
					name_of_texture.pop_back();
					temp.get()->loadFromFile(path_to_texture);
					textures.emplace(name_of_texture, temp);
				}
			}
		}else{
			throw std::runtime_error("No textures.txt file found!\n");
		}
		tex.close();
	}
	catch (std::runtime_error& e) {
		std::clog << e.what() << std::endl;
		return true;
	}
	return false;
}
