/*************************************************************************/
/*                          LoadManager.cpp                              */
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

#include "LoadManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "ErrorMessages.h"

namespace Xenro {

LoadManager::LoadManager()
{
	//Empty.
}


LoadManager::~LoadManager()
{
	m_loadData.clear();
}

void LoadManager::loadData(const std::string& filePath) {

	std::ifstream file;

	file.open(filePath);
	
	if (file.fail()) {
		fatalError("Failed to open File " + filePath);
	}

	std::string temp;
	while (std::getline(file, temp)) {
		m_loadData.push_back(temp);
	}

	file.close();
}

void LoadManager::clearLoadData() {

	m_loadData.clear();
}

bool LoadManager::fileExists(const std::string& filePath) const {

	std::ifstream file;

	file.open(filePath);

	if (file.fail()) {
		return false;
	}

	file.close();
	return true;

}

}