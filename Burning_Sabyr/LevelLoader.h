#ifndef LEVELLOADER_DEFINED
#define LEVELLOADER_DEFINED

#include <XenroEngine\World.h>
#include <XenroEngine\Window.h>

class LevelLoader : public Xenro::World
{
public:
	LevelLoader(Xenro::Window* window);
	~LevelLoader();

	virtual void updateGameWorld();

private:
	bool m_loaded = false;
};

#endif //LEVELLOADER_DEFINED