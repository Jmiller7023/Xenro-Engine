/*************************************************************************/
/*                          ImageLoader.cpp                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  11-3-2017 Joseph Miller.                   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#define _CRT_SECURE_NO_DEPRECATE
#include "ImageLoader.h"
#include "IOManager.h"
#include "ErrorMessages.h"
#include <vector>
#include <libpng\png.h>

namespace Xenro {

GLTexture ImageLoader::loadPNG(std::string filePath) {
	//Generate GLTexture and set its fields to 0.
	GLTexture texture = {};
	int width, height;
	GLubyte textureID = load_texture(filePath.c_str(), &width, &height);

	//Assign values to texture.
	texture.ID = textureID;
	texture.width = width;
	texture.height = height;
	texture.filePath = filePath;

	//return a copy of the texture data.
	return texture;
	}

GLuint ImageLoader::load_texture(const char * file_name, int * width, int * height)
{
	FILE *fp = fopen(file_name, "rb");
	if (fp == 0)
	{
		perror(file_name);
		return 0;
	}

	//Check if PNG signature is in file header
	png_byte sig[8];
	if (fread(sig, 1, sizeof(sig), fp) < 8) {
		fclose(fp);
		std::string errormsg = "Invalid signature for PNG file";
		errormsg += file_name;
		errormsg += "\n";
		fatalError(errormsg);
	}
	if (png_sig_cmp(sig, 0, 8))
	{
		std::string errormsg = "error: ";
		errormsg += file_name;
		errormsg += " is not a PNG.\n";
		fclose(fp);
		fatalError(errormsg);
	}

	//Create png_struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(fp);
		fatalError("error: png_create_read_struct returned 0.\n");
	}

	//Create png_info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		fatalError("error: png_create_info_struct returned 0.\n");
	}

	//Create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		fatalError("error: png_create_info_struct returned 0.\n");
	}

	//The code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		fatalError("error from libpng\n");
	}

	//Init png reading
	png_init_io(png_ptr, fp);

	//Let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	//Read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//Variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;

	//Get info about png
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
		NULL, NULL, NULL);

	*width = temp_width; 
	*height = temp_height; 

	if (bit_depth != 8)
	{
		std::string errormsg = file_name;
		errormsg += ": Unsupported bit depth " + std::to_string(bit_depth) + ". Must be 8.\n";
		fatalError(errormsg);
	}

	//Determine Format
	GLint format;
	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		format = GL_LUMINANCE;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		format = GL_LUMINANCE_ALPHA;
		break;
	case PNG_COLOR_TYPE_RGB:
		format = GL_RGB;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		format = GL_RGBA;
		break;
	case PNG_COLOR_TYPE_PALETTE:
		png_set_expand(png_ptr);
		format = GL_RGBA;
		break;
	default:
		std::string errormsg = file_name;
		errormsg += ": Unknown libpng color type " + std::to_string(color_type) + "\n";
		fatalError(errormsg);
	}

	//Prevent interlaced data
	png_set_interlace_handling(png_ptr);

	//Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	//Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	//glTexImage2d requires rows to be 4-byte aligned
	rowbytes += 3 - ((rowbytes - 1) % 4);

	//Allocate the image_data as a big block, to be given to opengl
	png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte) + 15);
	if (image_data == NULL)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		fatalError("error: could not allocate memory for PNG image data\n");
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
	if (row_pointers == NULL)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		fclose(fp);
		fatalError("error: could not allocate memory for PNG row pointers\n");
	}

	//Set the individual row_pointers to point at the correct offsets of image_data
	for (unsigned int i = 0; i < temp_height; i++)
	{
		row_pointers[i] = image_data + i * rowbytes;
	}

	//Read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//Generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Clean up
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(image_data);
	free(row_pointers);
	fclose(fp);
	return texture;
}

}