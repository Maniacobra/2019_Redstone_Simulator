#include "utils.h"
#include "res.h"
#include "display.h"
#include "map.h"
#include "loop.h"

int main(void)
{	
	// Display
	Displayer displayer(WIN_X, WIN_Y);
	// Res
	struct ProgramData {
		TextureResources textures;
	} program_res;
	// Map
	GameMap game_map(program_res.textures, "grass", "bedrock", 4, sf::Color(120, 200, 90));
	game_map.assign_display(displayer);
	// Interface
	InterfaceManager game_interface(&program_res.textures);
	displayer.set_interface(&game_interface);
	// Loop
	gameLoop(displayer, game_map, &game_interface);
	return 0;
}