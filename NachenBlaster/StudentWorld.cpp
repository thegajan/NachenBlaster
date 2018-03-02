#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp
//constructor
StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir)
{}
//destructor
StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init()
{
	//initialize number of villians to kill
	m_numVillains = 6 + (4 * getLevel());
	m_numVillainsDestroyed = 0;
	//create nachenblaster
	m_nach = new NachenBlaster(this);
	addItem(m_nach);
	//create 30 stars
	for (int i = 0; i < 30; i++) {
		int x = randInt(0, VIEW_WIDTH - 1);
		int y = randInt(0, VIEW_HEIGHT - 1);
		Star* p = new Star(x, y, this);
		addItem(p);
	}
	//displayStatus();

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	//make every actor in the vector do something, if they are dead however, delete them from the game
	for (vector<Actor*>::size_type n = 0; n < m_v.size();) {
		m_v[n]->doSomething();
		if (!(m_v[n]->getState())) {
			delete m_v[n];
			m_v.erase(m_v.begin() + n);
		}
		else
			n++;
	}
	//have the game decide whether to add a new item to the game
	newItem();

	m_nach->doSomething();
	
	//update the status bar
	displayStatus();
	//if the nachebblaster health is less than 0 end the level
	if (m_nach->getHealth() <= 0) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	//if goal number of villians killed, move to next level
	if (m_numVillainsDestroyed == m_numVillains) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	//continue level
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	//delete all actors in vector
	vector<Actor*>::iterator i = m_v.begin();
	while (i != m_v.end()) {

		delete *i;
		i = m_v.erase(i);
	}
}

void StudentWorld::newItem() {
	//introduce new stars
	int x = randInt(0, 14);
	if (x == 0) {
		int x = VIEW_WIDTH - 1;
		int y = randInt(0, VIEW_HEIGHT - 1);
		Star* p = new Star(x, y, this);
		addItem(p);
	}
	//introduce villains
	//decide whether to add a new enemy based on probability
	int d = m_numVillainsDestroyed;
	int r = m_numVillains - d;
	double m = 4 + (0.5*getLevel());
	int i = 0;
	vector<Actor*>::iterator it = m_v.begin();
	while (it != m_v.end()) {
		if ((*it)->getState() && (*it)->isEvil())
			i++;
		it++;
	}
	int min = (m < r) ? m : r;
	//add new enemy depending and decide what type to add
	if (i < min) {
		int s1 = 60;
		int s2 = 20 + getLevel() * 5;
		int s3 = 5 + getLevel() * 10;
		int s = s1 + s2 + s3;
		int rand = randInt(0, s - 1);
		Actor* a;
		if (rand <= s1)
			a = new Smallgon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT), this);
		else if (rand <= s1 + s2)
			a = new Smoregon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT), this);
		else
			a = new Snagglegon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT), this);
		addItem(a);
	}
}

//update the status bar by adding all useful information to the output stream and then running that into the update function
void StudentWorld::displayStatus() {
	ostringstream oss;
	oss << "Lives: " << getLives() << "  Health: ";
	int healthPerc = m_nach->getHealth() * 2;
	oss << healthPerc << "%  Score: " << getScore() << "  Level: " << getLevel() << "  Cabbages: ";
	int cabbages = (m_nach->getCabbage() * 100) / 30;
	oss << cabbages << "%  Torpedoes: " << m_nach->getTorpedo();
//	oss << "Bad Killed:" << m_numVillainsDestroyed; //REMOVE ONLY FOR DEBUGGIN PURPOSES
	string output = oss.str();
	setGameStatText(output);
}

//check to see if items can collide based on type
//process damage and also add sounds
void StudentWorld::collisionOccur(Actor* p1) {
	vector<Actor*>::iterator it = m_v.begin();
	while (it != m_v.end()) {
		if (p1->type() != (*it)->type() && collisionDistance(p1, *it)) {
			Actor* a = *it;
			int health = a->getHealth();
			a->setHealth(health - p1->getDamage());
			if (p1->type() == 3)
				p1->changeState();
			if (p1->type() == 4 && a == m_nach) {
				increaseScore(100);
				p1->changeState();
				p1->action();
				playSound(SOUND_GOODIE);
				return;
			}
			if (a->getHealth() > 0 && a->damageable())
				playSound(SOUND_BLAST);
			else if (a->getHealth() < 0 && a->damageable())
			{
				if (a->isEvil())
					killVillain();
				playSound(SOUND_DEATH);
				increaseScore(a->score());
			}
		}
		it++;
	}
}

//check to see if items are close enough for a collision to occur
bool StudentWorld::collisionDistance(Actor* p1, Actor* p2) const {
		if (p1->getState() && p2->getState()) {
			double x1 = p1->getX();
			double y1 = p1->getY();
			double r1 = p1->getRadius();
			double r2 = p2->getRadius();
			double x2 = p2->getX();
			double y2 = p2->getY();
			double distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
			if (distance < 0.75*(r1 + r2) && p2->collidable() && p2->type() != 0 && p1->type() != 0 && p1->getState() && p2->getState())
				return true;
		}
		return false;
}
