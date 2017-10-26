#ifndef XENRO_IMAGELOADER_DEFINED
#define XENRO_IMAGELOADER_DEFINED

#include <string>
#include "GLTexture.h"

namespace Xenro {

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}

#endif //XENRO_IMAGELOADER_DEFINED