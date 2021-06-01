#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <string>
#include "Objects.h"

using namespace std;

bool game_over;

class Window
{
	enum game_state { Run = 0, Menu, Start, End };

	keyboard_input user_input;
	game_state state;

	vector<Collidable> ingame_obj;
	vector<Projectile> projectiles;
	Player player = Player();

	unsigned char bm_run[HEIGHT][WIDTH];
	unsigned char bm_menu[HEIGHT][WIDTH];
	unsigned char bm_start[HEIGHT][WIDTH];
	unsigned char bm_end[HEIGHT][WIDTH];

	unsigned char score[6] = { "00000" };
	unsigned char health = '3';
	unsigned char ammo = '6';
	int reloading = 0;
	int immunity = -1;

public:
	Window()
	{
		setup_bm_start();
		setup_bm_run();
		setup_bm_menu();
		state = Start;
		user_input = keyboard_input::stale;
		game_over = false;
	};

	game_state get_state()
	{
		return state;
	}

	void insert_object(Collidable obj)
	{
		ingame_obj.push_back(obj);
	}

	void insert_projectile(Projectile pr)
	{
		projectiles.push_back(pr);
	}

	void remove_object(int id)
	{
		auto iter = ingame_obj.begin();
		while (iter != ingame_obj.end())
		{
			if (iter->id == id)
			{
				ingame_obj.erase(iter);
				break;
			}
			iter++;
		}
	}

	void remove_projectile(int id)
	{
		auto iter = projectiles.begin();
		while (iter != projectiles.end())
		{
			if (iter->id == id)
			{
				projectiles.erase(iter);
				break;
			}
			iter++;
		}
	}

	void draw()
	{
		switch (state)
		{
		case Run:
			draw_bm(bm_run);
			break;
		case Menu:
			draw_bm(bm_menu);
			break;
		case Start:
			draw_bm(bm_start);
			Sleep(400);
			state = Run;
			break;
		case End:
			setup_bm_end();
			draw_bm(bm_end);
			Sleep(3000);
			game_over = true;
			break;
		default:
			break;
		}
	}

	void take_input()
	{
		if (_kbhit())
		{
			if (state == Run)				// Input in game
			{
				switch (_getch())
				{
				case 'a':
					user_input = keyboard_input::left;
					break;
				case 'd':
					user_input = keyboard_input::right;
					break;
				case 'w':
					user_input = keyboard_input::up;
					break;
				case 's':
					user_input = keyboard_input::down;
					break;
				case 'm':
					state = Menu;
					break;
				case '0':
					user_input = keyboard_input::shoot;
					break;
				case 'r':
					user_input = keyboard_input::reload;
					break;

				default:
					break;
				}
			}
			else if (state == Menu)			// Input in menu
			{
				switch (_getch())
				{
				case 'e':
					state = End;
					break;
				case 'm':
					state = Run;
					break;
				case 'r':
					state = Start;
					break;

				default:
					break;
				}
			}
		}
	}

	void logic()
	{
		process_input();
		move_objects();
		collisions();
		reload();

		redraw();
		user_input = keyboard_input::stale;
	}

private:
	void inline draw_bm(unsigned char bm[HEIGHT][WIDTH])
	{
		system("cls");
		for (int row = 0; row < HEIGHT; row++)
			for (int col = 0; col < WIDTH; col++)
				printf("%c", bm[row][col]);
	}
	
	void setup_bm_start()
	{
		string intro = "SPACE IMPACT PRO";
		string info1 = "keyboard_input:   -W,A,S,D-";
		string info2 = "Shooting:   -Space-  ";
		string info3 = "Reload:     -R-      ";
		string info4 = "Menu:       -M-      ";

		for (int row = 0; row < HEIGHT; row++)
		{
			for (int col = 0; col < WIDTH; col++)
			{
				if (col == WIDTH - 1)
					bm_start[row][col] = '\n';
				else if (row == 0 || row == HEIGHT - 1)
					bm_start[row][col] = '#';
				else if (col == 0 || col == WIDTH - 2)
					bm_start[row][col] = '#';
				else if (row == 4 && col == ((WIDTH - intro.length()) / 2))
				{
					for (int s = 0; s < intro.length(); s++)
					{
						bm_start[row][col] = intro[s];
						col++;
					}
					col--;
				}
				else if (row == 7 && col == ((WIDTH - info1.length()) / 2))
				{
					for (int s = 0; s < info1.length(); s++)
					{
						bm_start[row][col] = info1[s];
						col++;
					}
					col--;
				}
				else if (row == 9 && col == ((WIDTH - info2.length()) / 2))
				{
					for (int s = 0; s < info2.length(); s++)
					{
						bm_start[row][col] = info2[s];
						col++;
					}
					col--;
				}
				else if (row == 11 && col == ((WIDTH - info3.length()) / 2))
				{
					for (int s = 0; s < info3.length(); s++)
					{
						bm_start[row][col] = info3[s];
						col++;
					}
					col--;
				}
				else if (row == 13 && col == ((WIDTH - info4.length()) / 2))
				{
					for (int s = 0; s < info4.length(); s++)
					{
						bm_start[row][col] = info4[s];
						col++;
					}
					col--;
				}
				else
					bm_start[row][col] = ' ';
			}
		}
	}

	void setup_bm_run()
	{
		string sc = "Score:  ";
		string h = "Health:  ";
		string a = "Ammo:  ";
		for (int row = 0; row < HEIGHT; row++)
		{
			for (int col = 0; col < WIDTH; col++)
			{
				if (col == WIDTH - 1)
					bm_run[row][col] = '\n';
				else if (row == 0 || row == HEIGHT - 3)
					bm_run[row][col] = '#';
				else if (row == HEIGHT - 2 && col == 10)
				{
					for (int s = 0; s < sc.length(); s++)
					{
						bm_run[row][col] = sc[s];
						col++;
					}
					col--;
				}
				else if (row == HEIGHT - 2 && col == 30)
				{
					for (int s = 0; s < h.length(); s++)
					{
						bm_run[row][col] = h[s];
						col++;
					}
					col--;
				}
				else if (row == HEIGHT - 2 && col == 50)
				{
					for (int s = 0; s < a.length(); s++)
					{
						bm_run[row][col] = a[s];
						col++;
					}
					col--;
				}
				else
					bm_run[row][col] = ' ';
			}
		}
		draw_score();
		draw_ammo();
		draw_health();
	}

	void setup_bm_menu()
	{
		string headline = "MENU ";
		string opt1 = "Press R for restart ";
		string opt2 = "Press E for Exit ";

		for (int row = 0; row < HEIGHT; row++)
		{
			for (int col = 0; col < WIDTH; col++)
			{
				if (col == WIDTH - 1)
					bm_menu[row][col] = '\n';
				else if (row == 0 || row == HEIGHT - 1)
					bm_menu[row][col] = '#';
				else if (col == 0 || col == WIDTH - 2)
					bm_menu[row][col] = '#';
				else if (row == 5 && col == ((WIDTH - headline.length()) / 2))
				{
					for (int s = 0; s < headline.length(); s++)
					{
						bm_menu[row][col] = headline[s];
						col++;
					}
					col--;
				}
				else if (row == 8 && col == ((WIDTH - opt1.length()) / 2))
				{
					for (int s = 0; s < opt1.length(); s++)
					{
						bm_menu[row][col] = opt1[s];
						col++;
					}
					col--;
				}
				else if (row == 11 && col == ((WIDTH - opt2.length()) / 2))
				{
					for (int s = 0; s < opt2.length(); s++)
					{
						bm_menu[row][col] = opt2[s];
						col++;
					}
					col--;
				}
				else
					bm_menu[row][col] = ' ';
			}
		}
	}

	void setup_bm_end()
	{
		string end_score = "Your score:  ";
		for (int k = 0; k < 6; k++)
			end_score += score[k];
		for (int row = 0; row < HEIGHT; row++)
		{
			for (int col = 0; col < WIDTH; col++)
			{
				if (col == WIDTH - 1)
					bm_end[row][col] = '\n';
				else if (row == 0 || row == HEIGHT - 1)
					bm_end[row][col] = '#';
				else if (col == 0 || col == WIDTH - 2)
					bm_end[row][col] = '#';
				else if (row == 5 && col == ((WIDTH - end_score.length()) / 2))
				{
					for (int s = 0; s < end_score.length(); s++)
					{
						bm_end[row][col] = end_score[s];
						col++;
					}
					col -= 2;
				}
				else
					bm_end[row][col] = ' ';
			}
		}
	}

	void inc_score()
	{
		for (int k = 4; k >= 0; k--)
		{
			if (score[k] == '9')
				score[k] = '0';  
			else
			{
				score[k]++;
				break;
			}
		}
	}

	void draw_score()
	{
		for (int k = 0; k < 6; k++)
		{
			bm_run[HEIGHT - 2][20+k] = score[k];
		}
	}

	void draw_health()
	{
		bm_run[HEIGHT - 2][40] = health;
	}

	void draw_ammo()
	{
		bm_run[HEIGHT - 2][60] = ammo;
	}

	void move_objects()
	{
		int del_obj, stop_player;
		for (int k = 0; k < ingame_obj.size(); k++)
		{
			del_obj = ingame_obj[k].move();
			if (del_obj != 0)
				remove_object(del_obj);
		}
		for (int k = 0; k < projectiles.size(); k++)
		{
			del_obj = projectiles[k].move();
			if (del_obj != 0)
				remove_projectile(del_obj);
		}
		player.move();
	}

	void redraw()
	{
		setup_bm_run();
		int k = 0;
		for (auto obj : ingame_obj)
		{
			for (int row = obj.x; row < obj.x + obj.size_x; row++)
				for (int col = obj.y; col < obj.y + obj.size_y; col++)
				{
					bm_run[col][row] = obj.picture[k];
					k++;
				}
			k = 0;
		}
		for (auto obj : projectiles)
		{
			for (int row = obj.x; row < obj.x + obj.size_x; row++)
				for (int col = obj.y; col < obj.y + obj.size_y; col++)
				{
					bm_run[col][row] = obj.picture[k];
					k++;
				}
			k = 0;
		}

		for (int i = player.x; i < player.x + player.size_x; i++)
		{
			bm_run[player.y][i] = player.picture[k];
			k++;
		}
	}

	void process_input()
	{
		if (user_input < keyboard_input::no_mov)
			player.set_direction(user_input);
		else if (user_input == keyboard_input::reload)
		{
			if (reloading < 0)
				reloading = 5;
		}
		else if (user_input == keyboard_input::shoot)
		{
			if (ammo > '0')
			{
				ammo -= 1;
				insert_projectile(player.shoot());
			}
		}
	}

	void reload()
	{
		if (reloading > 0)
			reloading--;
		else if (reloading == 0)
		{
			ammo = '6';
			reloading--;
		}		
	}

	void health_dec()
	{
		health--;
		immunity = 4;
		player.x = 15;
		player.y = 9;
	}

	void collisions()
	{

		for (int k = 0; k < ingame_obj.size(); k++)
		{
			for (int i = 0; i < projectiles.size(); i++)
			{
				int obj_id = ingame_obj[k].check_collision(projectiles[i].x, projectiles[i].y, projectiles[i].dir);
				if (obj_id != 0)
				{
					remove_projectile(projectiles[i].id);
					if (ingame_obj[k].hit() == 0)
						remove_object(obj_id);
				}
			}
			if (immunity < 0)
			{
				int obj_id = ingame_obj[k].check_collision(player.x, player.y, player.dir);
				if (obj_id != 0)
				{
					health_dec();
					if (ingame_obj[k].hit() == 0)
						remove_object(obj_id);
				}
			}
			else
			{
				immunity--;
			}
		}

	}
};

int main(void)
{
	Window Game = Window();

	while (!game_over)
	{
		Game.draw();
		Game.take_input();
		if (Game.get_state() == 1)
		{
			Game.draw();
			while (Game.get_state() == 1)
				Game.take_input();
		}
		Game.logic();
		Sleep(120);
	}
	system("cls");
}
