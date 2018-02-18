#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor abc
Actor::Actor(int imageID, int startX, int startY, int startDirection, double size, int depth)
	:GraphObject(imageID, startX, startY, startDirection, size, depth)
{}

bool Actor::getState() const{
	return m_state;
}

void Actor::changeState() {
	m_state = false;
}

//star class
Star::Star(int startX, int startY, double size, int startDirection)
	:Actor(IID_STAR, startX, startY, startDirection, size, 3)
{}

void Star::doSomething() {
	int x = getX();
	int y = getY();
	moveTo(x - 1, y);
	offScreen();
}

void Star::offScreen() {
	int x = getX();
	if (x < 0)
		changeState();
}