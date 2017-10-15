#pragma once
#include <vector>
class Human;
class Zombie
{
public:
	Zombie();
	~Zombie();

	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombie);
};

