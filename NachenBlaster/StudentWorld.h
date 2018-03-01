#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


//Student World Class
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir); //constructor
	virtual ~StudentWorld(); //destructor
	virtual int init(); //game calls init to start every level
	virtual int move(); //game calls move every tick of the game
	virtual void cleanUp(); //game calls cleanup when level or game is over
	void addItem(Actor* x) { m_v.push_back(x); } //introduce a game element into the game
	NachenBlaster* getNach() const { return m_nach; }  //return the pointer to the nacheblaster
	void collisionOccur(Actor* p1); //check if an actor is colliding with something
private:
	void killVillain() { m_numVillainsDestroyed++; } //increase the number of villians killed in a round
	bool collisionDistance(Actor* p1, Actor* p2) const; //check to see if two objects are in collision distance
	void displayStatus(); //update the stats bar
	void newItem(); //for the game to decide when to introduce new characters to the game
	NachenBlaster * m_nach; //pointer to nachenblaster
	std::vector<Actor*> m_v; //vector containing actors
	int m_numVillains; //number of villians for each level
	int m_numVillainsDestroyed; //number of villians killed so far
};

#endif // STUDENTWORLD_H_
