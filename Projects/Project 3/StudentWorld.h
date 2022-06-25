#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

using namespace std; // Is this allowed?

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
    public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setBorderY(int y);
    int getBorderY() const;
    GhostRacer* getGR() const;
    void decSouls();
    int calcStartX();
    bool willInsert(int chance);
    int getSoulCount() const;
    int getBonus() const;
    void addHealingGoodie(int x, int y);
    int getLane(Actor* a);
    bool collisionAvoidanceWorthyActorInLane(Actor* a, int& d, bool& isInFront, bool& isGhostRacer);
    void addOilSlick(int x, int y);
    bool addZombieCab(int& zCabX, int& zCabY, int& speed);
    Actor* holyWaterActivated(Actor* a);
    void addHolyWater(int x, int y, int dir);

    private:
    GhostRacer* m_ghostRacer;
    list<Actor*> m_actors;
    int m_border_y;
    int m_soulsToSave;
    int m_bonus;
};

#endif // STUDENTWORLD_H_
