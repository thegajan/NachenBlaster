#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor abc
Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, int startDirection, double size, int depth)
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

StudentWorld* Actor::getWorld() {
	return m_world;
}

void Actor::offScreen() {
	int x = getX(), y = getY();
	if (x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
		changeState();
}
//star class
Star::Star(int startX, int startY, double size, StudentWorld* world, int startDirection)
	:Actor(IID_STAR, startX, startY, world, startDirection, size, 3)
{}

void Star::doSomething() {
	int x = getX();
	int y = getY();
	moveTo(x - 1, y);
	offScreen();
}

//nachenblaster class
NachenBlaster::NachenBlaster(StudentWorld* world)
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
			if (!(x - 6 < 0))
				moveTo(x - 6, y);
			break;
		case KEY_PRESS_RIGHT:
			if (!( x + 6 >= VIEW_WIDTH))
				moveTo(x + 6, y);
			break;
		case KEY_PRESS_DOWN:
			if (!(y - 6 < 0))
				moveTo(x, y - 6);
			break;
		case KEY_PRESS_UP:
			if (!(y + 6 >= VIEW_HEIGHT))
				moveTo(x, y + 6);
			break;
		case KEY_PRESS_SPACE:
			if (m_cabbage >= 5) {
				fireCabbage(x, y);
			}
			break;
		}
	}
	if (m_cabbage < 30)
		m_cabbage++;
}

void NachenBlaster::fireCabbage(int x, int y){
	m_cabbage -= 5;
	StudentWorld* world = getWorld();
	getWorld()->playSound(SOUND_PLAYER_SHOOT);
	Cabbage* c = new Cabbage(x + 12, y, world);
	world->addItem(c);
}

//projectile class
Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* world, int startDirection)
	:Actor(imageID, startX, startY, world, startDirection, 0.5, 1)
{}

//projectile class
Cabbage::Cabbage(int startX, int startY, StudentWorld* world)
	:Projectile(IID_CABBAGE, startX, startY, world)
{}

void Cabbage::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	int x = getX(), y = getY();
	moveTo(x + 8, y);
}