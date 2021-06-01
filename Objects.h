#pragma once
#include <string>

#define WIDTH 111
#define HEIGHT 18

int gl_id = 1;
enum class keyboard_input { up = 0, down, right, left, no_mov, shoot, reload, restart, stale};

interface Objects
{
protected:
	enum speed{normal = 1, fast, ultra_fast };
	speed speed;

public:
	keyboard_input dir;
	int id;
	int x;
	int y;
	int size_x;
	int size_y;
	char* picture;

	int move()
	{
		switch (dir)
		{
		case keyboard_input::up:
			if (check_edges((y - speed), 'y'))
				y -= speed;
			else
				return this->id;
			break;
		case keyboard_input::down:
			if (check_edges((y + speed) + size_y, 'y'))
				y += speed;
			else
				return this->id;
			break;
		case keyboard_input::left:
			if (check_edges((x - speed*3), 'x'))
				x -= speed*3;
			else
				return this->id;
			break;
		case keyboard_input::right:
			if (check_edges((x + speed*3) + size_x, 'x'))
				x += speed*3;
			else
				return this->id;
			break;
		default:
			break;
		}
		return 0;
	}

	bool check_edges(int new_coor, char dir)
	{
		if (dir == 'y')
		{
			if (new_coor > HEIGHT - 3 || new_coor < 1)
				return false;
		}
		else
		{
			if (new_coor > WIDTH || new_coor < 0)
				return false;
		}
		return true;
	}
};

class Projectile : public Objects
{
public:
	Projectile(keyboard_input dir, Objects* origin)
	{
		this->id = gl_id++;
		this->speed = ultra_fast;
		this->dir = dir;
		this->picture = new char[1];
		char pic[] = "O";
		strcpy(this->picture, pic);
		this->size_x = 1;
		this->size_y = 1;
		if (dir == keyboard_input::right)
		{
			this->x = origin->x + origin->size_x;
			this->y = origin->y;
		}
		else if (dir == keyboard_input::left)
		{
			this->x = origin->x;
			this->y = origin->y + origin->size_y / 2;
		}
	}
};

class Player : public Objects
{
public:
	Player()
	{
		this->id = gl_id++;
		this->speed = normal;
		this->dir = keyboard_input::no_mov;
		this->picture = new char[4];
		char pic [] = "}oo>";
		strcpy(this->picture, pic);
		this->size_x = 4;
		this->size_y = 1;
		this->x = 15;
		this->y = 9;
	}

	Projectile shoot()
	{
		return Projectile(keyboard_input::right, this);
	}

	void set_direction(keyboard_input input_dir)
	{
		switch (input_dir)
		{
		case keyboard_input::up:
			if (dir == keyboard_input::down)
				this->dir = keyboard_input::no_mov;
			else
				this->dir = input_dir;
			break;	
		case keyboard_input::down:
			if (dir == keyboard_input::up)
				this->dir = keyboard_input::no_mov;
			else
				this->dir = input_dir;
			break;
		case keyboard_input::right:
			if (dir == keyboard_input::left)
				this->dir = keyboard_input::no_mov;
			else
				this->dir = input_dir;
			break;
		case keyboard_input::left:
			if (dir == keyboard_input::right)
				this->dir = keyboard_input::no_mov;
			else
				this->dir = input_dir;
			break;
		default:
			break;
		}
	}
};

class Collidable : public Objects
{
protected:
	int hp;

public:

	virtual int check_collision(int x, int y, keyboard_input dir) { return 1; };

	virtual int hit() { return 1; };
};

class Terrain : public Collidable
{

};

class Enemy : public Collidable
{

};

class Boss : public Enemy
{

};
