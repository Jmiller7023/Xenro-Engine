#ifndef XENRO_ERRORMESSAGES_DEFINED
#define XENRO_ERRORMESSAGES_DEFINED

#include <string>

namespace Xenro {

	//Prints error message to the console and closes the application.
	void fatalError(const std::string& errorMSG);

	//Prints error message to console and doesn''t close application.
	void errorMessage(const std::string& errorMSG);

}

#endif //XENRO_ERRORMESSAGES_DEFINED