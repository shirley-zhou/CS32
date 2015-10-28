#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// contains base, Player, and Wall class declarations

class Actor : public GraphObject
{
public:

	Actor(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir = none, int hp = 0);
	virtual ~Actor(){};

	//do something each tick
	virtual void doSomething()=0;

	//can actor be hit by bullet?
	virtual bool attackable() = 0;

	//does this actor block robot attacks?
	virtual bool blockRobotAttack() = 0;

	//does this actor block other actors from moving?
	virtual bool blockCoord(Direction dir, Actor* callerID){ return true; };

	// Does this actor count when a factory counts items near it?
	virtual bool countsInFactoryCensus(){ return false; };

	//return pointer to world actor is in
	StudentWorld* getWorld(){ return m_world; };

	//calculate next coord given direction and x,y
	void nextCoord(int& x, int& y, Direction& dir);

	//fire bullet
	void fire();

	//is actor alive?
	bool alive(){ return m_alive; };

	//make actor dead
	void setDead(){ m_alive = false; };

	//reduce health points by certain number
	void decHP(int n){ m_hp -= n; };

	//check health remaining
	int checkHP(){ return m_hp; };

	//restore hp (player starts out with 20hp)
	void restoreHP(){ m_hp = 20; };
private:
	StudentWorld* m_world;
	bool m_alive;
	int m_hp;
};
/*
class Agent : public Actor
{
public:
	Agent(StudentWorld* world, int startX, int startY, int imageID,
		unsigned int hp, Direction startDir);

	// Move to the adjacent square in the direction the agent is facing
	// if it is not blocked, and return true.  Return false if the agent
	// can't move.
	bool attemptMove();

	// Return true if this agent can push boulders (which means it's the
	// player).
	virtual bool canPushBoulders() const;

	// Return true if this agent doesn't shoot unless there's an unobstructed
	// path to the player.
	virtual bool needsClearShot() const;

	// Return the sound effect ID for a shot from this agent.
	// player).
	virtual int shootingSound() const;
};
*/
class Player : public Actor
{
public:
	Player(int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
	virtual bool attackable();
	virtual bool blockRobotAttack(){ return false; };
	//add ammo when pick up ammo goodie
	void addAmmo(){ m_ammo += 20; };
	//check ammo remaining
	int checkAmmo(){ return m_ammo; };
private:

	//reduce ammo by one when bullet fired
	void decAmmo(){ m_ammo--; };

	int m_ammo;
};

class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* ptr);
	virtual void doSomething(){};
	virtual bool attackable(){ return true; }; //return true because bullets can attack walls (no damage, but they can't pass through)
	virtual bool blockRobotAttack(){ return true; };
};

class Bullet : public Actor
{
public:
	Bullet(int startX, int startY, StudentWorld* ptr, Direction dir);
	virtual void doSomething();
	virtual bool attackable(){ return false; }; //return false because bullets pass through each other
	virtual bool blockCoord(Direction dir, Actor* callerID){ return false; }; //bullets don't block actors
	virtual bool blockRobotAttack(){ return false; };
};

class Boulder : public Actor
{
public:
	Boulder(int startX, int startY, StudentWorld* ptr);
	virtual void doSomething(){};
	virtual bool attackable();
	virtual bool blockCoord(Direction dir, Actor*);
	virtual bool blockRobotAttack(){ return true; };
	bool push(Direction dir);
};

class Hole : public Actor
{
public:
	Hole(int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
	virtual bool attackable(){ return false; }; //holes can't be attacked, bullets pass through
	virtual bool blockCoord(Direction dir, Actor* callerID);
	virtual bool blockRobotAttack(){ return false; };
};

class Exit : public Actor
{
public:
	Exit(int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
	virtual bool attackable(){ return false; }; //exit can't be attacked, bullets pass through
	virtual bool blockCoord(Direction dir, Actor* callerID){ return false; };
	virtual bool blockRobotAttack(){ return false; };
};

class Item : public Actor
{
public:
	Item(int imageID, int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
	virtual bool attackable(){ return false; } //goodies can't be attacked, bullets pass through
	virtual bool blockCoord(Direction dir, Actor* callerID);
	virtual void collectItem() = 0;
	virtual bool blockRobotAttack(){ return false; };
};

class Jewel : public Item
{
public:
	Jewel(int startX, int startY, StudentWorld* ptr);
	virtual void collectItem();
};

class Goodie : public Item
{
public:
	Goodie(int imageID, int startX, int startY, StudentWorld* ptr);
	void dropItem(int x, int y);
};

class Extra_Life : public Goodie
{
public:
	Extra_Life(int startX, int startY, StudentWorld* ptr);
	virtual void collectItem();
};

class Restore_Health : public Goodie
{
public:
	Restore_Health(int startX, int startY, StudentWorld* ptr);
	virtual void collectItem();
};

class Ammo : public Goodie
{
public:
	Ammo(int startX, int startY, StudentWorld* ptr);
	virtual void collectItem();
};

class Robot : public Actor
{
public:
	Robot(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir, int hp);
	virtual void doSomething();
	virtual bool attackable();
	virtual bool blockRobotAttack(){ return true; };
	virtual bool decideShoot();
	void reverseDirection();
	bool act();
	bool attemptMove(Direction dir);
	GraphObject::Direction translateDirection(int d);
	void tickReset(){ m_tick = 1; };
	virtual int pointsWorth() = 0;
private:
	int m_moveFrequency;
	int m_tick;
};

class Horiz_Snarlbot : public Robot
{
public:
	Horiz_Snarlbot(int startX, int startY, StudentWorld* ptr);
	virtual int pointsWorth(){ return 100; };
};

class Vert_Snarlbot : public Robot
{
public:
	Vert_Snarlbot(int startX, int startY, StudentWorld* ptr);
	virtual int pointsWorth(){ return 100; };
};

class Kleptobot : public Robot
{
public:
	Kleptobot(int startX, int startY, StudentWorld* ptr, int imageID = IID_KLEPTOBOT, int hp = 5);
	virtual ~Kleptobot();
	virtual bool decideShoot(){ return false; }; //regular kleptobots never shoot
	virtual void doSomething();
	virtual int pointsWorth(){ return 10; };
	virtual bool countsInFactoryCensus(){ return true; }; //all kleptobots count toward factory census
private:
	int distanceBeforeTurning;
	int m_moved;
	Goodie* m_goodie;
};

class Angry_Kleptobot : public Kleptobot
{
public:
	Angry_Kleptobot(int startX, int startY, StudentWorld* ptr);
	virtual bool decideShoot(){ return Robot::decideShoot(); }; //use robot's function to decide
	virtual int pointsWorth(){ return 20; };
};

class Kleptobot_Factory : public Actor
{
public:
	enum ProductType { REGULAR, ANGRY };
	Kleptobot_Factory(ProductType type, int startX, int startY, StudentWorld* ptr);
	virtual void doSomething();
	virtual bool attackable(){ return true; }; //return true because bullets can attack factories (no damage, but they can't pass through)
	virtual bool blockRobotAttack(){ return true; };
private:
	ProductType m_type;
};

//TO DO: check all block functions, simplify attack bulletDamage

#endif // ACTOR_H_