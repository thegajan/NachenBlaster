#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <vector>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor abc
Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, int startDirection, double size, int depth)
	:GraphObject(imageID, startX, startY, startDirection, size, depth)
{
	m_world = world;
}

bool Actor::getState() const {
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

bool Actor::collide(Actor* p1, Actor* p2) {
	double x1 = p1->getX();
	double y1 = p1->getY();
	double r1 = p1->getRadius();
	double r2 = p2->getRadius();
	double x2 = p2->getX();
	double y2 = p2->getY();
	double distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	if (distance < 0.75*(r1 + r2) && collidable())
		return true;
	return false;
}

//star class
Star::Star(int startX, int startY, StudentWorld* world)
	:Actor(IID_STAR, startX, startY, world, 0, .01*randInt(5, 50), 3)
{}

void Star::doSomething() {
	if (!getState())
		return;
	int x = getX();
	int y = getY();
	moveTo(x - 1, y);
	offScreen();
}

//explosion class
Explosion::Explosion(int startX, int startY, StudentWorld* world) 
	:Actor(IID_EXPLOSION, startX, startY, world)
{}

void Explosion::doSomething() {
	if (!getState())
		return;
	if (m_ticks > 4) {
		changeState();
		return;
	}
	setSize(1.5*getSize());
	m_ticks++;
}

//Craft class
Craft::Craft(int imageID, int startX, int startY, StudentWorld* world, int health)
	:Actor(imageID, startX, startY, world)
{
	setHealth(health);
}

void Craft::killed() {
	if (getHealth() < 0) {
		changeState();
	}
}

//nachenblaster class
NachenBlaster::NachenBlaster(StudentWorld* world)
	:Craft(IID_NACHENBLASTER, 0, 128, world, 50)
{}

void NachenBlaster::doSomething() {
	if (getHealth() < 0)
		changeState();
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
			if (!(x + 6 >= VIEW_WIDTH))
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
			if (m_cabbage >= 5)
				fireCabbage(x, y);
			break;
		case KEY_PRESS_TAB:
			if (m_torpedo >= 1)
				fireTorpedo(x, y);
			break;
		}
	}
	if (m_cabbage < 30)
		m_cabbage++;
}

void NachenBlaster::fireCabbage(int x, int y) {
	m_cabbage = m_cabbage - 5;
	StudentWorld* world = getWorld();
	getWorld()->playSound(SOUND_PLAYER_SHOOT);
	Cabbage* c = new Cabbage(x + 12, y, world);
	world->addItem(c);
}

void NachenBlaster::fireTorpedo(int x, int y) {
	m_torpedo--;
	StudentWorld* world = getWorld();
	getWorld()->playSound(SOUND_TORPEDO);
	Torpedo* t = new Torpedo(x + 12, y, world);
	world->addItem(t);
}

//Villian class
Villain::Villain(int imageID, int startX, int startY, StudentWorld* world, int health, double travelSpeed, int flightPath)
	:Craft(imageID, startX, startY, world, health)
{
	m_travelSpeed = travelSpeed;
	m_flightPath = flightPath;
}

void Villain::getHit() {

}

Smallgon::Smallgon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SMALLGON, startX, startY, world, 5 * (1 + (world->getLevel() - 1)*0.1), 2.0, 0)
{}

void Smallgon::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	killed();
}

Smoregon::Smoregon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SMOREGON, startX, startY, world, 5 * (1 + (world->getLevel() - 1)*0.1), 2.0, 0)
{}

void Smoregon::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	killed();
}

Snagglegon::Snagglegon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SNAGGLEGON, startX, startY, world, 10 * (1 + (world->getLevel() - 1)*0.1), 2.0, 0)
{}

void Snagglegon::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	killed();
}

//projectile class
Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* world, int startDirection)
	:Actor(imageID, startX, startY, world, startDirection, 0.5, 1)
{}

void Projectile::collideWithCraft(int damage, bool targetBad) {
	StudentWorld* world = getWorld();
	std::vector<Actor*> actors = world->getActors();
	std::vector<Actor*>::iterator it = actors.begin();
	while (it != actors.end()) {
		if ((*it)->getState() && (*it)->collide(*it, this)) {
			int health = (*it)->getHealth();
			(*it)->setHealth(health - damage);
			this->changeState();
			if((*it)->getHealth() > 0)
				world->playSound(SOUND_BLAST);
			else
				world->playSound(SOUND_DEATH);
		}
		it++;
	}
}

//cabbage class
Cabbage::Cabbage(int startX, int startY, StudentWorld* world)
	: Projectile(IID_CABBAGE, startX, startY, world)
{}

void Cabbage::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collideWithCraft(2, true);
	int x = getX(), y = getY();
	moveTo(x + 8, y);
	int d = getDirection();
	setDirection(d + 20);
}

//torpedo class
Torpedo::Torpedo(int startX, int startY, StudentWorld* world)
	:Projectile(IID_TORPEDO, startX, startY, world)
{}

void Torpedo::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	int x = getX(), y = getY();
	moveTo(x + 8, y);
}