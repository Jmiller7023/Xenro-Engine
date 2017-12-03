/*************************************************************************/
/*                         SaveManager.cpp                               */
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

#include "SaveManager.h"
#include <iostream>
#include <fstream>
#include "ErrorMessages.h"
#include "IOManager.h"

namespace Xenro{

SaveManager::SaveManager(const std::string& directory)
	:m_directory(directory)
{
	//Empty
}


SaveManager::~SaveManager()
{
	m_saveFile.clear();
}

void SaveManager::addEntry(const std::string& entry) {
	
	m_saveFile.push_back(entry);
}

void SaveManager::addEntry(const std::string& entry, int index) {


	//Error checking.
	if (index > m_saveFile.size() || index < 0) {
		warning("index is outside of vector range!");
		return;
	}

	auto it = m_saveFile.begin();
	for (int i = 0; i < index; i++) {
		it++;
	}
	
	m_saveFile.insert(it, entry);
}

void SaveManager::replaceEntry(const std::string& entry, int index) {

	//Error checking.
	if (index > m_saveFile.size() || index < 0) {
		warning("index is outside of vector range!");
		return;
	}

	m_saveFile[index] = entry;
}

void SaveManager::addFile(const std::vector<std::string>& saveFile) {
	
	for (size_t i = 0; i < saveFile.size(); i++) {
		m_saveFile.push_back(saveFile[i]);
	}
}

void SaveManager::replaceFile(const std::vector<std::string>& saveFile) {

	m_saveFile = saveFile;
}

void SaveManager::clearSave() {

	std::remove(m_directory.c_str());
}

void SaveManager::clearSave(const std::string& filePath) {

	std::remove(filePath.c_str());
}

void SaveManager::saveFiletoDir(const std::string& filePath) {

	std::ofstream saveFile;

	IOManager::makeDirectory(m_directory.c_str());

	saveFile.open(filePath);

	if (saveFile.fail()) {
		fatalError("Failed to create saveFile " + filePath);
	}

	for (size_t i = 0; i < m_saveFile.size(); i++) {
		saveFile << m_saveFile[i] + "\n";
	}

	saveFile.close();
}

void SaveManager::saveFiletoDir(const std::string& filePath, const std::string& dir) {

	std::ofstream saveFile;

	//Make sure the directory exists.
	IOManager::makeDirectory(dir.c_str());

	saveFile.open(filePath);

	if (saveFile.fail()) {
		fatalError("Failed to create saveFile " + filePath);
	}

	for (size_t i = 0; i < m_saveFile.size(); i++) {
		saveFile << m_saveFile[i] + "\n";
	}

	saveFile.close();
}

}