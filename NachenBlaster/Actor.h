#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world, int damage = 0, int startDirection = 0, double size = 1.0, int depth = 0);
	virtual ~Actor() {}
	virtual void doSomething() = 0;
	bool getState() {return m_state; }
	void changeState() { m_state = false; }
	StudentWorld* getWorld() { return m_world; }
	virtual void offScreen();
	bool collide(Actor* p1, Actor* p2); //check to see if collision is occuring
	virtual bool collidable() { return true; } //check to see if object can collide with other objects
	bool collision(); //call to proccess collision
	virtual bool isEvil() { return false; }
	int getHealth() { return m_health; }
	void setHealth(int health) { m_health = health; }
	virtual bool isNach() { return false; }
	int getDamage() { return m_damage; }
	virtual bool damageable()	{ return false; }
	virtual int type() { return 0; }
private:
	int m_damage;
	int m_health = 0;
	bool m_state = true;
	StudentWorld* m_world;
};

class Star : public Actor {
public:
	Star(int startX, int startY, StudentWorld* world);
	virtual ~Star() {}
	virtual void doSomething();
	virtual bool collidable() { return false; }
};

class Explosion : public Actor {
public:
	Explosion(int startX, int startY, StudentWorld* world);
	virtual ~Explosion() {};
	virtual void doSomething();
private:
	int m_ticks = 0;
};

//Craft class
class Craft : public Actor {
public:
	Craft(int imageID, int startX, int startY, StudentWorld* world, int health, int damage);
	virtual ~Craft() {}
	virtual void doSomething() = 0;
	virtual void getHit() = 0;
	virtual void killed();
	virtual bool damageable() { return true; }
};

//NachenBlaster class

class NachenBlaster : public Craft {
public:
	NachenBlaster(StudentWorld* world);
	virtual ~NachenBlaster() {}
	virtual void doSomething();
	void fireCabbage(int x, int y);
	void fireTorpedo(int x, int y);
	int getCabbage() { return m_cabbage; }
	int getTorpedo() { return m_torpedo; }
	virtual void getHit() {}
	virtual bool isNach() { return true; }
	virtual int type() { return 1; }
private:
	int m_cabbage = 30;
	int m_torpedo = 0;
};

//villian class
class Villain : public Craft {
public:
	Villain(int imageID, int startX, int startY, StudentWorld* world, int health, double travelSpeed, int flightPath, int damage);
	virtual ~Villain() {}
	virtual void doSomething() = 0;
	virtual bool isEvil() { return true; }
	virtual void getHit();
	virtual int type() { return 2; }
private:
	double m_travelSpeed;
	int m_flightPath;
};

class Smallgon : public Villain {
public:
	Smallgon(int startX, int startY, StudentWorld* world);
	virtual ~Smallgon() {};
	virtual void doSomething();
};

class Smoregon : public Villain {
public:
	Smoregon(int startX, int startY, StudentWorld* world);
	virtual ~Smoregon() {}
	virtual void doSomething();
};

class Snagglegon : public Villain {
public:
	Snagglegon(int startX, int startY, StudentWorld* world);
	virtual ~Snagglegon() {}
	virtual void doSomething();
};

//class projectile

class Projectile : public Actor {
public:
	Projectile(int imageID, int startX, int startY, StudentWorld* world, int startDirection = 0, int damage = 0);
	virtual ~Projectile() {};
	//virtual void collideWithCraft(int damage, bool targetBad);
	virtual bool collidable() { return false; }
	virtual int type() { return 3; }
};

//cabbage class

class Cabbage : public Projectile {
public:
	Cabbage(int startX, int startY, StudentWorld* world);
	virtual ~Cabbage() {}
	virtual void doSomething();
};

//torpedo class
class Torpedo : public Projectile {
public:
	Torpedo(int startX, int startY, StudentWorld* world);
	virtual ~Torpedo() {}
	virtual void doSomething();
};

#endif // ACTOR_H_
