#include <sstream>
#include "utils.h"
#include "res.h"

TextureResources::TextureResources(void)
{	
	std::string file_name;
	std::string key_name;
	bool key = false;
	std::istringstream iss_b(TEXTURE_FILES);
	char r_char;
	int texture_nb;
	sf::Texture loaded_texture;
	while (iss_b >> r_char)
	{	
		if (r_char == '.')
		{	
			iss_b >> texture_nb;
			for (int i = 0; i < texture_nb; i++)
			{	
				if (!loaded_texture.loadFromFile(TEXTURE_FOLDER + file_name + (texture_nb > 1 ? "_" + std::to_string(i) : "") + ".png"))
					logw("Impossible d'ouvrir le fichier " + file_name + " (" + std::to_string(texture_nb) + ") !\n");
				else
					m_textures[key_name].push_back(loaded_texture);
			}
			file_name.clear();
			key_name.clear();
			key = false;
		}
		else
		{	
			if (r_char == '/')
				key = true;
			else if (key)
				key_name += r_char;
			file_name += r_char;
		}
	}
	// Default
	sf::Image default_img;
	default_img.create(16, 16);
	for (int iy = 0; iy < 16; iy++)
		for (int ix = 0; ix < 16; ix++)
			default_img.setPixel(ix, iy, ix % 2 == 0 != iy % 2 ? sf::Color::Black : sf::Color::White);
	for (int ix = 0; ix < 16; ix++)
	{
		default_img.setPixel(ix, 0, sf::Color::White);
		default_img.setPixel(ix, 1, sf::Color::Black);
		default_img.setPixel(ix, 2, sf::Color::White);
		default_img.setPixel(ix, 3, sf::Color::Black);
	}
	m_default_texture.loadFromImage(default_img);
	// Info
	logw("Loaded data :\n");
	for (auto it(m_textures.begin()); it != m_textures.end(); ++it)
		logw("- '" + it->first + "' : " + std::to_string(it->second.size()));
	// Fonts
	sf::Font new_font;
	new_font.loadFromFile(DEFAULT_FONT_FILE);
	m_fonts["default"] = new_font;
}

TextureGroup TextureResources::get_group(std::string file_name)
{	
	if (m_textures.find(file_name) != m_textures.end())
	{	
		return TextureGroup(&m_default_texture, m_textures[file_name]);
	}
	return TextureGroup(&m_default_texture);
}

sf::Font& TextureResources::get_font(std::string file_name)
{
	return m_fonts[file_name];
}

sf::Texture& TextureResources::get_texture(std::string file_name, unsigned int nb)
{	
	if (m_textures.find(file_name) != m_textures.end() && m_textures[file_name].size() > nb)
		return m_textures[file_name][nb];
	return m_default_texture;
}
