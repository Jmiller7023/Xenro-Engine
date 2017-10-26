#ifndef XENRO_TEXTURECACHE_DEFINED
#define XENRO_TEXTURECACHE_DEFINED
#include <map>
#include <string>
#include "GLTexture.h"

namespace Xenro {
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string path);
	private:
		std::map<std::string, GLTexture> m_textureMap;
	};

}

#endif //XENRO_TEXTURECACHE_DEFINED