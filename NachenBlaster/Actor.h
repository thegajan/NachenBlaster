#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world, int startDirection = 0, double size = 1.0, int depth = 0);
	virtual ~Actor() {}
	virtual void doSomething() = 0;
	bool getState() const;
	void changeState();
	StudentWorld* getWorld();
	virtual void offScreen();
private:
	bool m_state = true;
	StudentWorld* m_world;
};

class Star: public Actor {
public:
	Star(int startX, int startY, double size, StudentWorld* world, int startDirection = 0);
	virtual ~Star() {}
	virtual void doSomething();
};

class NachenBlaster: public Actor {
public:
	NachenBlaster(StudentWorld* world);
	virtual ~NachenBlaster() {}
	virtual void doSomething();
	void fireCabbage(int x, int y);
	void fireTorpedo(int x, int y);
	int getHealth() { return m_health; };
	int getCabbage() { return m_cabbage; };
	int getTorpedo() { return m_torpedo; };
private:
	int m_health = 50;
	int m_cabbage = 30;
	int m_torpedo = 0;
};

//class projectile

class Projectile: public Actor {
public:
	Projectile(int imageID, int startX, int startY, StudentWorld* world, int startDirection = 0);
	virtual ~Projectile() {};
};

//cabbage class

class Cabbage: public Projectile {
public:
	Cabbage(int startX, int startY, StudentWorld* world);
	virtual ~Cabbage() {};
	virtual void doSomething();
};

//torpedo class
class Torpedo : public Projectile {
public: 
	Torpedo(int startX, int startY, StudentWorld* world);
	virtual ~Torpedo() {};
	virtual void doSomething();
};

#endif // ACTOR_H_
