#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject {
public:
	Actor(int imageID, int startX, int startY, int startDirection = 0, double size = 1.0, int depth = 0);
	virtual ~Actor() {};
	virtual void doSomething() = 0;
	bool getState() const;
	void changeState();
private:
	bool m_state = true;
};

class Star: public Actor {
public:
	Star(int startX, int startY, double size, int startDirection = 0);
	virtual ~Star() {};
	void doSomething();
	void offScreen();
};

class NachenBlaster: public Actor {

};

#endif // ACTOR_H_
