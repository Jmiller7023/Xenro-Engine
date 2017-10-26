#ifndef XENRO_PICOPNG_DEFINED
#define XENRO_PICOPNG_DEFINED

#include <vector>

int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

#endif //XENRO_PICOPNG_DEFINED
