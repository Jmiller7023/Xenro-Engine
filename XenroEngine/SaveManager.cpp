#include "SaveManager.h"
#include <iostream>
#include <fstream>
#include "ErrorMessages.h"

namespace Xenro{

SaveManager::SaveManager()
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

void SaveManager::addEntry(const std::string entry, int index) {
	if (index > m_saveFile.size() || index < 0) {
		errorMessage("index is outside of vector range!");
		return;
	}
	auto it = m_saveFile.begin();
	for (int i = 0; i < index; i++) {
		it++;
	}
	m_saveFile.insert(it, entry);
}

void SaveManager::addFile(const std::vector<std::string>& saveFile) {
	
	for (size_t i = 0; i < saveFile.size(); i++) {
		m_saveFile.push_back(saveFile[i]);
	}
}

void SaveManager::replaceFile(const std::vector<std::string>& saveFile) {

	m_saveFile = saveFile;
}

void SaveManager::clearSave(const std::string& filePath) {

	std::remove(filePath.c_str());
}

void SaveManager::saveFiletoDir(const std::string& filePath) {

	std::ofstream saveFile;

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