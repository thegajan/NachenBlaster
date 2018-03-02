#include "Actor.h"
#include "StudentWorld.h"
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

void Actor::collision(Actor* p) {
	getWorld()->collisionOccur(p);
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
	if (m_ticks >= 4) {
		changeState();
		return;
	}
	if (m_ticks == 0)
		setSize(1);
	else
		setSize(1.5*getSize());
	m_ticks++;
}

//Craft class
Craft::Craft(int imageID, int startX, int startY, StudentWorld* world, int health, int damage, double size, int depth)
	:Actor(imageID, startX, startY, world, damage, 0, size, depth)
{
	setHealth(health);
}

//nachenblaster class
NachenBlaster::NachenBlaster(StudentWorld* world)
	:Craft(IID_NACHENBLASTER, 0, 128, world, 50, 100, 1, 0)
{}

void NachenBlaster::doSomething	() {
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
				fire(x, y, 0);
			break;
		case KEY_PRESS_TAB:
			if (m_torpedo >= 1)
				fire(x, y, 1);
			break;
		}
	}
	if (m_cabbage < 30)
		m_cabbage++;
	collision(this);
}

void NachenBlaster::fire(int x, int y, int type) {
	if (type == 0) {
		m_cabbage = m_cabbage - 5;
		Projectile* p = new Cabbage(x + 12, y, getWorld());
		getWorld()->playSound(SOUND_PLAYER_SHOOT);
		getWorld()->addItem(p);
	}
	else if (type == 1) {
		m_torpedo--;
		Projectile* p = new Torpedo(x + 12, y, getWorld(), false);
		getWorld()->playSound(SOUND_TORPEDO);
		getWorld()->addItem(p);
	}
}

//Villian class
Villain::Villain(int imageID, int startX, int startY, StudentWorld* world, int health, double travelSpeed, int flightPath, int damage)
	:Craft(imageID, startX, startY, world, health, damage, 1.5, 1)
{
	m_travelSpeed = travelSpeed;
	m_flightPath = flightPath;
}

void Villain::flightPath() {
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
}


void Villain::killed() {
	if (getHealth() < 0 && getState()) {
		action();
		changeState();
		Explosion * e = new Explosion(getX(), getY(), getWorld());
		getWorld()->addItem(e);
	}
}

void Villain::fly() {
	//return;
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
	if (notSnagg())
		m_flightPath--;
}

bool Villain::actionDuringFlight() {
	int nachX = getWorld()->getNach()->getX();
	int nachY = getWorld()->getNach()->getY();
	if (nachX < getX() && abs(nachY - getY()) <= 4 && abs(nachY - getY()) >= 0) {
		int rand = randInt(0, ((20 / getWorld()->getLevel()) + 5) - 1);
		int rand2 = randInt(0, (((15 / getWorld()->getLevel()) + 10) - 1));
		if (rand == 0 && notSnagg()) {
			Turnip* t = new Turnip(getX() - 14, nachY, getWorld());
			getWorld()->addItem(t);
			getWorld()->playSound(SOUND_ALIEN_SHOOT);
			return true;
		}
		if (rand == 1 && smoregon()) {
			m_travelDir = 3;
			m_flightPath = VIEW_WIDTH;
			m_travelSpeed = 5;
			return false;
		}
		if (rand2 == 0 && !notSnagg()) {
			Torpedo* t = new Torpedo(getX() - 14, getY(), getWorld(), true);
			getWorld()->addItem(t);
			getWorld()->playSound(SOUND_TORPEDO);
			return true;
		}
	}
	return false;
}

void Villain::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collision(this);
	killed();
	flightPath();
	if (!actionDuringFlight())
		fly();
	collision(this);
	killed();
}

void Villain::action() {
	if (smoregon()) {
		int rand = randInt(0, 2);
		//int rand = 0; //REMOVE ONLY FOR DEBUGGING
		if (rand == 0) {
			int rand2 = randInt(0, 1);
			if (rand2 == 0) {
				Repair* r = new Repair(getX(), getY(), getWorld());
				getWorld()->addItem(r);
			}
			else {
				TorpedoGoodie* t = new TorpedoGoodie(getX(), getY(), getWorld());
				getWorld()->addItem(t);
			}
		}
	}
	else if (!notSnagg()) {
		int rand = randInt(0, 5);
		//int rand = 0;
		if (rand == 0) {
			ExtraLife* e = new ExtraLife(getX(), getY(), getWorld());
			getWorld()->addItem(e);
		}
	}
}

Smallgon::Smallgon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SMALLGON, startX, startY, world, (5 * (1 + (world->getLevel() - 1)*0.1)), 2.0, 0, 5)
{}

Smoregon::Smoregon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SMOREGON, startX, startY, world, (5 * (1 + (world->getLevel() - 1)*0.1)), 2.0, 0, 5)
{}

Snagglegon::Snagglegon(int startX, int startY, StudentWorld* world)
	:Villain(IID_SNAGGLEGON, startX, startY, world, (10 * (1 + (world->getLevel() - 1)*0.1)), 1.75, 0, 15)
{
	changeTravelDir(1);
}

//projectile class
Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* world, bool side, int startDirection, int damage)
	:Actor(imageID, startX, startY, world, damage, startDirection, 0.5, 1)
{
	m_side = side;
}

void Projectile::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collision(this);

	int x = getX(), y = getY();
	int d = getDirection();

	if (typeOfProj() == 0) {
		moveTo(x + 8, y);
		setDirection(d + 20);
	}
	else if (typeOfProj() == 1) {
		moveTo(x - 6, y);
		setDirection(d + 20);
	}
	else if (typeOfProj() == 2) {
		if (isEvil() == true)
			moveTo(x - 8, y);
		else
			moveTo(x + 8, y);
	}
}

//cabbage class
Cabbage::Cabbage(int startX, int startY, StudentWorld* world)
	: Projectile(IID_CABBAGE, startX, startY, world, false, 0, 2)
{}

//turnip class
Turnip::Turnip(int startX, int startY, StudentWorld* world)
	:Projectile(IID_TURNIP, startX, startY, world, true, 0, 2)
{}

//torpedo class
Torpedo::Torpedo(int startX, int startY, StudentWorld* world, bool side)
	:Projectile(IID_TORPEDO, startX, startY, world, side, 0, 8)
{}

//goodie class
Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* world)
	:Actor(imageID, startX, startY, world, 0, 0, 0.5, 1)
{}

void Goodie::doSomething() {
	if (!getState())
		return;
	offScreen();
	if (!getState())
		return;
	collision(this);
	moveTo(getX() - .75, getY() - .75);
	collision(this);
}

//extra life
ExtraLife::ExtraLife(int startX, int startY, StudentWorld* world)
	: Goodie(IID_LIFE_GOODIE, startX, startY, world)
{}

void ExtraLife::action() {
	getWorld()->incLives();
}

//repair
Repair::Repair(int startX, int startY, StudentWorld* world)
	: Goodie(IID_REPAIR_GOODIE, startX, startY, world)
{}

void Repair::action() {
	int nachHealth = getWorld()->getNach()->getHealth();
	if (nachHealth + 10 > 50)
		getWorld()->getNach()->setHealth(50);
	else
		getWorld()->getNach()->setHealth(nachHealth + 10);
}

//torpedo goodie
TorpedoGoodie::TorpedoGoodie(int startX, int startY, StudentWorld* world)
	: Goodie(IID_TORPEDO_GOODIE, startX, startY, world)
{}

void TorpedoGoodie::action() {
	getWorld()->getNach()->setTorpedo(5);
}