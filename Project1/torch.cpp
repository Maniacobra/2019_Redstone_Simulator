#include "utils.h"
#include "behaviors.h"

void Torch::post_init(TextureResources& textures, std::string param)
{	
	m_textures = textures.get_group("torch");
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr && m_connexions[i]->is_connectable((BlockOrientation)i))
			m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL);
	m_fast_ticks = 0;
	if (param == "hooked" && m_connexions[m_rot]->is_block())
		m_hooked = true;
}

void Torch::on_destroy(void)
{
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
			m_connexions[i]->signal(this, Utils::swapside(i), 0);
}

void Torch::on_interaction_add(BlockOrientation side)
{
	m_connexions[side]->signal(this, Utils::swapside(side), REDSTONE_MAX_SIGNAL * !m_off);
	if (m_short_circuited == 0 && m_wait_update)
		m_wait_update = false;
}

void Torch::on_interaction_remove(BlockOrientation side)
{
	if (side == m_rot && m_hooked)
	{
		m_hooked = false;
		m_undrawed = true;
		if (m_off)
		{
			for (int i = 0; i < 4; i++)
				if (m_connexions[i] != nullptr)
					m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL);
			m_off = false;
		}
	}
	if (m_short_circuited == 0 && m_wait_update)
		m_wait_update = false;
}

void Torch::update(void)
{	
	if ((m_sources[m_rot] || m_short_circuited || m_wait_update) && !m_off && m_hooked)
		m_next_send = 1;
	else if ((!m_sources[m_rot] || !m_hooked) && m_off && !(m_short_circuited || m_wait_update))
		m_next_send = 2;
	if (m_short_circuited > 0)
		m_short_circuited--;
}

void Torch::post_update(void)
{	
	if (m_next_send != 0)
	{	
		if (m_next_send == 1)
		{
			for (int i = 0; i < 4; i++)
				if (m_connexions[i] != nullptr)
					m_connexions[i]->signal(this, Utils::swapside(i), 0);
			m_off = true;
			m_undrawed = true;
		}
		else
		{
			for (int i = 0; i < 4; i++)
				if (m_connexions[i] != nullptr)
					m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL);
			m_off = false;
			m_undrawed = true;
		}
		if (m_ticks_interval < 2 && m_short_circuited == 0)
		{
			m_fast_ticks++;
			if (m_fast_ticks > 14)
			{
				if (m_ticks_interval == 0)
				{
					m_short_circuited = 20;
					m_wait_update = true;
				}
				else
					m_short_circuited = 80;
			}
		}
		else if (m_fast_ticks > 0)
			m_fast_ticks = 0;
		m_ticks_interval = 0;
		m_next_send = 0;
	}
	else if (m_ticks_interval < 10)
		m_ticks_interval++;
}

void Torch::signal(BlockBehavior* block, BlockOrientation side, int level)
{	
	m_sources[side] = level > 0;
	if (m_short_circuited == 0 && m_wait_update)
		m_wait_update = false;
}

void Torch::edit(std::string param)
{
	if (m_hooked)
		m_hooked = false;
	else
	{	
		if (m_connexions[TOP]->is_block())
		{
			m_rot = TOP;
			m_hooked = true;
			m_undrawed = true;
		}
		else if (m_connexions[LEFT]->is_block())
		{
			m_rot = LEFT;
			m_hooked = true;
			m_undrawed = true;
		}
		else if (m_connexions[RIGHT]->is_block())
		{
			m_rot = RIGHT;
			m_hooked = true;
			m_undrawed = true;
		}
		else if (m_connexions[BOT]->is_block())
		{
			m_rot = BOT;
			m_hooked = true;
			m_undrawed = true;
		}
	}
}

void Torch::draw(sf::Sprite& sprite)
{
	if (!m_hooked)
		sprite.setTexture(*m_textures[0]);
	else
	{
		sprite.setTexture(*m_textures[1 + m_off]);
		sprite.setOrigin(Utils::origin(m_rot));
		sprite.setRotation(m_rot * 90.f);
	}
}

std::string Torch::get_param(void) const
{
	return m_hooked ? "hooked" : "";
}
