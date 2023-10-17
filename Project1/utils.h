#pragma once
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <SFML/System/Vector2.hpp>
#include "program_const.h"
#include "block_enum.h"
#include "log.h"

class Utils
{
public:

	static BlockOrientation swapside(int side)
	{
		return (BlockOrientation)(side < 2 ? side + 2 : side - 2);
	}

	static BlockOrientation rotate(bool clockwise, int rot)
	{	
		if (clockwise)
		{
			rot++;
			if (rot == 4)
				rot = 0;
		}
		else
		{
			if (rot == 0)
				rot = 3;
			else
				rot--;
		}
		return (BlockOrientation)rot;
	}

	static sf::Vector2f origin(BlockOrientation rot)
	{
		switch (rot)
		{
		case RIGHT:
			return sf::Vector2f(0, SPRITE_SIZE);
		case BOT:
			return sf::Vector2f(SPRITE_SIZE, SPRITE_SIZE);
		case LEFT:
			return sf::Vector2f(SPRITE_SIZE, 0);
		default:
			return sf::Vector2f(0, 0);
		}
	}

	class Vec2i_predicate
	{
	public:
		bool operator()(sf::Vector2i const& v1, sf::Vector2i const& v2) const
		{
			if (v1.y < v2.y)
				return true;
			if (v1.y == v2.y && v1.x < v2.x)
				return true;
			return false;
		}
	};
};

#endif // !UTILS_H_INCLUDED
