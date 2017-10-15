#include "ResourceManager.h"

TextureCache ResourceManager::m_texturecache;
 
GLTexture ResourceManager::getTexture(std::string texturePath) {
	return m_texturecache.getTexture(texturePath);
}