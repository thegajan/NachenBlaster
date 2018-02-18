#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
public:
	Actor(int imageID, int startX, int startY, GameWorld* world, int startDirection = 0, double size = 1.0, int depth = 0);
	virtual ~Actor() {}
	virtual void doSomething() = 0;
	bool getState() const;
	void changeState();
	GameWorld* getWorld();
private:
	bool m_state = true;
	GameWorld* m_world;
};

class Star: public Actor {
public:
	Star(int startX, int startY, double size, GameWorld* world, int startDirection = 0);
	virtual ~Star() {}
	void doSomething();
	void offScreen();
};

class NachenBlaster: public Actor {
public:
	NachenBlaster(GameWorld* world);
	virtual ~NachenBlaster() {}
	void doSomething();
private:
	int m_life = 50;
	int m_cabbage = 30;
};

#endif // ACTOR_H_
