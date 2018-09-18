#ifndef THIEF_DEFINED
#define THIEF_DEFINED

#include "Actor.h"
#include "StatSet.h"
#include "string"

class Thief : public Actor
{
public:
	Thief();
	virtual ~Thief();

	virtual void update(const std::vector<std::string>& WorldData) override;
	virtual void update() override;

private:

	Statset m_stats;
	bool m_isAlive = true;
	std::string m_name; 

};

#endif //THIEF_DEFINED