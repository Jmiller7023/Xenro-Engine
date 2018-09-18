#ifndef STATSET_DEFINED
#define STATSET_DEFINED

struct Statset {

	Statset() {}
	Statset(int HP, int MoveNum, int Dext, int Strength, int Wisdom, int Intelligence, int Level, int EXP);

	int hp = 0;
	int moveNum = 0;
	int dext = 0;
	int strength = 0;
	int wisdom = 0;
	int intelligence = 0;
	int level = 0;
	int exp = 0;
};
#endif //STATSET_DEFINED