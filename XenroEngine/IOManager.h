#ifndef XENRO_IOMANAGER_DEFINED
#define XENRO_IOMANAGER_DEFINED

#include <vector>
#include <string>

namespace Xenro {
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
		static bool readFileToBuffer(std::string filePath, std::string& buffer);
	};
}

#endif //XENRO_IOMANAGER_DEFINED