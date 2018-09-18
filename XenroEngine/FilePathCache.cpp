#include "FilePathCache.h"

namespace Xenro {

FilePathCache::FilePathCache()
{
	//Empty
}


FilePathCache::~FilePathCache()
{
	//Empty
}

void FilePathCache::createMapping(char character, std::string filePath)
{
	//lookup the texture and see if its in the map.
	auto mit = m_filePathMap.find(character);

	//check if its not in the map.
	if (mit == m_filePathMap.end()) {

		//Insert it into the map
		m_filePathMap.insert(make_pair(character, filePath));
	}
}

std::string FilePathCache::getPath(char character)
{
	//lookup the filepath and see if its in the map.
	auto mit = m_filePathMap.find(character);

	//check if its not in the map.
	if (mit == m_filePathMap.end()) {
		//not in the map. Return empty path.
		return "";
	}

	return mit->second;
}

}




