#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world, int damage = 0, int startDirection = 0, double size = 1.0, int depth = 0); //constructor
	virtual ~Actor() {} //destructor
	virtual void doSomething() = 0; //every actor has a doSomething that does something specific to them
	bool getState() const { return m_state; } //return a boolean stating whether actor is alive or dead
	void changeState() { m_state = false; } //change actor to dead
	StudentWorld* getWorld() const { return m_world; } //return a pointer to the studentworld
	void offScreen(); //check if an object is offscreen, if so kill it
	virtual bool collidable() const { return true; } //state if object can collide with other objects
	void collision(Actor* p); //call to proccess collision
	virtual bool isEvil() const { return false; } //state that identifies villians and villain projectiles
	int getHealth() const { return m_health; } //return health of actor
	void setHealth(int health) { m_health = health; } //set health of actor
	int getDamage() const { return m_damage; } //get damage actor does to other actors
	virtual bool damageable() const { return false; } //boolean that determines if actor can take damage
	virtual int type() const { return 0; } //return an int that identifies the type of actor
	virtual int score() const { return 0; } //return the score associated with each actor
	virtual void action() {} //if an actor performs a special action during its tick
private:
	int m_damage = 0;
	int m_health = 0;
	bool m_state = true;
	StudentWorld* m_world;
};

class Star : public Actor {
public:
	Star(int startX, int startY, StudentWorld* world); //constructor
	virtual ~Star() {} //destructor
	virtual void doSomething(); //doSomething is moving the stars
	virtual bool collidable() { return false; } //cannot collide with stars
};

class Explosion : public Actor {
public:
	Explosion(int startX, int startY, StudentWorld* world); //constructor
	virtual ~Explosion() {}; // destructor
	virtual void doSomething(); //do something is performing explosion graphic
	virtual bool collidable() { return false; } //cannot collide with explosion
private:
	int m_ticks = 0;
};

//Craft class
class Craft : public Actor {
public:
	Craft(int imageID, int startX, int startY, StudentWorld* world, int health, int damage, double size, int depth); //constructor
	virtual ~Craft() {} //destructor
	virtual void doSomething() = 0; //do something is pure virtual because nachenblaster and villains have different do something
	virtual bool damageable() const{ return true; } //craft can take damage
};

//NachenBlaster class

class NachenBlaster : public Craft {
public:
	NachenBlaster(StudentWorld* world);  //constructor
	virtual ~NachenBlaster() {} //destructor
	virtual void doSomething(); //do soemthing controls nachenblaster
	virtual int type() const { return 1; } //return type that identifies nachenblaster
	int getCabbage() const { return m_cabbage; } //get amount of Cabbages
	int getTorpedo() const { return m_torpedo; } //get amount of Torpedoes
	void setTorpedo(int amt) { m_torpedo = m_torpedo + amt; } //set number of torpedoes
private:
	void fire(int x, int y, int type);
	int m_cabbage = 30;
	int m_torpedo = 0;
};

//villian class
class Villain : public Craft {
public:
	Villain(int imageID, int startX, int startY, StudentWorld* world, int health, double travelSpeed, int flightPath, int damage); //constructor
	virtual ~Villain() {} //destructor
	virtual bool isEvil() const { return true; } //villians are evil
	virtual int type() const { return 2; } //type that identifies villain class
	virtual int score() const { return 250; } //score when destroyed (changed for snagglegon)
	void changeTravelDir(int travelDir) { m_travelDir = travelDir; } //change the travel direction
	virtual bool notSnagg() const { return true; } //bool determines if villain is a snagg
	virtual bool smoregon() const { return false; } //bool determines if villain is a smoregon
	virtual void doSomething(); //defines the villains action in a tick
	virtual void action();
private:
	bool actionDuringFlight();
	void flightPath(); //determine the flight path of the villain
	void fly();
	void killed(); //perform actions when craft is killed
	double m_travelSpeed;
	int m_flightPath;
	int m_travelDir; //1 = dl, 2 = ul, 3 = l
};

class Smallgon : public Villain {
public:
	Smallgon(int startX, int startY, StudentWorld* world); //constructor
	virtual ~Smallgon() {}; //destructor
};

class Smoregon : public Villain {
public:
	Smoregon(int startX, int startY, StudentWorld* world); //constructor
	virtual ~Smoregon() {} //destructor
	virtual bool smoregon() const { return true; } //bool that states this is smoregon class
};

class Snagglegon : public Villain {
public:
	Snagglegon(int startX, int startY, StudentWorld* world); //constructor
	virtual ~Snagglegon() {} //destructor
	virtual int score() const { return 1000; } //score specific to snagglegon
	virtual bool notSnagg() const { return false; } //bool that states this is snagglegon
};

//class projectile

class Projectile : public Actor {
public:
	Projectile(int imageID, int startX, int startY, StudentWorld* world, bool side, int startDirection = 0, int damage = 0); //constructor
	virtual ~Projectile() {}; //destructor
	virtual bool collidable() const { return false; } //projectiles cannot collide with objects, only the other way around. this prevents double collisions
	virtual int type() const { return 3; } //return the type of a projectile
	virtual bool isEvil() const { return m_side; } //return whether a projectile is evil or not to prevent villains damaging villains
	virtual int typeOfProj() const = 0; //return type of the projectile being fired
	virtual void doSomething(); //perform the projectiles action during a tick
private:
	bool m_side;
};

//cabbage class
class Cabbage : public Projectile {
public:
	Cabbage(int startX, int startY, StudentWorld* world); //constructor
	virtual ~Cabbage() {} //destructor
	virtual int typeOfProj() const { return 0; } //return type associated to cabbage
};

//turnip class
class Turnip :public Projectile {
public:
	Turnip(int startX, int startY, StudentWorld* world); //constructor
	virtual ~Turnip() {}; // destructor
	virtual int typeOfProj() const { return 1; } //return type associated with turnip
};

//torpedo class
class Torpedo : public Projectile {
public:
	Torpedo(int startX, int startY, StudentWorld* world, bool side); //constructor
	virtual ~Torpedo() {} // destructor
	virtual int typeOfProj() const { return 2; } //return type associated with torpedo
};

//goodie class
class Goodie : public Actor {
public:
	Goodie(int imageID, int startX, int startY, StudentWorld* world); //constructor
	virtual ~Goodie() {}; //destructor
	virtual int type() const { return 4; } //return type of actor 
	virtual void doSomething(); //perform action for the goodies during a tick
	virtual bool collidable() const { return false; } //projectiles cannot collide with objects, only the other way around. this prevents double collisions
};

class ExtraLife :public Goodie {
public:
	ExtraLife(int startX, int startY, StudentWorld* world); //constructor
	virtual ~ExtraLife() {}; //destructor
	virtual void action(); //perform special collision action for extra life
};

class Repair :public Goodie {
public:
	Repair(int startX, int startY, StudentWorld* world); //constructor
	virtual ~Repair() {}; //destructor
	virtual void action(); //perform special collision action for repair 
};

class TorpedoGoodie :public Goodie {
public:
	TorpedoGoodie(int startX, int startY, StudentWorld* world); //constructor
	virtual ~TorpedoGoodie() {}; //destructor
	virtual void action(); // perform special collision action for torpedo goodie
};
#endif // ACTOR_H_
