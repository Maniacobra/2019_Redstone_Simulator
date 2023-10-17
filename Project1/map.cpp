#include "utils.h"
#include "map.h"

GameMap::GameMap(TextureResources& textures, std::string ground, std::string border, int size, sf::Color ground_color)
{	
	m_textures = &textures;
	m_size = size * CHUNKS_SIZE;
	MapBlock* new_block(0);
	for (int iy = -m_size + 1; iy < m_size; iy++)
		for (int ix = -m_size + 1; ix < m_size; ix++)
		{	
			bool is_border = ix == -m_size + 1 || iy == -m_size + 1 || ix == m_size - 1 || iy == m_size - 1;
			new_block = new MapBlock(&m_textures->get_texture(is_border ? border : ground, 0), sf::Vector2i(ix, iy),
				is_border ? sf::Color::White : ground_color, is_border);
			m_grid[sf::Vector2i(ix, iy)] = new_block;
			m_all_blocks.push_back(new_block);
			m_nb++;
		}
	m_last_second = (long)time(NULL);
	m_last_tick = 0;
}

void GameMap::update(Displayer& displayer)
{	
	m_current_updates++;
	if (time(NULL) - m_last_second > 0)
	{
		m_ups = m_current_updates;
		m_current_updates = 0;
		m_last_second = (long)time(NULL);
	}
	if (m_last_tick >= m_ups * TICK_DURATION)
	{	
		for (auto it(m_ticked_behaviors.begin()); it != m_ticked_behaviors.end(); ++it)
			(*it)->update();
		for (auto it(m_ticked_behaviors.begin()); it != m_ticked_behaviors.end(); ++it)
			(*it)->post_update();
		m_last_tick = 0;
	}
	for (auto it(m_active_blocks.begin()); it != m_active_blocks.end(); ++it)
	{
		if ((*it)->get_behavior()->is_undrawed())
			displayer.draw_block(*it);
	}
	m_last_tick++;
}

void GameMap::assign_display(Displayer& displayer)
{	
	displayer.set_chunks(m_size / CHUNKS_SIZE);
	displayer.set_blocks(m_size, m_all_blocks);
}

MapBlock* GameMap::get_block(sf::Vector2f pos)
{	
	pos /= (float)SPRITE_SIZE;
	if (pos.x < 0)
		pos.x -= 0.25;
	else if (pos.x > 0)
		pos.x += 0.75;
	if (pos.y < 0)
		pos.y -= 0.25;
	else if (pos.y > 0)
		pos.y += 0.75;
	if (pos.x > -m_size && pos.x < m_size && pos.y > -m_size && pos.y < m_size)
		return m_grid[(sf::Vector2i)pos];
	return nullptr;
}

MapBlock* GameMap::get_block(int x, int y)
{
	if (x >= -m_size && x <= m_size && y >= -m_size && y <= m_size)
		return m_grid[sf::Vector2i(x, y)];
	return nullptr;
}

void GameMap::place_block(MapBlock* block, int behavior_id, BlockOrientation rot, std::string param, bool auto_rotate)
{	
	if (block == nullptr || block->is_bedrock())
		return;
	if (block->get_behavior()->get_id() == behavior_id && (block->get_behavior()->get_rot() == rot || auto_rotate))
	{
		block->get_behavior()->edit(param);
		return;
	}
	// Verif
	sf::Vector2i block_pos(block->get_pos());
	// Neighbors
	BlockBehavior* block_neighbors[] = {
		get_block(block_pos.x, block_pos.y - 1)->get_behavior(),
		get_block(block_pos.x + 1, block_pos.y)->get_behavior(),
		get_block(block_pos.x, block_pos.y + 1)->get_behavior(),
		get_block(block_pos.x - 1, block_pos.y)->get_behavior()
	};
	// New behavior
	BlockBehavior* new_behavior;
	switch (behavior_id)
	{
	case BLOCK_IRON_BLOCK:
		new_behavior = new SimpleBlock;
		param = "iron_block";
		auto_rotate = false;
		break;
	case BLOCK_REDSTONE_WIRE:
		new_behavior = new RedstoneWire;
		auto_rotate = false;
		break;
	case BLOCK_REDSTONE_BLOCK:
		new_behavior = new RedstoneBlock;
		auto_rotate = false;
		break;
	case BLOCK_REPEATER:
		new_behavior = new Repeater;
		break;
	case BLOCK_TORCH:
		new_behavior = new Torch;
		break;
	default:
		new_behavior = new BlockBehavior;
		auto_rotate = false;
	}
	// Destroy
	if (block->get_behavior()->is_active())
		m_active_blocks.remove(block);
	if (block->get_behavior()->is_ticked())
		m_ticked_behaviors.remove(block->get_behavior());
	block->get_behavior()->destroy();
	// Set
	block->set_behavior(new_behavior);
	new_behavior->init(*m_textures, block_neighbors, rot, param, auto_rotate);
	if (new_behavior->is_active())
		m_active_blocks.push_back(block);
	if (new_behavior->is_ticked())
		m_ticked_behaviors.push_back(new_behavior);
}

void GameMap::rot_block(MapBlock* block, bool clockwise)
{
	if (block == nullptr)
		return;
	place_block(block, block->get_behavior()->get_id(), Utils::rotate(clockwise, block->get_behavior()->get_rot()), block->get_behavior()->get_param(), false);
}

int GameMap::get_ups(void) const
{
	return m_ups;
}
