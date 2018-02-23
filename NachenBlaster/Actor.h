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
	bool getState() const { return m_state; }
	void changeState() { m_state = false; }
	StudentWorld* getWorld() const { return m_world; }
	virtual void offScreen();
//	bool collide(Actor* p1, Actor* p2); //check to see if collision is occuring
	virtual bool collidable() const { return true; } //check to see if object can collide with other objects
	void collision(Actor* p); //call to proccess collision
	virtual bool isEvil() const { return false; }
	int getHealth() const { return m_health; }
	void setHealth(int health) { m_health = health; }
	virtual bool isNach() const { return false; }
	int getDamage() const { return m_damage; }
	virtual bool damageable() const { return false; }
	virtual int type() const { return 0; }
	virtual int score() const { return 0; }
private:
	int m_damage = 0;
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
	virtual bool collidable() { return false; }
private:
	int m_ticks = 0;
};

//Craft class
class Craft : public Actor {
public:
	Craft(int imageID, int startX, int startY, StudentWorld* world, int health, int damage, double size, int depth);
	virtual ~Craft() {}
	virtual void doSomething() = 0;
	virtual bool damageable() const{ return true; }
	virtual void killed();
};

//NachenBlaster class

class NachenBlaster : public Craft {
public:
	NachenBlaster(StudentWorld* world);
	virtual ~NachenBlaster() {}
	virtual void doSomething();
	//virtual void getHit() {}
	virtual bool isNach() const { return true; }
	virtual int type() const { return 1; }
	int getCabbage() const { return m_cabbage; }
	int getTorpedo() const { return m_torpedo; }
private:
	void fire(int x, int y, int type);
	int m_cabbage = 30;
	int m_torpedo = 0;
};

//villian class
class Villain : public Craft {
public:
	Villain(int imageID, int startX, int startY, StudentWorld* world, int health, double travelSpeed, int flightPath, int damage);
	virtual ~Villain() {}
	virtual void doSomething() = 0;
	virtual bool isEvil() const { return true; }
	virtual int type() const { return 2; }
	virtual int score() const { return 250; }
	void flightPath();
	void fly();
	bool actionDuringFlight();
	int getFlightPath() const { return m_flightPath; }
	void setFlightPath(int path) { m_flightPath = path; }
	double getTravelSpeed() const { return m_travelSpeed; }
	void setTravelSpeed(int speed) { m_travelSpeed = speed; }
	int getTravelDir() const { return m_travelDir; }
	void changeTravelDir(int travelDir) { m_travelDir = travelDir; }
	virtual bool notSnagg() const { return true; }
	virtual bool smoregon() const { return false; }
private:
	double m_travelSpeed;
	int m_flightPath;
	int m_travelDir; //1 = dl, 2 = ul, 3 = l
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
	virtual bool smoregon() const { return true; }
};

class Snagglegon : public Villain {
public:
	Snagglegon(int startX, int startY, StudentWorld* world);
	virtual ~Snagglegon() {}
	virtual void doSomething();
	virtual int score() const { return 1000; }
	virtual bool notSnagg() const { return false; }
};

//class projectile

class Projectile : public Actor {
public:
	Projectile(int imageID, int startX, int startY, StudentWorld* world, bool side, int startDirection = 0, int damage = 0);
	virtual ~Projectile() {};
	//virtual void collideWithCraft(int damage, bool targetBad);
	virtual bool collidable() const { return false; }
	virtual int type() const { return 3; }
	virtual bool isEvil() const { return m_side; }
private:
	bool m_side;
};

//cabbage class
class Cabbage : public Projectile {
public:
	Cabbage(int startX, int startY, StudentWorld* world);
	virtual ~Cabbage() {}
	virtual void doSomething();
};

//turnip class
class Turnip :public Projectile {
public:
	Turnip(int startX, int startY, StudentWorld* world);
	virtual ~Turnip() {};
	virtual void doSomething();
};

//torpedo class
class Torpedo : public Projectile {
public:
	Torpedo(int startX, int startY, StudentWorld* world, bool side);
	virtual ~Torpedo() {}
	virtual void doSomething();
};

#endif // ACTOR_H_
