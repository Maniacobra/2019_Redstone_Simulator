#include "utils.h"
#include "behaviors.h"

void BlockBehavior::init(TextureResources& textures, BlockBehavior* neighbors[4], BlockOrientation rot, std::string param, bool auto_rotate)
{	
	pre_init();
	if (auto_rotate)
	{	
		if (neighbors[BOT]->is_connectable(TOP))
			m_rot = TOP;
		else if (neighbors[LEFT]->is_connectable(RIGHT))
			m_rot = RIGHT;
		else if (neighbors[RIGHT]->is_connectable(LEFT))
			m_rot = LEFT;
		else if (neighbors[TOP]->is_connectable(BOT))
			m_rot = BOT;
	}
	else
		m_rot = rot;
	for (int i = 0; i < 4; i++)
		if (neighbors[i] != nullptr)
		{
			m_connexions[i] = neighbors[i];
			m_connexions[i]->add_interaction(this, Utils::swapside(i));
		}
	post_init(textures, param);
}

void BlockBehavior::destroy(void)
{	
	on_destroy();
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
		{
			m_connexions[i]->remove_interaction(this, Utils::swapside(i));
			m_connexions[i] = nullptr;
		}
}

void BlockBehavior::add_interaction(BlockBehavior* block, BlockOrientation side)
{
	m_connexions[side] = block;
	on_interaction_add(side);
}

void BlockBehavior::remove_interaction(BlockBehavior* block, BlockOrientation side)
{
	if (m_connexions[side] != block)
		assert("La connexion n'a pas pu être retirée");
	on_interaction_remove(side);
	m_connexions[side] = nullptr;
}

BlockOrientation BlockBehavior::get_rot(void) const
{
	return m_rot;
}

bool BlockBehavior::is_undrawed(void) const
{
	return m_undrawed;
}
