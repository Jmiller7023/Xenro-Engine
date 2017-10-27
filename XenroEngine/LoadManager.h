#ifndef XENRO_LOADMANAGER_DEFINED
#define XENRO_LOADMANAGER_DEFINED

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

	//Returns wheter or not the file successfully opened.
	bool fileExists(const std::string& filePath) const;

	//Getters.
	std::vector<std::string> getLoadData() const { return m_loadData; }

private:
	std::vector<std::string> m_loadData;
};

}

#endif //XENRO_LOADMANAGER_DEFINED