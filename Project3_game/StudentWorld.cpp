#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir) {}

StudentWorld::~StudentWorld()
{
	cleanUp();
};

/*
A. Initialize the data structures used to keep track of your game’s world.
B.Load the current maze details from a level data file.
C. Allocate and insert a valid Player object into the game world.
D. Allocate and insert any SnarlBots objects, Wall objects, Boulder objects, Factory
objects, Jewel objects, Goodie objects, or Exit objects into the game world, as
required by the specification in the current level’s data file.
*/
int StudentWorld::init()
{
	//initial conditions set
	m_njewels = 0;
	m_exitRevealed = false;
	m_levelComplete = false;
	m_bonus = 1000;

	//add level actors
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << getLevel() << ".dat";
	string	curLevel = oss.str();
	Level lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(curLevel);
	if (result == Level::load_fail_file_not_found)
	{
		cerr << "Could not find" << curLevel << "data file\n";
		return GWSTATUS_PLAYER_WON;
	}
	else if (result == Level::load_fail_bad_format)
	{
		cerr << "Your level was improperly formatted\n";
		return  GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level\n";
		for (int x = 0; x < VIEW_WIDTH; x++)
		{
			for (int y = 0; y < VIEW_HEIGHT; y++)
			{
				Level::MazeEntry item = lev.getContentsOf(x, y);
				switch (item)
				{
					case Level::player:
					{
						Player* ptr = new Player(x, y, this);
						m_actors.push_back(ptr);
						m_player = ptr;
						break;
					}
					case Level::wall:
						m_actors.push_back(new Wall(x, y, this));
						break;
					case Level::boulder:
						m_actors.push_back(new Boulder(x, y, this));
						break;
					case Level::hole:
						m_actors.push_back(new Hole(x, y, this));
						break;
					case Level::jewel:
						m_actors.push_back(new Jewel(x, y, this));
						m_njewels++;
						break;
					case Level::exit:
						m_actors.push_back(new Exit(x, y, this));
						break;
					case Level::extra_life:
						m_actors.push_back(new Extra_Life(x, y, this));
						break;
					case Level::restore_health:
						m_actors.push_back(new Restore_Health(x, y, this));
						break;
					case Level::ammo:
						m_actors.push_back(new Ammo(x, y, this));
						break;
					case Level::horiz_snarlbot:
						m_actors.push_back(new Horiz_Snarlbot(x, y, this));
						break;
					case Level::vert_snarlbot:
						m_actors.push_back(new Vert_Snarlbot(x, y, this));
						break;
					case Level::kleptobot_factory:
						m_actors.push_back(new Kleptobot_Factory(Kleptobot_Factory::REGULAR, x, y, this));
						break;
					case Level::angry_kleptobot_factory:
						m_actors.push_back(new Kleptobot_Factory(Kleptobot_Factory::ANGRY, x, y, this));
						break;
				}
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	updateDisplayText(); // update the score/lives/level text at screen top

	// Give each actor a chance to do something
	for (list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++)
	{
		if ((*p)->alive())
			(*p)->doSomething();
		if (!m_player->alive()) //player died during this tick
		{ 
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
			
		if (m_levelComplete)
		{
			increaseScore(2000 + m_bonus);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}
	// Remove newly-dead actors after each tick
	removeDeadGameObjects(); // delete dead game objects

	// Reduce the current bonus for the Level by one
	if (m_bonus > 0)
		m_bonus--;

	// the player hasn’t completed the current level and hasn’t died, so
	// continue playing the current level
	return GWSTATUS_CONTINUE_GAME;
}

//at end of each level, clean up
void StudentWorld::cleanUp()
{
	for (list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end();)
	{
		delete *p;
		p = m_actors.erase(p);
	}
}

void StudentWorld::addActor(Actor* actor)
{
	m_actors.push_front(actor);
}

bool StudentWorld::checkCoord(int x, int y, Actor* callerID)
{
	bool found = false;
	for (list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++)
		if ((*p)->getX() == x && (*p)->getY() == y)
		{
			if ((*p)->blockCoord(m_player->getDirection(), callerID))
				found = true; //found something on coordinate that blocks this particular actor
		}
	return found; //return after searching through all actors
};

//check whether bullet will hit something
bool StudentWorld::bulletHit(int x, int y)
{
	bool damage = false;
	for (list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++)
		if ((*p)->getX() == x && (*p)->getY() == y)
		{
			if ((*p)->attackable()) //was an object hit? (certain objects can't be hit)
					damage = true;
		}
	return damage;
}

bool StudentWorld::blockedShootingPath(int x, int y)
{
	bool block = false;
	for (list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++)
		if ((*p)->getX() == x && (*p)->getY() == y)
		{
			if ((*p)->blockRobotAttack()) //was an object hit? (certain objects can't be attacked by robots)
				block = true;
		}
	return block;
}

bool StudentWorld::doFactoryCensus(int x, int y)
{
	int count = 0;
	for (list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++)
	{
		int actor_x = (*p)->getX();
		int actor_y = (*p)->getY();
		//check if kleptobot on factory
		if (actor_x == x && actor_y == y && (*p)->countsInFactoryCensus())
			return false;  //kleptobot found on factory, don't add more kleptobots

		int dist_x = x - actor_x;
		int dist_y = y - actor_y;
		//check if object within range of factory
		if (dist_x >= -3 && dist_x <= 3 && dist_y >= -3 && dist_y <= 3)
			if ((*p)->countsInFactoryCensus())
				count++;
	}
	if (count < 3)
		return true; //true, factory should produce new kleptobot
	return false;
}

Goodie* StudentWorld::findGoodie(int x, int y)
{
	Goodie* item = nullptr;
	for (list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++)
		//object must match coords and be visible to be picked up (otherwise already claimed)
		if ((*p)->getX() == x && (*p)->getY() == y && (*p)->isVisible())
		{
			item = dynamic_cast<Goodie*>(&**p);
			if (item != nullptr) //pickup-able item found on coordinate
				break;
		}
	return item;
}

bool StudentWorld::revealExit()
{
	if (!m_exitRevealed && m_njewels <= 0) //if exit not already revealed and all jewels collected
	{
		m_exitRevealed = true;
		return true;
	}
	return false;
}

void StudentWorld::decJewels()
{
	m_njewels--;
}

void StudentWorld::removeDeadGameObjects()
{
	for (list<Actor*>::iterator p = m_actors.begin(); p != m_actors.end();)
	{
		if (!(*p)->alive())
		{
			delete *p;
			p = m_actors.erase(p);
		}
		else
			p++;
	}
}

string StudentWorld::format(int score, int level, int lives, int health, int ammo, int bonus)
{
	ostringstream oss;
	oss.fill('0');
	oss << "Score: " << setw(7) << score
		<< "  Level: " << setw(2) << level;
	oss.fill(' ');
	oss << "  Lives: " << setw(2) << lives
		<< "  Health: " << setw(3) << health << "%"
		<< "  Ammo: " << setw(3) << ammo
		<< "  Bonus: " << setw(4) << bonus;
	return oss.str();
}

void StudentWorld::updateDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = m_player->checkHP();
	double healthPercent = (health / 20.0) * 100;
	int ammo = m_player->checkAmmo();
	// Next, create a string from your statistics, of the form:
	string s = format(score, level, lives, healthPercent, ammo, m_bonus);
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls GameWorld::setGameStatText
}
