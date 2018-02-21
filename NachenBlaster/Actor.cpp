#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <vector>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor abc
Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, int damage, int startDirection, double size, int depth)
	:GraphObject(imageID, startX, startY, startDirection, size, depth)
{
	m_world = world;
	m_damage = damage;
}

void Actor::offScreen() {
	int x = getX(), y = getY();
	if (x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
		changeState();
}

bool Actor::collide(Actor* p1, Actor* p2) {
	if (p1->getState() && p2->getState()) {
		double x1 = p1->getX();
		double y1 = p1->getY();
		double r1 = p1->getRadius();
		double r2 = p2->getRadius();
		double x2 = p2->getX();
		double y2 = p2->getY();
		double distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
		if (distance < 0.75*(r1 + r2) && collidable())
			return true;
	}
	return false;
}

bool Actor::collision(bool evil) {
		std::vector<Actor*> actors = m_world->getActors();
		std::vector<Actor*>::iterator it = actors.begin();
		while (it != actors.end()) {
			if ((*it)->type() != type() && (*it)->collide(*it, this)) {
				//prevent good from shooting good and bad from shooting bad
				//if (evil && !(*it)->isEvil()) {
				//	it++;
				//	continue;
				//}
				//if (!evil && !(*it)->isEvil()) {
				//	it++;
				//	continue;
				//}
				int health = (*it)->getHealth();
				(*it)->setHealth(health - m_damage);
				if (type() == 3)
					changeState();
				if((*it)->getHealth() > 0 && (*it)->damageable())
					m_world->playSound(SOUND_BLAST);
				else {
					if ((*it)->isEvil())
						m_world->killVillain();
					m_world->playSound(SOUND_DEATH);
					m_world->increaseScore((*it)->score());
				}
			}
			it++;
		}
		return true;
}

//star class
Star::Star(int startX, int startY, StudentWorld* world)
	:Actor(IID_STAR, startX, startY, world, 0, 0, .01*randInt(5, 50), 3)
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
	m_ticks++;
	setSize(1.5*m_ticks);
}

//Craft class
Craft::Craft(int imageID, int startX, int startY, StudentWorld* world, int health, int damage, double size, int depth)
	:Actor(imageID, startX, startY, world, damage, 0, size, depth)
{
	setHealth(health);
}

void Craft::killed() {
	if (getHealth() < 0) {
		changeState();
		Explosion * e = new Explosion(getX(), getY(), getWorld());
		getWorld()->addItem(e);
	}
}

//nachenblaster class
NachenBlaster::NachenBlaster(StudentWorld* world)
	:Craft(IID_NACHENBLASTER, 0, 128, world, 50, 100, 1, 0)
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
	collision(false);
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
	Torpedo* t = new Torpedo(x + 12, y, world, false);
	world->addItem(t);
}

//Villian class
Villain::Villain(int imageID, int startX, int startY, StudentWorld* world, int health, double travelSpeed, int flightPath, int damage)
	:Craft(imageID, startX, startY, world, health, damage, 1.5, 1)
{
	m_travelSpeed = travelSpeed;
	m_flightPath = flightPath;
}

void Villain::flight() {
	//return; //HERE FOR DEBUGING TO PREVENT VILLAINS FROM FLIGHING, COMMENT THIS OUT
	if (m_flightPath == 0 && notSnagg()) {
		m_travelDir = randInt(1, 3);
		m_flightPath = randInt(1, 32);
	}
	else if (getY() - m_travelSpeed <= 0) {
		m_travelDir = 2;
		m_flightPath = randInt(1, 32);
	}
	else if (getY() + m_travelSpeed >= VIEW_HEIGHT - 1) {
		m_travelDir = 1;
		m_flightPath = randInt(1, 32);
	}
	switch (m_travelDir)
	{
	case 1:
		moveTo(getX() - m_travelSpeed, getY() - m_travelSpeed);
		break;
	case 2: 
		moveTo(getX() - m_travelSpeed, getY() + m_travelSpeed);
		break;
	case 3:
		moveTo(getX() - m_travelSpeed, getY());
		break;
	}
	if(notSnagg())
		m_flightPath--;
}

Smallgon::Smallgon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SMALLGON, startX, startY, world, 5 * (1 + (world->getLevel() - 1)*0.1), 2.0, 0, 5)
{}

void Smallgon::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collision(true);
	killed();
	flight();
	collision(true);
	killed();
}

Smoregon::Smoregon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SMOREGON, startX, startY, world, 5 * (1 + (world->getLevel() - 1)*0.1), 2.0, 0, 5)
{}

void Smoregon::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collision(true);
	killed();
	flight();
	collision(true);
	killed();
}

Snagglegon::Snagglegon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SNAGGLEGON, startX, startY, world, 10 * (1 + (world->getLevel() - 1)*0.1), 1.75, 0, 15)
{
	changeTravelDir(1);
}

void Snagglegon::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collision(true);
	killed();
	flight();
	collision(true);
	killed();
}

//projectile class
Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* world, bool side, int startDirection, int damage)
	:Actor(imageID, startX, startY, world, damage, startDirection, 0.5, 1)
{}

//cabbage class
Cabbage::Cabbage(int startX, int startY, StudentWorld* world)
	: Projectile(IID_CABBAGE, startX, startY, world, false, 0, 2)
{}

void Cabbage::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collision(false);
	int x = getX(), y = getY();
	moveTo(x + 8, y);
	int d = getDirection();
	setDirection(d + 20);
}

//turnip class
Turnip::Turnip(int startX, int startY, StudentWorld* world)
	:Projectile(IID_TURNIP, startX, startY, world, true, 0, 2)
{}

void Turnip::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collision(true);
	int x = getX(), y = getY();
	moveTo(x - 6, y);
	int d = getDirection();
	setDirection(d + 20);
}

//torpedo class
Torpedo::Torpedo(int startX, int startY, StudentWorld* world, bool side)
	:Projectile(IID_TORPEDO, startX, startY, world, side, 0, 8)
{}

void Torpedo::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	int x = getX(), y = getY();
	if (isEvil() == true)
		moveTo(x - 8, y);
	else
		moveTo(x + 8, y);
}