#include "ErrorMessages.h"
#include <string>
#include <iostream>
#include<SDL\SDL.h>
#include <cstdlib>

namespace Xenro {

extern void fatalError(const std::string& errorMSG) {
	std::cout << errorMSG << std::endl;
	SDL_Quit();
	exit(1);
	}


extern void errorMessage(const std::string& errorMSG) {
	std::cout << errorMSG << std::endl;
}

}