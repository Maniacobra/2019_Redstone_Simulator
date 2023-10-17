#include "utils.h"
#include "behaviors.h"

void RedstoneWire::update_wire(void)
{
	unsigned char connexion =
		(m_connexions[TOP] != nullptr && m_connexions[TOP]->is_connectable(BOT)) +
		(m_connexions[RIGHT] != nullptr && m_connexions[RIGHT]->is_connectable(LEFT)) * 2 +
		(m_connexions[BOT] != nullptr && m_connexions[BOT]->is_connectable(TOP)) * 4 +
		(m_connexions[LEFT] != nullptr && m_connexions[LEFT]->is_connectable(RIGHT)) * 8;
	switch (connexion)
	{
	case 0x1: case 0x4: case 0x5:
		m_rot = TOP;
		m_nb_connexions = 1;
		break;
	case 0x2: case 0x8: case 0xA:
		m_rot = RIGHT;
		m_nb_connexions = 1;
		break;
	case 0x3:
		m_rot = TOP;
		m_nb_connexions = 2;
		break;
	case 0x6:
		m_rot = RIGHT;
		m_nb_connexions = 2;
		break;
	case 0xC:
		m_rot = BOT;
		m_nb_connexions = 2;
		break;
	case 0x9:
		m_rot = LEFT;
		m_nb_connexions = 2;
		break;
	case 0xB:
		m_rot = TOP;
		m_nb_connexions = 3;
		break;
	case 0x7:
		m_rot = RIGHT;
		m_nb_connexions = 3;
		break;
	case 0xE:
		m_rot = BOT;
		m_nb_connexions = 3;
		break;
	case 0xD:
		m_rot = LEFT;
		m_nb_connexions = 3;
		break;
	case 0xF:
		m_rot = TOP;
		m_nb_connexions = 4;
		break;
	default:
		m_rot = TOP;
		m_nb_connexions = 0;
	}
	m_undrawed = true;
}

void RedstoneWire::pre_init(void)
{	
	m_nb_connexions = 0;
	m_sources[0] = 0;
	m_sources[1] = 0;
	m_sources[2] = 0;
	m_sources[3] = 0;
	m_signal_level = 0;
}

void RedstoneWire::post_init(TextureResources& textures, std::string param)
{	
	m_textures = textures.get_group("redstone");
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr && (m_connexions[i]->is_connectable(Utils::swapside(i)) || (m_nb_connexions == 1 && (i == m_rot || i == Utils::swapside(m_rot)))))
			m_connexions[i]->signal(this, Utils::swapside(i), m_signal_level > 0 ? m_signal_level - 1 : 0);
	update_wire();
}

void RedstoneWire::on_destroy(void)
{
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
			m_connexions[i]->signal(this, Utils::swapside(i), 0);
}

void RedstoneWire::on_interaction_add(BlockOrientation side)
{
	update_wire();
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
		{
			if (m_connexions[i]->is_connectable(Utils::swapside(i)) || (m_nb_connexions == 1 && (i == m_rot || i == Utils::swapside(m_rot))))
				m_connexions[i]->signal(this, Utils::swapside(i), m_signal_level > 0 ? m_signal_level - 1 : 0);
			else
				m_connexions[i]->signal(this, Utils::swapside(i), 0);
		}
}

void RedstoneWire::signal(BlockBehavior* block, BlockOrientation side, int level)
{	
	if (m_sources[side] == level)
		return;
	int prev_signal = m_signal_level;
	m_sources[side] = level;
	if (m_signal_level < level)
		m_signal_level = level;
	else
	{	
		m_signal_level = 0;
		for (int i = 0; i < 4; i++)
			if (m_sources[i] > m_signal_level)
				m_signal_level = m_sources[i];
	}
	// Send
	if (prev_signal != m_signal_level)
		for (int i = 0; i < 4; i++)
			if (m_connexions[i] != nullptr && (m_connexions[i]->is_connectable(Utils::swapside(i)) || (m_nb_connexions == 1 && (i == m_rot || i == Utils::swapside(m_rot)))))
				m_connexions[i]->signal(this, Utils::swapside(i), m_signal_level > 0 ? m_signal_level - 1 : 0);
	update_wire();
}

void RedstoneWire::draw(sf::Sprite& sprite)
{
	sprite.setTexture(*m_textures[m_nb_connexions]);
	sprite.setColor(sf::Color(m_signal_level * (205 / REDSTONE_MAX_SIGNAL) + 50, 0, 0, 255));
	sprite.setOrigin(Utils::origin(m_rot));
	sprite.setRotation((float)(m_rot * 90));
	m_undrawed = false;
}
