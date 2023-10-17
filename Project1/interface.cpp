#include "utils.h"
#include "interface.h"

InterfaceManager::InterfaceManager(TextureResources* textures)
{
	m_textures = textures;
	m_toolbar.setTexture(m_textures->get_texture("toolbar", 0));
	m_alpha_cd = -1;
	for (int i = 0; i < 9; i++)
		if (TOOL_BAR_CONTENT[i] != "")
			m_items[i] = sf::Sprite(textures->get_texture(TOOL_BAR_CONTENT[i], 0));
	m_selection_square.setTexture(textures->get_texture("select_square", 0));
	m_current_selection = 0;
	m_fps_text.setFont(textures->get_font("default"));
	m_fps_text.setCharacterSize(15);
}

void InterfaceManager::update(int ups)
{
	if (m_alpha_cd > 0)
	{
		m_alpha_cd -= 1000 / ups;
		if (m_alpha_cd < 0)
			m_alpha_cd = 0;
		m_selected_item.setColor(sf::Color(255, 255, 255, (int)((float)m_alpha_cd / 500.f * 100.f) + 155));
		if (m_alpha_cd >= 400)
			m_selected_item.setScale((((float)m_alpha_cd - 400.f) / 300.f + 1) * 0.25f, (((float)m_alpha_cd - 400.f) / 300.f + 1) * 0.25f);
	}
	m_fps_text.setString(std::to_string(ups) + " FPS");
}

BlockType InterfaceManager::change_selection(int toolbar_case)
{	
	if (TOOL_BAR_CONTENT[toolbar_case] == "")
		m_alpha_cd = -1;
	else
	{
		m_selected_item.setTexture(m_textures->get_texture(TOOL_BAR_CONTENT[toolbar_case], 0));
		sf::Vector2f pos(m_selected_item.getPosition());
		pos.x -= m_selected_item.getGlobalBounds().width / 2;
		pos.y -= m_selected_item.getGlobalBounds().height / 2;
		m_selected_item.setPosition(pos);
		m_alpha_cd = 500;
	}
	m_current_selection = toolbar_case;
	return TOOLBAR_BLOCK_TYPES[toolbar_case];
}

BlockType InterfaceManager::scroll(bool foward)
{	
	if (foward)
	{	
		m_current_selection++;
		if (m_current_selection == 9)
			m_current_selection = 0;
	}
	else
	{
		m_current_selection--;
		if (m_current_selection == -1)
			m_current_selection = 8;
	}
	return change_selection(m_current_selection);
}

void InterfaceManager::set_cursor(sf::Vector2f pos)
{	
	if (m_alpha_cd != -1)
	{
		pos.x -= m_selected_item.getGlobalBounds().width / 2;
		pos.y -= m_selected_item.getGlobalBounds().height / 2;
	}
	m_selected_item.setPosition((sf::Vector2f)pos);
}

void InterfaceManager::draw(sf::RenderWindow& window)
{	
	if (m_alpha_cd != -1)
		window.draw(m_selected_item);
	sf::Vector2f transform_ratio((float)WIN_X / (float)window.getSize().x, (float)WIN_Y / (float)window.getSize().y);
	// Toolbar
	window.setView(window.getDefaultView());
	m_toolbar.setScale(TOOLBAR_SIZE * transform_ratio.x, TOOLBAR_SIZE * transform_ratio.y);
	m_toolbar.setPosition(sf::Vector2f((float)WIN_X / 2.f - (float)m_toolbar.getGlobalBounds().width / 2.f, (float)WIN_Y - (float)m_toolbar.getGlobalBounds().height));
	window.draw(m_toolbar);
	for (int i = 0; i < 9; i++)
	{
		m_items[i].setPosition(sf::Vector2f(m_toolbar.getPosition().x + (float)m_toolbar.getGlobalBounds().width / 9.1f * i + (3.f * transform_ratio.x * TOOLBAR_SIZE), m_toolbar.getPosition().y + (3.f * transform_ratio.y * TOOLBAR_SIZE)));
		m_items[i].setScale(0.25f * transform_ratio.x * TOOLBAR_SIZE, 0.25f * transform_ratio.y * TOOLBAR_SIZE);
		window.draw(m_items[i]);
	}
	m_selection_square.setPosition(m_toolbar.getPosition().x + (float)m_toolbar.getGlobalBounds().width / 9.1f * m_current_selection - (3.f * transform_ratio.x), m_toolbar.getPosition().y - (3.f * transform_ratio.y));
	m_selection_square.setScale(TOOLBAR_SIZE * transform_ratio.x, TOOLBAR_SIZE * transform_ratio.y);
	window.draw(m_selection_square);
	m_fps_text.setScale(transform_ratio.x, transform_ratio.y);
	window.draw(m_fps_text);
}
