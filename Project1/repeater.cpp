#include "utils.h"
#include "behaviors.h"

void Repeater::pre_init(void)
{
	m_delay_ticks = 2;
}

void Repeater::post_init(TextureResources& textures, std::string param)
{	
	m_textures = textures.get_group("repeater");
	if (m_connexions[m_rot] != nullptr && m_alimented)
		m_connexions[m_rot]->signal(this, Utils::swapside(m_rot), REDSTONE_MAX_SIGNAL);
	if (param == "3")
		m_delay_ticks = 3;
	else if (param == "4")
		m_delay_ticks = 4;
	else if (param == "5")
		m_delay_ticks = 5;
}

void Repeater::on_destroy(void)
{
	if (m_connexions[m_rot] != nullptr)
		m_connexions[m_rot]->signal(this, Utils::swapside(m_rot), 0);
}

void Repeater::on_interaction_add(BlockOrientation side)
{
	if (side == m_rot && m_alimented)
		m_connexions[m_rot]->signal(this, Utils::swapside(side), REDSTONE_MAX_SIGNAL);
}

void Repeater::update(void)
{	
	if (m_signal_ticks_on > 0)
		m_signal_ticks_on -= 1;
	else if (m_signal_ticks_off > 0)
		m_signal_ticks_off -= 1;
}

void Repeater::post_update(void)
{	
	if (m_signal_ticks_on == 1)
	{
		m_signal_ticks_on = 0;
		if (!(m_locked_l || m_locked_r ||
			(m_connexions[Utils::rotate(true, m_rot)]->get_signal() &&
				m_connexions[Utils::rotate(true, m_rot)]->get_id() == BLOCK_REPEATER &&
				m_connexions[Utils::rotate(true, m_rot)]->get_rot() == Utils::rotate(false, m_rot)) ||
			(m_connexions[Utils::rotate(false, m_rot)]->get_signal() &&
				m_connexions[Utils::rotate(false, m_rot)]->get_id() == BLOCK_REPEATER &&
				m_connexions[Utils::rotate(false, m_rot)]->get_rot() == Utils::rotate(true, m_rot))))
		{
			m_alimented = true;
			m_undrawed = true;
			if (m_connexions[m_rot] != nullptr)
				m_connexions[m_rot]->signal(this, Utils::swapside(m_rot), REDSTONE_MAX_SIGNAL);
			if (!m_last_signal)
				m_signal_ticks_off = m_delay_ticks;
		}
	}
	else if (m_signal_ticks_off == 1)
	{
		m_signal_ticks_off = 0;
		if (!(m_locked_l || m_locked_r))
		{
			m_alimented = false;
			m_undrawed = true;
			if (m_connexions[m_rot] != nullptr)
				m_connexions[m_rot]->signal(this, Utils::swapside(m_rot), 0);
			if (m_last_signal)
				m_signal_ticks_on = m_delay_ticks;
		}
	}
}

void Repeater::signal(BlockBehavior* block, BlockOrientation side, int level)
{	
	if (side == Utils::swapside(m_rot))
	{	
		if (!(m_locked_l || m_locked_r))
		{
			if (level > 0 && !m_last_signal && m_signal_ticks_off == 0)
				m_signal_ticks_on = m_delay_ticks;
			else if (level == 0 && m_last_signal && m_signal_ticks_on == 0)
				m_signal_ticks_off = m_delay_ticks;
		}
		m_last_signal = level > 0;
	}
	else if (side != m_rot && block->get_id() == BLOCK_REPEATER)
	{
		if (level > 0)
		{
			if (side == Utils::rotate(false, m_rot))
				m_locked_l = true;
			else
				m_locked_r = true;
			m_signal_ticks_on = 0;
			m_signal_ticks_off = 0;
			m_undrawed = true;
		}
		else if (level == 0 && (m_locked_l || m_locked_r))
		{
			if (side == Utils::rotate(false, m_rot))
				m_locked_l = false;
			else
				m_locked_r = false;
			if (!(m_locked_l || m_locked_r))
			{
				if (m_last_signal)
					m_signal_ticks_on = m_delay_ticks;
				else
					m_signal_ticks_off = m_delay_ticks;
			}
			m_undrawed = true;
		}
	}
}

void Repeater::edit(std::string param)
{	
	m_delay_ticks++;
	if (m_delay_ticks == 6)
		m_delay_ticks = 2;
}

void Repeater::draw(sf::Sprite& sprite)
{
	sprite.setTexture(*m_textures[m_delay_ticks - 2 + (m_locked_l || m_locked_r) * 4 + m_alimented * 8]);
	sprite.setOrigin(Utils::origin(m_rot));
	sprite.setRotation(m_rot * 90.f);
	m_undrawed = false;
}

std::string Repeater::get_param(void) const
{
	return std::to_string(m_delay_ticks);
}

bool Repeater::get_signal(void) const
{
	return m_signal_ticks_on == 1;
}

bool Repeater::is_connectable(BlockOrientation side) const
{
	return side == m_rot || side == Utils::swapside(m_rot);
}
