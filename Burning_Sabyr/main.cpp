#include<Windows.h>

#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "MainGame.h"
#include "GameWorld.h"
#include "Application.h"
/*

int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.run();

	return 0;
}
*/

int main(int argc, char** argv) {
	Application app;
	app.run();

	return 0;
}