#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <stdlib.h>//rand()

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Enemy.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Player.h"
#include "Bullet.h"
#include "Shield.h"
#include "Controller.h"
#include "Menu.h"
#include "Destruction.h"

#define WINDOW_X 1280
#define WINDOW_Y 720
#define SIZE_X 100
#define SIZE_Y 100
#define NUMBER_OF_ENEMYS 21
#define NUMBER_OF_ENEMYS_IN_ROW 7
#define LEVEL_OF_BEGIN 1
#define PLAYER_MAX_HEALTH 3
#define NUMBER_OF_SHIELDS 3
#define ENEMY_MOVMENT_DELAY 0.5f
#define ENEMY_SHOT_DELAY 2.0f
#define PLAYER_SHOT_DELAY 0.5f
#define BULLET_SPEED 0.75f

class Game
{
public:
	Controller controller;
	Menu LOGO;
	Menu menu_main;
	Menu menu_pause;
	Menu menu_game_over;
	Menu menu_score_board;

	/*
		Mapping for multiple input control
	*/
	std::map<std::string, bool> mapping = {
		{"A",false},
		{"D",false},
		{"space",false},
		{"pause",true}
	};

	/*
		1-main menu
		2-pause
		3-game
		4-game over
		5-score board
	*/
	std::map<int, bool> menu = {
		{1,true},
		{2,false},
		{3,false},
		{4,false},
		{5,false}
	};

	std::vector<std::shared_ptr<Entity>> objects;
	std::vector<Destruction> destructions;
	std::shared_ptr<Entity> player;

	sf::RenderWindow window;
	
	sf::Clock enemy_move_clock;
	sf::Clock enemy_shot_clock;
	sf::Clock bullet_shot_clock;

	sf::Clock frame_clock;
	sf::Time Elapsed_frame_time;
	
	int count;
	int max_count = count;
	int row;
	int level;
	int health;
	unsigned int score;
	float player_x;//for saving player location
	float player_y;//for saving player location
	const int shield_number;
	const float enemy_move;
	const float enemy_shot;
	const float player_shot;
	const float bullet_speed;
	
	Game(Controller& _controller):
		controller(_controller),
		LOGO(controller.font, 60, 1280, 200, 0, 0),
		menu_main(controller.font, 25, 1280, 520, 0, 200),
		menu_pause(controller.font, 25, 1280, 720, 0, 0),
		menu_game_over(controller.font, 60, 1280, 720, 0, 0),
		menu_score_board(controller.font, 25, 1280 / 6, 720 / 6, 0, 0),
		count(NUMBER_OF_ENEMYS),
		max_count(count),
		row(NUMBER_OF_ENEMYS_IN_ROW),
		level(LEVEL_OF_BEGIN),
		health(PLAYER_MAX_HEALTH),
		score(0),
		player_x(0),
		player_y(0),
		shield_number(NUMBER_OF_SHIELDS),
		enemy_move(ENEMY_MOVMENT_DELAY),
		enemy_shot(ENEMY_SHOT_DELAY),
		player_shot(PLAYER_SHOT_DELAY),
		bullet_speed(BULLET_SPEED)
	{
		if (controller.ready) {
			srand(time(NULL));

			window.create(sf::VideoMode(1280, 720), "SPACE INVAIDERS BY J.R.", sf::Style::Close);
			window.setVerticalSyncEnabled(true);

			LOGO.color = sf::Color::Magenta;
			LOGO.add_item("SPACE INVAIDERS BY J.R.");

			menu_main.add_item("START");
			menu_main.add_item("SCORE BOARD");
			menu_main.add_item("EXIT");
			menu_main.color_selected = sf::Color::Green;
			menu_main.move_up();

			menu_pause.add_item("RESUME");
			menu_pause.add_item("SAVE EXIT");
			menu_pause.color_selected = sf::Color::Green;
			menu_pause.move_up();

			menu_game_over.color = sf::Color::Red;
			menu_game_over.add_item("YOU DIED");

			menu_score_board.add_item("BACK");
			menu_score_board.color_selected = sf::Color::Green;
			menu_score_board.move_up();

			inicize_game();
			open_window();
		}
		else {
			std::cerr << "files corrupted, mission abort!" << std::endl;
		}
	}
	~Game() {};

	void inicize_game() {
		health = PLAYER_MAX_HEALTH;
		count = NUMBER_OF_ENEMYS;
		objects.clear();
		for (int i = 0, y = -1; i < count; i++) {
			int random = rand() % 3;
			if (random == 0) {
				auto temp = new Enemy1;
				temp->size.x = SIZE_X;
				temp->size.y = SIZE_Y;
				objects.push_back(std::shared_ptr<Entity>(temp));
			}
			else
				if (random == 1) {
					auto temp = new Enemy2;
					temp->size.x = SIZE_X;
					temp->size.y = SIZE_Y;
					objects.push_back(std::shared_ptr<Entity>(temp));
				}
				else
					if (random == 2) {
						auto temp = new Enemy3;
						temp->size.x = SIZE_X;
						temp->size.y = SIZE_Y;
						objects.push_back(std::shared_ptr<Entity>(temp));
					}
			if (static_cast<int>(objects[i].get()->size.x * i )% row == 0)y++;
			objects[i].get()->x += objects[i].get()->size.x * (i % row) + static_cast<float>(objects[i].get()->size.x / 5) * (i % row);
			objects[i].get()->y += objects[i].get()->size.y * y + static_cast<float>(objects[i].get()->size.y / 5) * y;
		}
		auto temp = new Player;
		temp->size.x = SIZE_X;
		temp->size.y = SIZE_Y;
		temp->x = player_x;
		temp->y = player_y;
		objects.push_back(std::shared_ptr<Entity>(temp));
		player = objects[objects.size() - 1];
		for (int i = 1; i <= shield_number; i++) {
			auto temp = new Shield;
			temp->size.x = SIZE_X;
			temp->size.y = SIZE_Y;
			objects.push_back(std::shared_ptr<Entity>(temp));
			objects[objects.size() - 1].get()->x = static_cast<float>(i * window.getSize().x / (shield_number + 1)) - static_cast<float>(objects[objects.size() - 1].get()->size.x / 2);
			objects[objects.size() - 1].get()->y = window.getSize().y - objects[objects.size() - 1].get()->size.y * 2 - 1;
		}
	}
	void open_window() {
		while (window.isOpen())
		{
			//frame_delay
			Elapsed_frame_time = frame_clock.restart();
			//events
			user_events();
			anty_suprise_atack();
			event_game_over();
			new_level();
			//controls
			player_controll();
			bullet_move_controll();
			enemy_move_controll();
			enemy_shot_controll();
			colision_controll();
			//drawing
			window.setActive();
			window.clear();
			draw_objects_controll();
			draw_destructions_controll();
			draw_interface_controll();
			draw_menu_controll();
			window.display();
		}
	}

	void user_events() {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				if (menu[4]) {
					controller.add_sort_save_scores(score);
				}
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (menu[1]) {
					if (event.key.code == sf::Keyboard::W) {
						menu_main.move_up();
					}
					else
						if (event.key.code == sf::Keyboard::S) {
							menu_main.move_down();
						}
						else
							if (event.key.code == sf::Keyboard::Enter) {
								switch (menu_main.item_selected()) {
								case 0:
									score = 0;
									player_x = static_cast<float>(window.getSize().x / 2) - SIZE_X / 2;
									player_y = window.getSize().y - SIZE_Y;
									inicize_game();
									menu[1] = false;
									menu[3] = true;
									mapping["pause"] = false;
									break;
								case 1:
									menu[1] = false;
									menu[5] = true;
									break;
								case 2:
									window.close();
									break;
								}
							}
				}
				else
					if (menu[2]) {
						if (event.key.code == sf::Keyboard::W) {
							menu_pause.move_up();
						}
						else
							if (event.key.code == sf::Keyboard::S) {
								menu_pause.move_down();
							}
							else
								if (event.key.code == sf::Keyboard::Escape) {
									menu[2] = false;
									menu[3] = true;
									mapping["pause"] = false;
								}
								else
									if (event.key.code == sf::Keyboard::Enter) {
										switch (menu_pause.item_selected()) {
										case 0:
											menu[2] = false;
											menu[3] = true;
											mapping["pause"] = false;
											break;
										case 1:
											menu[2] = false;
											menu[1] = true;
											controller.add_sort_save_scores(score);
											break;
										}
									}
					}
					else
						if (menu[3]) {
							if (event.key.code == sf::Keyboard::A) {
								mapping["A"] = true;
							}
							else
								if (event.key.code == sf::Keyboard::D) {
									mapping["D"] = true;
								}
								else
									if (event.key.code == sf::Keyboard::Space) {
										mapping["space"] = true;
									}
									else
										if (event.key.code == sf::Keyboard::Escape) {
											// gra w pausie
											menu[3] = false;
											menu[2] = true;
											mapping["pause"] = true;
										}
						}
						else
							if (menu[4]) {
								if (event.key.code == sf::Keyboard::Enter) {
									menu[4] = false;
									menu[1] = true;
									controller.add_sort_save_scores(score);
								}
							}
							else
								if (menu[5]) {
									if (event.key.code == sf::Keyboard::Escape) {
										menu[5] = false;
										menu[1] = true;
									}
									else
										if (event.key.code == sf::Keyboard::Enter) {
											menu[5] = false;
											menu[1] = true;
										}
								}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (menu[3]) {
					if (event.key.code == sf::Keyboard::A) {
						mapping["A"] = false;
					}
					else
						if (event.key.code == sf::Keyboard::D) {
							mapping["D"] = false;
						}
						else
							if (event.key.code == sf::Keyboard::Space) {
								mapping["space"] = false;
							}
				}
			}
		}
	};
	void anty_suprise_atack() {
		if (!menu[3]) {
			bullet_shot_clock.restart();
			enemy_shot_clock.restart();
		}
	};
	void event_game_over() {
		if (mapping["pause"] and menu[3] and health <= 0) {
			menu[3] = false;
			menu[4] = true;
		}
	};
	void new_level() {
		if (count <= 0) {
			score += 100 * level * health;
			for (auto& obj : objects) {
				auto shield = dynamic_cast<Shield*>(obj.get());
				auto p = dynamic_cast<Player*>(obj.get());
				if (shield) {
					for (bool is : shield->inside_boxes) {
						if (is) {
							score += 10 * level;
						}
					}
				}
				else
					if (p) {
						player_x = p->x;
						player_y = p->y;
					}
			}
			level++;
			inicize_game();
		}
	};

	void player_controll() {
		if (!mapping["pause"]) {
			if (mapping["A"] and (player.get()->x - 1 * Elapsed_frame_time.asMilliseconds()) >= 0) {
				player.get()->x -= static_cast<float>(1 * Elapsed_frame_time.asMilliseconds() );
			}
			if (mapping["D"] and (player.get()->x + 1 * Elapsed_frame_time.asMilliseconds()) <= static_cast<float>(window.getSize().x - player.get()->size.x) ) {
				player.get()->x += static_cast<float>(1 * Elapsed_frame_time.asMilliseconds() );
			}
			if (mapping["space"]) {
				sf::Time bullet_shot_time = bullet_shot_clock.getElapsedTime();
				if (bullet_shot_time.asSeconds() >= player_shot) {
					std::shared_ptr<Entity> temp(new Bullet);
					temp.get()->size.x = SIZE_X / 10;
					temp.get()->size.y = SIZE_Y / 10;
					temp.get()->x = player.get()->x + static_cast<float>(player.get()->size.x / 2) - (SIZE_X / 20);
					temp.get()->y = player.get()->y - temp.get()->size.y - 1;
					objects.push_back(temp);
					bullet_shot_clock.restart();
				}
			}
		}
	};
	void bullet_move_controll() {
		if (!mapping["pause"]) {
			for (auto& obj : objects) {
				auto bullet = dynamic_cast<Bullet*>(obj.get());
				if (bullet) {
					bullet->y -= bullet_speed * bullet->direction * Elapsed_frame_time.asMilliseconds();
					if (bullet->y <= 0 || bullet->y >= window.getSize().y) {
						obj.reset();
					}
				}
			}
		}
	};
	void enemy_move_controll() {
		if (!mapping["pause"]) {
			sf::Time enemy_move_time = enemy_move_clock.getElapsedTime();
			if (enemy_move_time.asSeconds() >= ((enemy_move)*sqrt(count)) / (sqrt(level))) {
				for (auto& obj : objects) {
					if (obj) {
						auto enemy = dynamic_cast<Enemy*>(obj.get());
						if (enemy) {
							if (enemy->x + static_cast<float>(enemy->size.x / 2) * (enemy->direction) >= window.getSize().x - enemy->size.x ||
								enemy->x + static_cast<float>(enemy->size.x / 2) * (enemy->direction) <= 0 ) {
								for (auto& obj : objects) {
									auto enemy = dynamic_cast<Enemy*>(obj.get());
									if (enemy) {
										enemy->direction *= -1;
										enemy->y += static_cast<float>(enemy->size.y / 2);
										if (enemy->y >= player->y) {
											mapping["pause"] = true;
											health = 0;
										}
									}
								}
							}
						}
					}
				}
				for (auto& obj : objects) {
					if (obj) {
						auto enemy = dynamic_cast<Enemy*>(obj.get());
						if (enemy) {
							enemy->x += static_cast<float>(enemy->size.x / 2) * (enemy->direction);
							enemy->next_animation_frame();
						}
					}
				}
				enemy_move_clock.restart();
			}
		}
	};
	void enemy_shot_controll() {
		if (!mapping["pause"]) {
			sf::Time enemy_shot_time = enemy_shot_clock.getElapsedTime();
			if (enemy_shot_time.asSeconds() >= (enemy_shot) / sqrt(level)) {
				std::map<int, std::shared_ptr<Entity>> ready;
				for (int i = max_count - 1; i >= 0; i--) {
					if (objects[i].get()) {
						bool add = true;
						int j = i;
						while (j <= max_count - 1) {
							j += row;
							if (ready[j]) {
								add = false;
							}
							else {
								ready.erase(j);
							}
						}
						if (add) {
							ready.emplace(i, objects[i]);
						}
					}
				}
				if (ready.size() > 0) {
					auto i = ready.begin();
					int choosen = rand() % ready.size();
					while (choosen > 0) {
						i++;
						choosen--;
					}
					std::shared_ptr<Entity> temp(new Bullet);
					temp.get()->size.x = SIZE_X / 10;
					temp.get()->size.y = SIZE_Y / 10;
					temp.get()->x = i->second.get()->x + static_cast<float>(i->second.get()->size.x / 2) - (SIZE_X / 20);
					temp.get()->y = i->second.get()->y + i->second.get()->size.y + 1;
					temp.get()->direction *= -1;
					objects.push_back(temp);
				}
				enemy_shot_clock.restart();
			}
		}
	};
	void colision_controll() {
		for (auto& obj1 : objects) {
			for (auto& obj2 : objects) {
				if (obj1.get() and obj2.get() and obj1 != obj2) {
					if (obj1->collision(obj2.get())) {
						auto bullet = dynamic_cast<Bullet*>(obj1.get());
						auto player = dynamic_cast<Player*>(obj2.get());
						auto enemy = dynamic_cast<Enemy*>(obj2.get());
						auto enemy1 = dynamic_cast<Enemy*>(obj1.get());
						auto shield = dynamic_cast<Shield*>(obj2.get());
						if (bullet and player) {
							if (health) {
								obj1.reset();
								health--;
							}
							else {
								obj1.reset();
								mapping["pause"] = true;
							}
						}
						else
							if (enemy1 and player) {
								mapping["pause"] = true;
								health = 0;
							}
							else
								if (bullet and enemy) {
									Destruction temp(obj2.get()->x, obj2.get()->y, obj2.get()->size.x, obj2.get()->size.y);
									destructions.push_back(temp);
									std::thread(&Destruction::animation, &destructions[destructions.size() - 1]).detach();//creatin destruction animation
									count--;
									obj1.reset();
									obj2.reset();
									score += 10 * level;
								}
								else
									if (bullet and shield) {
										if (shield->inside_collision(bullet)) {
											obj1.reset();
										}
										else {
											bool destroy = true;
											for (auto box : shield->inside_boxes) {
												if (box) {
													destroy = false;
												}
											}
											if (destroy) {
												obj2.reset();
											}
										}
									}
									else
										if (enemy1 and shield) {
											count--;
											obj1.reset();
											obj2.reset();
										}
					}
				}
			}
		}
	};

	void draw_objects_controll() {
		for (auto& obj : objects) {
			if (obj) {
				sf::RectangleShape r;
				r.setPosition(obj.get()->x, obj.get()->y);
				r.setSize(sf::Vector2f(obj.get()->size.x, obj.get()->size.y));
				r.setFillColor(sf::Color(0, 255, 0, 255));
				if (dynamic_cast<Enemy1*>(obj.get()))
				{
					int rrr = rand() % 2;
					std::string temp("enemy1_");
					/*
					if (rrr == 0) {
						temp = "enemy1_";
					}
					else {
						temp = "enemy2_";
					}*/
					temp += std::to_string(obj.get()->animation_frame);
					r.setTexture(controller.textures[temp].get());
				}
				else
					if (dynamic_cast<Enemy2*>(obj.get()))
					{
						std::string temp("enemy2_");
						temp += std::to_string(obj.get()->animation_frame);
						r.setTexture(controller.textures[temp].get());
					}
					else
						if (dynamic_cast<Enemy3*>(obj.get()))
						{
							std::string temp("enemy3_");
							temp += std::to_string(obj.get()->animation_frame);
							r.setTexture(controller.textures[temp].get());
						}
						else
							if (dynamic_cast<Player*>(obj.get())) {
								r.setFillColor(sf::Color::Magenta);
								r.setTexture(controller.textures["player"].get());
							}
							else
								if (dynamic_cast<Bullet*>(obj.get())) {
									r.setFillColor(sf::Color(255, 255, 255, 255));
									r.setTexture(controller.textures["bullet"].get());
								}
								else
									if (dynamic_cast<Shield*>(obj.get())) {
										auto shield = dynamic_cast<Shield*>(obj.get());
										r.setFillColor(sf::Color(255, 0, 255, 0));
										sf::RectangleShape rs;
										for (int i = 0; i < shield->inside_boxes.size(); i++) {
											if (shield->inside_boxes[i]) {
												float X = (shield->x + static_cast<float>(((i % 3) * shield->size.x) / 3));
												float Y = (shield->y + static_cast<float>(((i / 3) * shield->size.y) / 3));
												rs.setPosition(X, Y);
												rs.setSize(sf::Vector2f(shield->size.x / 3, shield->size.y / 3));
												rs.setFillColor(sf::Color::Magenta);
												//r.setTexture();
												window.draw(rs);
											}
										}
									}
				window.draw(r);
			}
		}
	};
	void draw_destructions_controll() {
		int how_many_to_delete = 0;
		for (auto& obj : destructions) {
			if (obj.frame <= obj.frame_s) {
				sf::RectangleShape r;
				r.setPosition(obj.x, obj.y);
				r.setSize(sf::Vector2f(obj.size_x, obj.size_y));
				r.setFillColor(sf::Color::White);
				std::string temp("destruction_");
				temp += std::to_string(obj.frame);
				r.setTexture(controller.textures[temp].get());
				window.draw(r);
			}
			else {
				how_many_to_delete++;
			}
		}
		while (how_many_to_delete--) {
			destructions.erase(destructions.begin());
		}
	};
	void draw_interface_controll() {
		if (menu[3]) {
			int how_many_health = health;
			float place_x = window.getSize().x - SIZE_X / 4;
			float place_y = window.getSize().y - SIZE_Y / 4;
			while (how_many_health > 0) {
				sf::RectangleShape r;
				r.setPosition(place_x, place_y);
				r.setSize(sf::Vector2f(SIZE_X / 4, SIZE_Y / 4));
				r.setFillColor(sf::Color(255, 255, 255, 255));
				window.draw(r);
				how_many_health--;
				place_x -= SIZE_X / 4 + 10;
			}
			//SCORE
			sf::Text sc;
			std::string text;
			text += "LeveL ";
			text += std::to_string(level);
			text += " | score ";
			text += std::to_string(score);
			sc.setString(text);
			sc.setFont(*(controller.font.get()));
			sc.setCharacterSize(20);
			sc.setPosition(10, window.getSize().y - sc.getGlobalBounds().height - 10);
			window.draw(sc);
			//FPS
			text = "FPS: ";
			text += std::to_string(static_cast<int>(floor(1.0f / Elapsed_frame_time.asSeconds())));
			//std::cout << text << std::endl;
			sc.setString(text);
			sc.setFont(*(controller.font.get()));
			sc.setCharacterSize(20);
			sc.setPosition(0, 0);
			window.draw(sc);
		}
	};
	void draw_menu_controll() {
		if (menu[1]) {
			LOGO.draw(&window);
			menu_main.draw(&window);
		}
		else
			if (menu[2]) {
				menu_pause.draw(&window);
			}
			else
				if (menu[4]) {
					menu_game_over.draw(&window);
				}
				else
					if (menu[5]) {
						controller.menu_scores.draw(&window);
						menu_score_board.draw(&window);
					}
	};
};

