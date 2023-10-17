#include "utils.h"
#include "display.h"

Displayer::Displayer(int win_x, int win_y)
{
	m_window.create(sf::VideoMode(win_x, win_y), "Redstone Simulator");
	m_view = m_window.getDefaultView();
}

void Displayer::set_interface(InterfaceManager* game_interface)
{
	m_game_interface = game_interface;
}

bool Displayer::update(void)
{	
	sf::Event win_event;
	while (m_window.pollEvent(win_event))
	{
		switch (win_event.type)
		{
		case sf::Event::Closed:
			return false;
		case sf::Event::MouseWheelScrolled:
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{	
				if (win_event.mouseWheelScroll.delta > 0)
					m_events.scroll_up = true;
				else
					m_events.scroll_down = true;
			}
			else
				zoom((int)win_event.mouseWheelScroll.delta);
			break;
		case sf::Event::Resized:
			resize();
			break;
		case sf::Event::MouseButtonPressed:
			switch (win_event.mouseButton.button)
			{
			case (sf::Mouse::Left):
				m_events.mouse_left = true;
				break;
			case (sf::Mouse::Middle):
				m_events.mouse_middle = true;
				break;
			case (sf::Mouse::Right):
				m_events.mouse_right = true;
				break;
			}
			break;
		case sf::Event::KeyPressed:
			m_events.keys.push_back(win_event.key.code);
			break;
		}
	}
	// Move
	sf::Vector2f move_vec(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		move_vec.x = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		move_vec.x += 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		move_vec.y = -1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		move_vec.y += 1.f;
	if (move_vec != sf::Vector2f(0.f, 0.f) && !(
		(move_vec.x < 0 && m_view.getCenter().x <= ((float)-m_size + 0.6) * SPRITE_SIZE + m_view.getSize().x / 2.f) ||
		(move_vec.y < 0 && m_view.getCenter().y <= ((float)-m_size + 0.6) * SPRITE_SIZE + m_view.getSize().y / 2.f) ||
		(move_vec.x > 0 && m_view.getCenter().x >= ((float)m_size - 0.6) * SPRITE_SIZE - m_view.getSize().x / 2.f) ||
		(move_vec.y > 0 && m_view.getCenter().y >= ((float)m_size - 0.6) * SPRITE_SIZE - m_view.getSize().y / 2.f)))
	{
		m_view.move(move_vec);
		m_window.setView(m_view);
	}
	// Draw
	sf::FloatRect draw_zone(m_view.getCenter() - m_view.getSize() / 2.f * CUSTOM_DRAW_ZONE_RATIO, m_view.getSize() * CUSTOM_DRAW_ZONE_RATIO);
	for (auto it(m_all_chunks.begin()); it != m_all_chunks.end(); ++it)
	{
		if ((*it)->is_inWindow(draw_zone))
			(*it)->draw(m_window);
	}
	// Interface
	m_game_interface->set_cursor(m_window.mapPixelToCoords(sf::Mouse::getPosition() - m_window.getPosition() + sf::Vector2i(MOUSE_X_CORRECTION, MOUSE_Y_CORRECTION)));
	m_game_interface->draw(m_window);
	m_window.setView(m_view);
	// Visible Zone
	sf::RectangleShape visible_zone;
	visible_zone.setSize(sf::Vector2f(draw_zone.width, draw_zone.height));
	visible_zone.setPosition(sf::Vector2f(draw_zone.left, draw_zone.top));
	visible_zone.setFillColor(sf::Color(0, 0, 0, 0));
	visible_zone.setOutlineColor(sf::Color::Red);
	visible_zone.setOutlineThickness(5.f);
	m_window.draw(visible_zone);
	m_window.display();
	return true;
}

void Displayer::zoom(int ticks)
{	
	if (m_view.getSize().x < MIN_ZOOM && ticks < 0 || m_view.getSize().x > MAX_ZOOM && ticks > 0)
		return;
	m_view.zoom(ticks < 0 ? pow(1 / ZOOM_RATIO, ticks) : pow(ZOOM_RATIO, -ticks));
	if (m_view.getCenter().x <= ((float)-m_size + 0.5) * SPRITE_SIZE + m_view.getSize().x / 2.f)
		m_view.setCenter(sf::Vector2f(((float)-m_size + 1) * SPRITE_SIZE + m_view.getSize().x / 2.f, m_view.getCenter().y));
	else if (m_view.getCenter().x >= ((float)m_size - 1) * SPRITE_SIZE - m_view.getSize().x / 2.f)
		m_view.setCenter(sf::Vector2f(((float)m_size - 1) * SPRITE_SIZE - m_view.getSize().x / 2.f, m_view.getCenter().y));
	if (m_view.getCenter().y <= ((float)-m_size + 1) * SPRITE_SIZE + m_view.getSize().y / 2.f)
		m_view.setCenter(sf::Vector2f(m_view.getCenter().x, ((float)-m_size + 1) * SPRITE_SIZE + m_view.getSize().y / 2.f));
	else if (m_view.getCenter().y >= ((float)m_size - 1) * SPRITE_SIZE - m_view.getSize().y / 2.f)
		m_view.setCenter(sf::Vector2f(m_view.getCenter().x, ((float)m_size - 1) * SPRITE_SIZE - m_view.getSize().y / 2.f));
	m_window.setView(m_view);
}

void Displayer::resize(void)
{
	m_view.setSize(sf::Vector2f(m_window.getSize()));
	m_window.setView(m_view);
}

void Displayer::add_chunk(sf::Vector2i pos)
{	
	Chunk* new_chunk(new Chunk(pos));
	m_chunks_grid[pos] = new_chunk;
	m_all_chunks.push_back(new_chunk);
}

void Displayer::set_chunks(int size)
{
	for (int iy = -size; iy < size; iy++)
		for (int ix = -size; ix < size; ix++)
			add_chunk(sf::Vector2i(ix, iy));
}

bool Displayer::add_block(MapBlock* block)
{
	for (auto it(m_chunks_grid.begin()); it != m_chunks_grid.end(); ++it)
	{
		if (it->second->add_block(block))
		{	
			block->set_chunk_pos(it->first);
			it->second->draw_block(block);
			return true;
		}
	}
	return false;
}

void Displayer::set_blocks(int size, std::vector<MapBlock*>& blocks)
{	
	m_size = size;
	sf::Vector2i block_pos;
	sf::Vector2i chunk_pos;
	Chunk* new_chunk(0);
	int unadded_blocks = 0;
	for (auto it(blocks.begin()); it != blocks.end(); ++it)
	{	
		if (!add_block(*it))
			unadded_blocks++;
	}
	for (auto it(m_all_chunks.begin()); it != m_all_chunks.end(); ++it)
		(*it)->display();
	if (unadded_blocks > 0)
		logw("Blocks unplaced !");
}

void Displayer::draw_block(MapBlock* block)
{	
	if (block->is_drawable())
		m_chunks_grid[block->get_chunk_pos()]->draw_block(block);
}

void Displayer::reset_events(void)
{
	m_events.keys.clear();
	m_events.mouse_left = false;
	m_events.mouse_middle = false;
	m_events.mouse_right = false;
	m_events.scroll_up = false;
	m_events.scroll_down = false;
}

sf::Vector2f Displayer::get_mouse_pos(void)
{	
	sf::Vector2i pos(sf::Mouse::getPosition());
	pos -= m_window.getPosition();
	pos.x += MOUSE_X_CORRECTION;
	pos.y += MOUSE_Y_CORRECTION;
	if (pos.x < 0)
		pos.x = 0;
	else if (pos.x >= (signed int)m_window.getSize().x)
		pos.x = m_window.getSize().x - 1;
	if (pos.y < 0)
		pos.y = 0;
	else if (pos.y >= (signed int)m_window.getSize().y)
		pos.y = m_window.getSize().y - 1;
	sf::Vector2f map_pos(m_window.mapPixelToCoords(pos) - sf::Vector2f(4.f, 4.f));
	return map_pos;
}

bool Displayer::running(void) const
{
	return m_window.isOpen();
}

bool Displayer::mouse_click(int button) const
{	
	switch (button)
	{
	case sf::Mouse::Left:
		return m_events.mouse_left;
	case sf::Mouse::Middle:
		return m_events.mouse_middle;
	case sf::Mouse::Right:
		return m_events.mouse_right;
	}
	return false;
}

bool Displayer::key_pressed(int key) const
{
	for (auto it(m_events.keys.begin()); it != m_events.keys.end(); ++it)
	{
		if (*it == key)
			return true;
	}
	return false;
}

bool Displayer::mouse_pressed(int button) const
{
	return sf::Mouse::isButtonPressed(sf::Mouse::Button(button));
}

bool Displayer::mouse_scrolled(bool upward) const
{
	if (upward && m_events.scroll_up)
		return true;
	if (!upward && m_events.scroll_down)
		return true;
	return false;
}
