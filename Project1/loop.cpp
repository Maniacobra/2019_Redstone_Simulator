#include "utils.h"
#include "loop.h"
#include "display.h"

int gameLoop(Displayer& displayer, GameMap& game_map, InterfaceManager* game_interface)
{	
	BlockType selected_type = BLOCK_AIR;
	sf::Vector2f mouse_pos;
	MapBlock* selected_block(0);
	MapBlock* tmp_selected_block(0);
	int delay = 0;
	while (displayer.running())
	{
		// Selection
		tmp_selected_block = game_map.get_block(displayer.get_mouse_pos());
		if (tmp_selected_block != nullptr && selected_block != tmp_selected_block)
		{	
			if (selected_block != nullptr)
			{
				selected_block->unhighlight();
				displayer.draw_block(selected_block);
			}
			selected_block = tmp_selected_block;
			selected_block->highlight();
			displayer.draw_block(selected_block);
			delay = 0;
		}
		// Events
		if (selected_block != nullptr)
		{
			if (displayer.mouse_pressed(sf::Mouse::Right) && delay == 0)
			{	
				game_map.place_block(selected_block, selected_type);
				displayer.draw_block(selected_block);
				delay = game_map.get_ups() / 4;
				
			}
			else if (displayer.mouse_pressed(sf::Mouse::Left))
			{	
				game_map.place_block(selected_block, BLOCK_AIR);
				displayer.draw_block(selected_block);
				
			}
			if (displayer.key_pressed(sf::Keyboard::F))
			{
				game_map.rot_block(selected_block, false);
				displayer.draw_block(selected_block);

			}
			else if (displayer.key_pressed(sf::Keyboard::G))
			{
				game_map.rot_block(selected_block, true);
				displayer.draw_block(selected_block);
			}
		}
		if (displayer.key_pressed(sf::Keyboard::Num1))
			selected_type = game_interface->change_selection(0);
		else if (displayer.key_pressed(sf::Keyboard::Num2))
			selected_type = game_interface->change_selection(1);
		else if (displayer.key_pressed(sf::Keyboard::Num3))
			selected_type = game_interface->change_selection(2);
		else if (displayer.key_pressed(sf::Keyboard::Num4))
			selected_type = game_interface->change_selection(3);
		else if (displayer.key_pressed(sf::Keyboard::Num5))
			selected_type = game_interface->change_selection(4);
		else if (displayer.key_pressed(sf::Keyboard::Num6))
			selected_type = game_interface->change_selection(5);
		else if (displayer.key_pressed(sf::Keyboard::Num7))
			selected_type = game_interface->change_selection(6);
		else if (displayer.key_pressed(sf::Keyboard::Num8))
			selected_type = game_interface->change_selection(7);
		else if (displayer.key_pressed(sf::Keyboard::Num9))
			selected_type = game_interface->change_selection(8);
		if (displayer.mouse_scrolled(true))
			selected_type = game_interface->scroll(false);
		else if (displayer.mouse_scrolled(false))
			selected_type = game_interface->scroll(true);
		// Reset
		displayer.reset_events();
		// Draw + Update
		if (delay > 0)
			delay--;
		game_map.update(displayer);
		game_interface->update(game_map.get_ups());
		if (!displayer.update())
			return FCT_QUIT;
	}
	return FCT_QUIT;
}
