#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <cstdlib>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir, int hp)
	: GraphObject(imageID, startX, startY, dir), m_alive(true), m_world(ptr), m_hp(hp)
{
	setVisible(true);
};

void Actor::nextCoord(int& x, int& y, Direction& d)
{
	x = getX();
	y = getY();
	switch (d)
	{
	case left:
		x--;
		break;
	case right:
		x++;
		break;
	case up:
		y++;
		break;
	case down:
		y--;
		break;
	}
}

void Actor::fire()
{
	int x, y;
	Direction dir = getDirection();
	nextCoord(x, y, dir);
	Actor* ptr = new Bullet(x, y, getWorld(), dir);
	getWorld()->addActor(ptr);
}

Player::Player(int startX, int startY, StudentWorld* ptr)
	:Actor(IID_PLAYER, startX, startY, ptr, right, 20), m_ammo(20){};

void Player::doSomething()
{
	if (!alive())
		return;
	//if the user pressed a key
	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key this tick!
		switch (ch)
		{
			case KEY_PRESS_ESCAPE:
				setDead();
				break;
			case KEY_PRESS_SPACE:
				if (m_ammo > 0)
				{
					m_ammo--;
					fire();
					getWorld()->playSound(SOUND_PLAYER_FIRE);
				}
				break;
			case KEY_PRESS_LEFT:
				setDirection(left);
				if (!getWorld()->checkCoord(getX() - 1, getY(), this) )
					moveTo(getX() - 1, getY());
				break;
			case KEY_PRESS_RIGHT:
				setDirection(right);
				if (!getWorld()->checkCoord(getX() + 1, getY(), this) )
					moveTo(getX() + 1, getY());
				break;
			case KEY_PRESS_UP:
				setDirection(up);
				if (!getWorld()->checkCoord(getX(), getY() + 1, this) )
					moveTo(getX(), getY() + 1);
				break;
			case KEY_PRESS_DOWN:
				setDirection(down);
				if (!getWorld()->checkCoord(getX(), getY() - 1, this) )
					moveTo(getX(), getY() - 1);
		}
	}
}

bool Player::attackable()
{
	decHP(2);
	if (checkHP() > 0)
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
	else
	{
		setDead();
		getWorld()->playSound(SOUND_PLAYER_DIE);
	}
	return true;
}

Wall::Wall(int startX, int startY, StudentWorld* ptr) 
	:Actor(IID_WALL, startX, startY, ptr){};

Bullet::Bullet(int startX, int startY, StudentWorld* ptr, Direction dir)
	:Actor(IID_BULLET, startX, startY, ptr, dir) {};

void Bullet::doSomething()
{
	if (!alive())
		return;
	if (getWorld()->bulletHit(getX(), getY())) //an object that could be attacked was found on the coordinate
	{
		setDead();
		return;
	}
	int x, y;
	Direction dir=getDirection();
	nextCoord(x, y, dir);
	moveTo(x, y);

	if (getWorld()->bulletHit(getX(), getY())) //an object that could be attacked was found on the coordinate
		setDead();
}

Boulder::Boulder(int startX, int startY, StudentWorld* ptr)
	:Actor(IID_BOULDER, startX, startY, ptr, none, 10) {};

bool Boulder::attackable()
{
	decHP(2);
	if (checkHP() <= 0)
		setDead();
	return true; //boulders are attackable
}

bool Boulder::blockCoord(Direction dir, Actor* callerID)
{
	Hole* hole = dynamic_cast<Hole*>(callerID);
	Player* player = dynamic_cast<Player*>(callerID);
	if (hole != nullptr) //if hole called function, then boulder must be on a hole
		setDead();
	if (player != nullptr && push(dir)) //check if object trying to push is a player, only players can push boulders
		return false; //push succeeded, not blocked
	return true; //boulders block everything else beside holes and players
}

bool Boulder::push(Direction dir)
{
	int x, y;
	nextCoord(x, y, dir); //calculate next coordinate
	if (!getWorld()->checkCoord(x, y, this)) //check if something is blocking
	{
		moveTo(x, y);
		return true;
	}
	return false;
}

Hole::Hole(int startX, int startY, StudentWorld* ptr)
	:Actor(IID_HOLE, startX, startY, ptr) {};

void Hole::doSomething()
{
	if (!alive())
		return;
	if (getWorld()->checkCoord(getX(), getY(), this)) //found something on the hole
		setDead();
}

bool Hole::blockCoord(Direction dir, Actor* callerID)
{
	Boulder* boulder = dynamic_cast<Boulder*>(callerID);
	Hole* hole = dynamic_cast<Hole*>(callerID);
	if (boulder != nullptr || hole != nullptr) //holes don't block boulders or themselves
		return false;
	return true;
}

Exit::Exit(int startX, int startY, StudentWorld* ptr)
	:Actor(IID_EXIT, startX, startY, ptr, none) 
{
	setVisible(false);
}

void Exit::doSomething()
{
	Actor* player = getWorld()->getPlayer();
	if (player->getX() == getX() && player->getY() == getY() && isVisible()) //player on same square as visible exit
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->levelComplete();
	}

	//check if all jewels collected
	if (getWorld()->revealExit())
	{
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
	}
}

Item::Item(int imageID, int startX, int startY, StudentWorld* ptr)
	:Actor(imageID, startX, startY, ptr){}

void Item::doSomething()
{
	if (!alive())
		return;
	Actor* player = getWorld()->getPlayer();
	if (player->getX() == getX() && player->getY() == getY() && isVisible()) //player on same square, collect item
	{
		collectItem();
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

bool Item::blockCoord(Direction dir, Actor* callerID)
{
	Player* player = dynamic_cast<Player*>(callerID);
	Robot* robot = dynamic_cast<Robot*>(callerID);
	if (player != nullptr || robot != nullptr) //items don't block player or robots
		return false;
	return true;
}

Jewel::Jewel(int startX, int startY, StudentWorld* ptr)
	:Item(IID_JEWEL, startX, startY, ptr) {}

void Jewel::collectItem()
{
	getWorld()->increaseScore(50);
	getWorld()->decJewels();
}

Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* ptr)
	:Item(imageID, startX, startY, ptr) {}

void Goodie::dropItem(int x, int y)
{
	moveTo(x, y);
	setVisible(true);
}

Extra_Life::Extra_Life(int startX, int startY, StudentWorld* ptr)
	:Goodie(IID_EXTRA_LIFE, startX, startY, ptr) {}

void Extra_Life::collectItem()
{
	getWorld()->increaseScore(1000);
	getWorld()->incLives();
}

Restore_Health::Restore_Health(int startX, int startY, StudentWorld* ptr)
	:Goodie(IID_RESTORE_HEALTH, startX, startY, ptr) {}

void Restore_Health::collectItem()
{
	Actor* player = getWorld()->getPlayer();
	getWorld()->increaseScore(500);
	player->restoreHP();
}

Ammo::Ammo(int startX, int startY, StudentWorld* ptr)
	:Goodie(IID_AMMO, startX, startY, ptr)
{}

void Ammo::collectItem()
{
	Player* player = getWorld()->getPlayer();
	getWorld()->increaseScore(100);
	player->addAmmo();
}

Robot::Robot(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir, int hp)
	:Actor(imageID, startX, startY, ptr, dir, hp), m_tick(1)
{
	m_moveFrequency = (28 - getWorld()->getLevel()) / 4; // calculate number of ticks between moves
	// level number (0, 1, 2, etc.)
	if (m_moveFrequency < 3)
		m_moveFrequency = 3; // no robot moves more frequently than this
}

void Robot::reverseDirection()
{
	Direction dir = getDirection();
	switch (dir)
	{
	case left:
		setDirection(right);
		break;
	case right:
		setDirection(left);
		break;
	case up:
		setDirection(down);
		break;
	case down:
		setDirection(up);
		break;
	}
}

//determine whether robot should act during this tick
bool Robot::act()
{
	if (!alive())
		return false;
	if (m_tick != m_moveFrequency)
	{
		m_tick++;
		return false;
	}
	return true;
}

bool Robot::attemptMove(Direction dir)
{
	int x, y;
	nextCoord(x, y, dir);
	if (!getWorld()->checkCoord(x, y, this)) //check if something is blocking
	{
		moveTo(x, y);
		return true;
	}
	return false;
}

void Robot::doSomething()
{
	if (!act())
		return; //don't do anything during this tick

	m_tick = 1; //reset m_tick

	//try to shoot
	if (decideShoot()) //returned true, ok to shoot
	{
		fire();
		getWorld()->playSound(SOUND_ENEMY_FIRE);
	}

	//try to move
	else if (!attemptMove(getDirection())) //unable to move
			reverseDirection();
	
}

bool Robot::attackable()
{
	decHP(2);
	if (checkHP() > 0)
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	else
	{
		setDead();
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(pointsWorth());
	}
	return true;
}

Horiz_Snarlbot::Horiz_Snarlbot(int startX, int startY, StudentWorld* ptr)
	: Robot(IID_SNARLBOT, startX, startY, ptr, right, 10) {}

Vert_Snarlbot::Vert_Snarlbot(int startX, int startY, StudentWorld* ptr)
	: Robot(IID_SNARLBOT, startX, startY, ptr, down, 10) {}

bool Robot::decideShoot()
{
	int player_x = getWorld()->getPlayer()->getX();
	int player_y = getWorld()->getPlayer()->getY();
	int robot_x = getX();
	int robot_y = getY();
	int dist;
	bool okShoot = true;

	if (player_y == robot_y) //player in same row
	{
		dist = player_x - robot_x;
		if ((dist > 0 && getDirection() == right)) //robot is facing player
		{
			for (int k = 1; k < dist; k++)
				if (getWorld()->blockedShootingPath(robot_x + k, robot_y))
					okShoot = false;
			return okShoot;
		}

		else if (dist < 0 && getDirection() == left) //robot is facing player
		{
			for (int k = -1; k > dist; k--)
				if (getWorld()->blockedShootingPath(robot_x + k, robot_y))
					okShoot = false;
			return okShoot;
		}
	}

	else if (player_x == robot_x) //player in same col
	{
		dist = player_y - robot_y;
		if ((dist > 0 && getDirection() == up)) //robot is facing player
		{
			for (int k = 1; k < dist; k++)
				if (getWorld()->blockedShootingPath(robot_x, robot_y + k))
					okShoot = false;
			return okShoot;
		}

		else if (dist < 0 && getDirection() == down) //robot is facing player
		{
			for (int k = -1; k > dist; k--)
				if (getWorld()->blockedShootingPath(robot_x, robot_y + k))
					okShoot = false;
			return okShoot;
		}
	}
	return false; //don't shoot
}

Kleptobot::Kleptobot(int startX, int startY, StudentWorld* ptr, int imageID, int hp)
	: Robot(imageID, startX, startY, ptr, right, hp), m_goodie(nullptr), m_moved(0)
{
	distanceBeforeTurning = 1 + (rand() % 6); //select random integer from 1 to 6
}

Kleptobot::~Kleptobot()
{
	if (m_goodie != nullptr)
		m_goodie->dropItem(getX(), getY());
}

GraphObject::Direction Robot::translateDirection(int d)
{
	switch (d)
	{
	case 1:
		return up;
	case 2:
		return down;
	case 3:
		return left;
	case 4:
		return right;
	default:
		cerr << "direction error";
		return none;
	}
}

void Kleptobot::doSomething()
{
	if (!act())
		return;

	//if act on this tick, reset ticks
	tickReset();

	//try to shoot
	if (decideShoot()) //returned true, ok to shoot
	{
		fire();
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		return;
	}

	int pickUpGoodie = rand() % 10; //randomizer, determine whether to pick up goodie
	
	//1 in 10 chance that pickUpGoodie == 1 (1/10 chance of picking up a goodie)
	//if robot randomly decides to pick up goodie and does not already have a goodie
	if (pickUpGoodie == 1 && m_goodie == nullptr)
	{
		m_goodie = getWorld()->findGoodie(getX(), getY());
		//if goodie found, make goodie invisible and not pick-up able
		if (m_goodie != nullptr)
		{
			m_goodie->setVisible(false);
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			return;
		}
	}

	if (m_moved != distanceBeforeTurning && attemptMove(getDirection())) //if can move, move and do nothing else
	{
		m_moved++;
		return;
	}
		
	else
	{
		m_moved = 0; //reset number of moves
		distanceBeforeTurning = 1 + (rand() % 6); //select random integer from 1 to 6
		int direction = 1 + rand() % 4; //generate random direction (int from 1 to 4)
		Direction dir = translateDirection(direction);
		//first check this direction
		if (attemptMove(dir))
			return; //succesfully moved
		else
		{
			for (int d = 1; d <= 4; d++)
			{
				if (d == direction)
					continue; //don't repeat original direction
				Direction newDir = translateDirection(d);
				if (attemptMove(newDir))
				{
					setDirection(newDir); //reset robot's direction
					return; //successfully moved, done
				}
			}
			//no path found in any direction
			setDirection(dir);
		}
	}
}

Angry_Kleptobot::Angry_Kleptobot(int startX, int startY, StudentWorld* ptr)
	:Kleptobot(startX, startY, ptr, IID_ANGRY_KLEPTOBOT, 8){}

Kleptobot_Factory::Kleptobot_Factory(ProductType type, int startX, int startY, StudentWorld* ptr)
	: Actor(IID_ROBOT_FACTORY, startX, startY, ptr), m_type(type){}

void Kleptobot_Factory::doSomething()
{
	int chanceCreateRobot = rand() % 50;
	if (chanceCreateRobot == 1 && getWorld()->doFactoryCensus(getX(), getY())) //chanceCreateRobot == 1? 1/50th chance that this is true
	{
		//add a kleptobot of correct type
		Actor* ptr = nullptr;
		switch (m_type)
		{
		case REGULAR:
			ptr = new Kleptobot(getX(), getY(), getWorld());
			break;
		case ANGRY:
			ptr = new Angry_Kleptobot(getX(), getY(), getWorld());
			break;
		}
		getWorld()->addActor(ptr);
		getWorld()->playSound(SOUND_ROBOT_BORN);
	}
}

//TO DO: load snarlbots in level (set horiz, vert), finish Robot class, make other block functs check for snarlbot
//are snarlbots blocked by other things?
