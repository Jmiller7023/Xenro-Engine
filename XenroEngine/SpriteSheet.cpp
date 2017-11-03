/*************************************************************************/
/*                         SpriteSheet.cpp                               */
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

#include "SpriteSheet.h"

namespace Xenro{

SpriteSheet::SpriteSheet()
{

}


SpriteSheet::~SpriteSheet()
{

}

void SpriteSheet::setSpriteSheet(const GLTexture& texture, const glm::ivec2& sheetDims) {
	m_texture = texture;
	m_sheetDims = sheetDims;
}

glm::vec4 SpriteSheet::getUVcoords(int i) {
	//This will return the index of the tile needed.
	int xTile = i % m_sheetDims.x;
	int yTile = i / m_sheetDims.x;

	//Creating UV vector.
	glm::vec4 uv;
	uv.x = xTile / (float)m_sheetDims.x;
	uv.y = yTile / (float)m_sheetDims.y;
	uv.z = 1.0f / m_sheetDims.x;
	uv.w = 1.0f / m_sheetDims.y;

	return uv;
}
}