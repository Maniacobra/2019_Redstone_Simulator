#include "utils.h"
#include "behaviors.h"

void RedstoneBlock::post_init(TextureResources& textures, std::string param)
{	
	m_textures = textures.get_group("redstone_block");
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
			m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL);
}

void RedstoneBlock::on_destroy(void)
{
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
			m_connexions[i]->signal(this, Utils::swapside(i), 0);
}

void RedstoneBlock::on_interaction_add(BlockOrientation side)
{
	m_connexions[side]->signal(this, Utils::swapside(side), REDSTONE_MAX_SIGNAL);
}

void RedstoneBlock::draw(sf::Sprite& sprite)
{
	sprite.setTexture(*m_textures[0]);
}
