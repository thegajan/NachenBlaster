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

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir)
{}

StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init()
{
	//create 30 stars
	m_numVillains = 6 + (4 * getLevel());
	m_numVillainsDestroyed = 0;
	m_nach = new NachenBlaster(this);
	addItem(m_nach);
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
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//make nachenblaster do something
	//make each actor do something
	for (vector<Actor*>::size_type n = 0; n < m_v.size();) {
		m_v[n]->doSomething();
		if (!(m_v[n]->getState())) {
			//if (m_v[n]->isEvil())
			//	m_numVillainsDestroyed++;
			delete m_v[n];
			m_v.erase(m_v.begin() + n);
		}
		else
			n++;
	}
	newItem();
	m_nach->doSomething();
	displayStatus();
	if (m_nach->getHealth() <= 0) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	if (m_numVillainsDestroyed >= m_numVillains) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
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
	//introduce other stuff
	//decide whether to add a new enemy
	int d = m_numVillainsDestroyed;
	int r = m_numVillains - d;
	int m = 4 + (0.5*getLevel());
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

void StudentWorld::displayStatus() {
	ostringstream oss;
	oss << "Lives: " << getLives() << "  Health: ";
	int healthPerc = m_nach->getHealth() * 2;
	oss << healthPerc << "%  Score: " << getScore() << "  Level: " << getLevel() << "  Cabbages: ";
	int cabbages = (m_nach->getCabbage() * 100) / 30;
	oss << cabbages << "%  Torpedoes: " << m_nach->getTorpedo();
	//oss << "Bad Killed:" << m_numVillainsDestroyed; //REMOVE ONLY FOR DEBUGGIN PURPOSES
	string output = oss.str();
	setGameStatText(output);
}