/*************************************************************************/
/*                           SaveManager.h                               */
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

#ifndef XENRO_SAVEMANAGER_DEFINED
#define XENRO_SAVEMANAGER_DEFINED

#include <string>
#include <vector>

namespace Xenro{

class SaveManager {
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
	void saveFiletoDir(const std::string& filePath);

	//Saves current file to specified directory.
	void saveFiletoDir(const std::string& filePath, const std::string& dir);
	
private:
	std::vector<std::string> m_saveFile;
	std::string m_directory;
};

}

#endif //XENRO_SAVEMANAGER_DEFINED