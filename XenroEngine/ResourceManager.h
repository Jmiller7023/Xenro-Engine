#ifndef XENRO_RESOURCEMANAGER_DEFINED
#define XENRO_RESOURCEMANAGER_DEFINED

#include "TextureCache.h"
#include <string>

namespace Xenro {

class ResourceManager
{
public:
	static GLTexture getTexture(std::string texturePath);

private:
	static TextureCache m_texturecache;
};

}

#endif //XENRO_RESOURCEMANAGER_DEFINED