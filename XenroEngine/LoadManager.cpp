#include "LoadManager.h"
#include <iostream>
#include <fstream>
#include "ErrorMessages.h"

namespace Xenro{

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
	while (file >> temp) {
		m_loadData.push_back(temp);
	}

	file.close();
}

void LoadManager::clearLoadData() {

	m_loadData.clear();
}

}