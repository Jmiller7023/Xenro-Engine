#pragma once
#include <map>
#include <string>
#include "GLTexture.h"

class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	GLTexture getTexture(std::string path);
private:
	std::map<std::string, GLTexture> m_textureMap;
};

