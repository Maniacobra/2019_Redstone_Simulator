#pragma once
#ifndef PROGRAM_CONST_H_INCLUDED
#define PROGRAM_CONST_H_INCLUDED

#include <string>

// Win
const int WIN_X = 700;
const int WIN_Y = 700;
// Events
const float ZOOM_RATIO = 0.95f;
const int MIN_ZOOM = 64;
const int MAX_ZOOM = 1024;
const int MOUSE_X_CORRECTION = -7;
const int MOUSE_Y_CORRECTION = -29;
// Map
const int CHUNKS_SIZE = 16;
const int SPRITE_SIZE = 16;
const float CUSTOM_DRAW_ZONE_RATIO = 1.f;
// Gameplay
const int REDSTONE_MAX_SIGNAL = 16;
const float TICK_DURATION = 0.1f;
// Files
const std::string TEXTURE_FOLDER("resources/minecraft/");
const std::string LOG_FILE("log.txt");
// Interface
const float TOOLBAR_SIZE = 3.f;
// Menus
enum { FCT_QUIT, LOOP_GAME };

#endif // !PROGRAM_CONST_H_INCLUDED
