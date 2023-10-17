#pragma once
#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <cassert>
#include <iostream>
#include <SFML/System/Vector2.hpp>

template <typename T1>
void logw(const sf::Vector2<T1> & coord)
{
	std::cout << "x : " << coord.x << " | y : " << coord.y << "\n";
}

template <typename T2>
void logw(const T2& element)
{
	std::cout << element << "\n";
}

#endif // !LOG_H_INCLUDED
