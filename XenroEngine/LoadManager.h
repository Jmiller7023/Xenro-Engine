#pragma once
#include <vector>
#include <string>

namespace Xenro{

class LoadManager
{
public:
	LoadManager();
	~LoadManager();

	//Loads in data from specified filePath.
	void loadData(const std::string& filePath);

	//Empties the loadData vector.
	void clearLoadData();

	//Getters.
	std::vector<std::string> getLoadData() const { return m_loadData; }

private:
	std::vector<std::string> m_loadData;
};

}