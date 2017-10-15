#pragma once

#include <string>
#include "GLTexture.h"

namespace Xenro {

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}

