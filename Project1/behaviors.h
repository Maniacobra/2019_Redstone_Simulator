#pragma once
#ifndef BEHAVIORS_H_INCLUDED
#define BEHAVIORS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "block_enum.h"
#include "res.h"

class BlockBehavior
{
protected:

	BlockBehavior* m_connexions[4];
	TextureGroup m_textures;
	BlockOrientation m_rot;
	bool m_undrawed;
	// Methods
	virtual void pre_init(void) {};
	virtual void post_init(TextureResources& textures, std::string param) {};
	virtual void on_destroy() {};
	virtual void on_interaction_add(BlockOrientation side) {};
	virtual void on_interaction_remove(BlockOrientation side) {};

public:

	BlockBehavior(void) {};
	virtual void init(TextureResources& textures, BlockBehavior* neighbors[4], BlockOrientation rot, std::string param, bool auto_rotate);
	virtual void destroy(void);
	virtual void add_interaction(BlockBehavior* block, BlockOrientation side);
	virtual void remove_interaction(BlockBehavior* block, BlockOrientation side);
	virtual void update(void) {};
	virtual void post_update(void) {};
	virtual void signal(BlockBehavior* block, BlockOrientation side, int level) {};
	virtual void edit(std::string param) {};
	virtual void draw(sf::Sprite& sprite) {};
	virtual ~BlockBehavior(void) {};
	// Const
	virtual std::string get_param(void) const { return ""; };
	virtual BlockOrientation get_rot(void) const;
	virtual bool get_signal(void) const { return false; };
	virtual int get_id(void) const { return BLOCK_AIR; };
	virtual bool is_undrawed(void) const;
	virtual bool is_connectable(BlockOrientation side) const { return false; };
	virtual bool is_active(void) const { return false; };
	virtual bool is_ticked(void) const { return false; };
	virtual bool is_sensitive(void) const { return false; };
	virtual bool is_indirect(void) const { return false; };
	virtual bool is_block(void) const { return false; };
};

class SimpleBlock : public BlockBehavior
{	
	bool m_alimented;
	bool m_charged;
	int m_sources[4];
	// Methods
	virtual void pre_init(void);
	virtual void post_init(TextureResources& textures, std::string param);
	virtual void on_destroy(void);
	virtual void on_interaction_add(BlockOrientation side);

public:

	SimpleBlock(void) {};
	virtual void signal(BlockBehavior* block, BlockOrientation side, int level);
	virtual void draw(sf::Sprite& sprite);
	virtual ~SimpleBlock(void) {};
	// Const
	virtual int get_id(void) const { return BLOCK_IRON_BLOCK; };
	virtual bool is_indirect(void) const { return true; };
	virtual bool is_block(void) const { return true; };
};

class RedstoneWire : public BlockBehavior
{	
	int m_nb_connexions;
	int m_sources[4];
	int m_signal_level;
	// Methods
	virtual void pre_init(void);
	virtual void post_init(TextureResources& textures, std::string param);
	virtual void on_destroy(void);
	virtual void on_interaction_add(BlockOrientation side);
	void update_wire(void);

public:

	RedstoneWire(void) {};
	virtual void signal(BlockBehavior* block, BlockOrientation side, int level);
	virtual void draw(sf::Sprite& sprite);
	virtual ~RedstoneWire(void) {};
	// Const
	virtual int get_id(void) const { return BLOCK_REDSTONE_WIRE; };
	virtual bool is_connectable(BlockOrientation side) const { return true; };
	virtual bool is_active(void) const { return true; };
};

class RedstoneBlock : public BlockBehavior
{	
	// Methods
	virtual void post_init(TextureResources& textures, std::string param);
	virtual void on_destroy(void);
	virtual void on_interaction_add(BlockOrientation side);

public:

	RedstoneBlock(void) {};
	virtual void draw(sf::Sprite& sprite);
	virtual ~RedstoneBlock(void) {};
	// Const
	virtual int get_id(void) const { return BLOCK_REDSTONE_BLOCK; };
	virtual bool is_connectable(BlockOrientation side) const { return true; };
	virtual bool is_indirect(void) const { return true; };
	virtual bool is_block(void) const { return true; };
};

class Repeater : public BlockBehavior
{
	bool m_alimented;
	int m_signal_ticks_on;
	int m_signal_ticks_off;
	bool m_last_signal;
	int m_delay_ticks;
	bool m_locked_l;
	bool m_locked_r;
	// Methods
	virtual void pre_init(void);
	virtual void post_init(TextureResources& textures, std::string param);
	virtual void on_destroy(void);
	virtual void on_interaction_add(BlockOrientation side);

public:

	Repeater(void) {};
	virtual void update(void);
	virtual void post_update(void);
	virtual void signal(BlockBehavior* block, BlockOrientation side, int level);
	virtual void edit(std::string param);
	virtual void draw(sf::Sprite& sprite);
	virtual ~Repeater(void) {};
	// Const
	virtual std::string get_param(void) const;
	virtual int get_id(void) const { return BLOCK_REPEATER; };
	virtual bool get_signal(void) const;
	virtual bool is_connectable(BlockOrientation side) const;
	virtual bool is_active(void) const { return true; };
	virtual bool is_ticked(void) const { return true; };
};

class Torch : public BlockBehavior
{
	bool m_off;
	int m_next_send;
	bool m_hooked;
	bool m_sources[4];
	bool m_last_signal;
	int m_fast_ticks;
	int m_ticks_interval;
	int m_short_circuited;
	bool m_wait_update;
	// Methods
	virtual void post_init(TextureResources& textures, std::string param);
	virtual void on_destroy(void);
	virtual void on_interaction_add(BlockOrientation side);
	virtual void on_interaction_remove(BlockOrientation side);

public:

	Torch(void) {};
	virtual void update(void);
	virtual void post_update(void);
	virtual void signal(BlockBehavior* block, BlockOrientation side, int level);
	virtual void edit(std::string param);
	virtual void draw(sf::Sprite& sprite);
	virtual ~Torch(void) {};
	// Const
	virtual std::string get_param(void) const;
	virtual int get_id(void) const { return BLOCK_TORCH; };
	virtual bool is_connectable(BlockOrientation side) const { return true; };
	virtual bool is_active(void) const { return true; };
	virtual bool is_ticked(void) const { return true; };
	virtual bool is_sensitive(void) const { return true; };
	virtual bool is_indirect(void) const { return true; };
};

#endif // !BEHAVIORS_H_INCLUDED
