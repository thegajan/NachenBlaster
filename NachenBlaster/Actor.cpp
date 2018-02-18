#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor abc
Actor::Actor(int imageID, int startX, int startY, GameWorld* world, int startDirection, double size, int depth)
	:GraphObject(imageID, startX, startY, startDirection, size, depth)
{
	m_world = world;
}

bool Actor::getState() const{
	return m_state;
}

void Actor::changeState() {
	m_state = false;
}

GameWorld* Actor::getWorld()
{
	return m_world;
}
//star class
Star::Star(int startX, int startY, double size, GameWorld* world, int startDirection)
	:Actor(IID_STAR, startX, startY, world, startDirection, size, 3)
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

//nachenblaster class
NachenBlaster::NachenBlaster(GameWorld* world)
	:Actor(IID_NACHENBLASTER, 0, 128, world)
{}

void NachenBlaster::doSomething() {
	//check if nachenblaster is dead
	if (!getState()) {
		return;
	}
	//check key input
	int ch;
	if (getWorld()->getKey(ch)) {
		int x = getX(), y = getY();
		switch (ch) {
		case KEY_PRESS_LEFT:
			if (x != 0)
				moveTo(x - 1, y);
			break;
		case KEY_PRESS_RIGHT:
			if (x != VIEW_WIDTH - 1)
				moveTo(x + 1, y);
			break;
		case KEY_PRESS_DOWN:
			if (y != 0)
				moveTo(x, y - 1);
			break;
		case KEY_PRESS_UP:
			if (y != VIEW_HEIGHT - 1)
				moveTo(x, y + 1);
			break;
		}
	}
	if (m_cabbage < 30)
		m_cabbage++;
}
