/*************************************************************************/
/*                            IOManager.h                                */
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

#ifndef XENRO_IOMANAGER_DEFINED
#define XENRO_IOMANAGER_DEFINED

#include <vector>
#include <string>

namespace Xenro {

struct DirEntry {
	std::string path;
	bool isDirectory;
};

class IOManager {
public:
	static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	static bool readFileToBuffer(std::string filePath, std::string& buffer);

	// Gets all directory entries in the directory specified by path and stores in rvEntries.
	// Returns false if path is not a directory.
	static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);
	static bool makeDirectory(const char* path);
};

}

#endif //XENRO_IOMANAGER_DEFINED