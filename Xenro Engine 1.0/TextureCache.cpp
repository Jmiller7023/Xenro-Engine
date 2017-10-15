#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}

GLTexture TextureCache::getTexture(std::string path) {

	//lookup the texture and see if its in the map.
	auto mit = m_textureMap.find(path);

	//check if its not in the map.
	if (mit == m_textureMap.end()) {

		//Load the texture
		GLTexture newTexture = ImageLoader::loadPNG(path);

		//Insert it into the map
		m_textureMap.insert(make_pair(path, newTexture));
		std::cout << "Loaded Texture!\n";
		
		return newTexture;
	}
	std::cout << "Used Cached Texture!\n";

	return mit->second;
}