#ifndef XENRO_GLTEXTURE_DEFINED
#define XENRO_GLTEXTURE_DEFINED
#include<GL\glew.h>

namespace Xenro {
	struct GLTexture {
		GLuint ID;
		int height;
		int width;
	};
}

#endif //XENRO_GLTEXTURE_DEFINED