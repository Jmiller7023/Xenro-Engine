#ifndef XENRO_SAVEMANAGER_DEFINED
#define XENRO_SAVEMANAGER_DEFINED

#include <string>
#include <vector>

namespace Xenro{

class SaveManager
{
public:
	SaveManager(const std::string& directory);
	~SaveManager();

	//Appends a line to the end of the current File.
	void addEntry(const std::string& entry);

	//Adds a line in at a specific Index.
	void addEntry(const std::string& entry, int index);

	//Replaces a line with the given string.
	void replaceEntry(const std::string& entry, int index);

	//Adds all vector strings into current file.
	void addFile(const std::vector<std::string>& saveFile);

	//Replaces the current file with the one provided.
	void replaceFile(const std::vector<std::string>& saveFile);

	//Deletes the save file at current directory.
	void clearSave();

	//Deletes the save file at specified directory.
	void clearSave(const std::string& filePath);

	//Saves current file to saved directory.
	void saveFiletoDir();

	//Saves current file to specified directory.
	void saveFiletoDir(const std::string& filePath);
	
private:
	std::vector<std::string> m_saveFile;
	std::string m_directory;
};

}

#endif //XENRO_SAVEMANAGER_DEFINED