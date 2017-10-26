#ifndef LEVELLOADER_DEFINED
#define LEVELLOADER_DEFINED

#include <XenroEngine\World.h>
class LevelLoader : public Xenro::World
{
public:
	LevelLoader();
	~LevelLoader();

	virtual void updateGameWorld();

private:
	bool m_loaded = false;
};

#endif //LEVELLOADER_DEFINED