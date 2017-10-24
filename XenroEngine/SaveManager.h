#pragma once
#include <string>
#include <vector>

namespace Xenro{

class SaveManager
{
public:
	SaveManager();
	~SaveManager();

	//Appends a line to the end of the current File.
	void addEntry(const std::string& entry);

	//Adds a line in at a specific Index.
	void addEntry(const std::string entry, int index);

	//Adds all vector strings into current file.
	void addFile(const std::vector<std::string>& saveFile);

	//Replaces the current file with the one provided.
	void replaceFile(const std::vector<std::string>& saveFile);

	//Deletes the save file at specified filePath.
	void clearSave(const std::string& filePath);

	//Saves current file to specified directory.
	void saveFiletoDir(const std::string& filePath);
	
private:
	std::vector<std::string> m_saveFile;
};

}