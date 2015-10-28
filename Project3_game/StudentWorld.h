#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Goodie;
class Player;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	StudentWorld* getWorld(){ return this; };
	Player* getPlayer(){ return m_player; };
	void addActor(Actor* actor);

	//check whether agent can move to coord (default: called by player unless otherwise specified)
	bool checkCoord(int x, int y, Actor* callerID);

	//check whether bullet can hit object (default: called by player unless otherwise specified)
	bool bulletHit(int x, int y);

	//check whether actor has clear path to shoot
	bool blockedShootingPath(int x, int y);

	// If a factory is at x,y, how many items of the type that should be
	// counted are in the rectangle bounded by x-distance,y-distance and
	// x+distance,y+distance?  Set count to that number and return true,
	// unless an item is on the factory itself, in which case return false
	// and don't care about count.  (The items counted are only ever going
	// KleptoBots.)
	bool doFactoryCensus(int x, int y);

	//check if there's a goodie at coordinate x, y, return pointer to it. otherwise nullptr
	Goodie* findGoodie(int x, int y);

	bool revealExit();

	//reduce jewels by 1
	void decJewels();

	void levelComplete(){ m_levelComplete = true; };

private:
	void removeDeadGameObjects();
	void updateDisplayText();
	//unsigned int getBonus() { return m_bonus; };
	string format(int score, int level, int lives, int health, int ammo, int bonus);

	list<Actor*> m_actors;
	Player* m_player;
	int m_njewels; //keep track of jewels left
	bool m_exitRevealed; //has the exit been revealed already?
	bool m_levelComplete;
	unsigned int m_bonus;
};

#endif // STUDENTWORLD_H_
