#include "ErrorMessages.h"
#include <string>
#include <iostream>
#include<SDL\SDL.h>
#include <cstdlib>

namespace Xenro {

extern void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	SDL_Quit();
	exit(1);
	}


extern void error(std::string errorString) {
	std::cout << errorString << std::endl;
}

}