#include "Errors.h"
#include <string>
#include <iostream>
#include<SDL\SDL.h>
#include <cstdlib>
using namespace std;

extern void fatalError(string errorString) {
	cout << errorString << endl;
	cout << "Enter a key to continue..." << endl;
	int tmp;
	cin >> tmp;
	SDL_Quit();
	exit(1);
}