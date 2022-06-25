#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_bonus(5000)
{
    m_ghostRacer = nullptr;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::setBorderY(int y)
{
    m_border_y = y;
}

int StudentWorld::getBorderY() const
{
    return m_border_y;
}

GhostRacer* StudentWorld::getGR() const
{
    return m_ghostRacer;
}

void StudentWorld::decSouls()
{
    m_soulsToSave--;
}

// Gives actors random starting X coordinates
int StudentWorld::calcStartX()
{
    return randInt(ROAD_CENTER - (ROAD_WIDTH / 2), ROAD_CENTER + (ROAD_WIDTH / 2));
}

// Calculates chances of inserting Actors in each tick
bool StudentWorld::willInsert(int chance)
{
    return randInt(0, chance) == 0;
}

int StudentWorld::getSoulCount() const
{
    return m_soulsToSave;
}

int StudentWorld::getBonus() const
{
    return m_bonus;
}

void StudentWorld::addHealingGoodie(int x, int y)
{
    m_actors.push_front(new HealingGoodie(x, y, this));
}

void StudentWorld::addOilSlick(int x, int y)
{
    m_actors.push_front(new OilSlick(this, x, y));
}

// Determines which lane Actor a is in
int StudentWorld::getLane(Actor* a)
{
    double LEFT_EDGE = ROAD_CENTER - (ROAD_WIDTH / 2);
    double RIGHT_EDGE = ROAD_CENTER + (ROAD_WIDTH / 2);
    double LANE_WIDTH = (ROAD_WIDTH / 3);
    if(a->getX() > LEFT_EDGE && a->getX() < LEFT_EDGE + LANE_WIDTH)
        return 1;
    else if(a->getX() > LEFT_EDGE && a->getX() < LEFT_EDGE + 2 * LANE_WIDTH)
        return 2;
    else if(a->getX() < RIGHT_EDGE && a->getX() < RIGHT_EDGE - LANE_WIDTH)
        return 3;
    return 0;
}

// Determines whether there is a collision avoidance worthy actor in the lane of Actor a
bool StudentWorld::collisionAvoidanceWorthyActorInLane(Actor* a, int& d, bool& isInFront, bool& isGhostRacer)
{
    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->isCollisionAvoidanceWorthy() && getLane(a) == getLane(*it))
        {
            d = abs(a->getY() - (*it)->getY());
            isInFront = ((*it)->getY() > a->getY());
            isGhostRacer = false;
            return true;
        }
    }
    if(getLane(m_ghostRacer) == getLane(a))
    {
        d = abs(a->getY() - (m_ghostRacer)->getY());
        isInFront = ((m_ghostRacer)->getY() > a->getY());
        isGhostRacer = true;
        return true;
    }
    return false;
}

// Determines starting position of new Zombie Cabs and constructs them
bool StudentWorld::addZombieCab(int &zCabX, int &zCabY, int& speed)
{
    list<Actor*> lane1Actors; list<Actor*> lane2Actors; list<Actor*> lane3Actors;
    int minDistanceToBottom = VIEW_HEIGHT; Actor* closestActorToBottom = nullptr; int chosenLane = 0;
    Actor* closestActorToTop = nullptr; int minDistanceToTop = 0;
    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if(getLane(*it) == 1)
            lane1Actors.push_front(*it);
        else if(getLane(*it) == 2)
            lane2Actors.push_front(*it);
        else if(getLane(*it) == 3)
            lane3Actors.push_front(*it);
    }
    int cur_lane = randInt(1, 3);
    for(int i = cur_lane; i < cur_lane + 3; i++)
    {
        list<Actor*> iteratedList;
        if(i % 3 == 1)
            iteratedList = lane1Actors;
        else if(i % 3 == 2)
            iteratedList = lane2Actors;
        else if(i % 3 == 0)
            iteratedList = lane3Actors;
        for(list<Actor*>::iterator it = iteratedList.begin(); it != iteratedList.end(); it++)
        {
            if((*it)->isCollisionAvoidanceWorthy() && (*it)->getY() < minDistanceToBottom)
            {
                minDistanceToBottom = (*it)->getY();
                closestActorToBottom = *it;
            }
            else if((*it)->isCollisionAvoidanceWorthy() && (*it)->getY() > minDistanceToTop)
            {
                minDistanceToTop = (*it)->getY(); // Not actually, just highest y
                closestActorToTop = *it;
            }
        }
        if(closestActorToBottom == nullptr || minDistanceToBottom > VIEW_HEIGHT / 3)
        {
            if(i % 3 == 0)
                chosenLane = 3;
            else
                chosenLane = cur_lane % 3;
            zCabY = SPRITE_HEIGHT / 2;
            speed = m_ghostRacer->getSpeed() + randInt(2, 4);
            break;
        }
        else if(closestActorToTop == nullptr || minDistanceToTop < VIEW_HEIGHT * (2/3))
        {
            if(i % 3 == 0)
                chosenLane = 3;
            else
                chosenLane = cur_lane % 3;
            zCabY = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
            speed = m_ghostRacer->getSpeed() - randInt(2, 4);
            break;
        }
    }
    if(chosenLane == 0)
        return false;
    if(chosenLane == 1)
        zCabX = ROAD_CENTER - ROAD_WIDTH / 3;
    else if(chosenLane == 2)
        zCabX = ROAD_CENTER;
    else if(chosenLane == 3)
        zCabX = ROAD_CENTER + ROAD_WIDTH / 3;
    return true;
}

// Determines if Holy Water Projectiles have an effect
Actor* StudentWorld::holyWaterActivated(Actor* a)
{
    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->isSprayable() && a->areOverlapping(*it))
            return *it;
    }
    return nullptr;
}

void StudentWorld::addHolyWater(int x, int y, int dir)
{
    m_actors.push_front(new Spray(this, x, y, dir));
}

int StudentWorld::init()
{
    m_ghostRacer = new GhostRacer(this);
    list<Actor*> l;
    m_actors = l;
    m_soulsToSave = 5 + 2 * getLevel();
    m_border_y = VIEW_HEIGHT;
    double LEFT_EDGE = ROAD_CENTER - (ROAD_WIDTH / 2);
    double RIGHT_EDGE = ROAD_CENTER + (ROAD_WIDTH / 2);
    for(int i = 0; i < VIEW_HEIGHT / SPRITE_HEIGHT; i++)
    {
        m_actors.push_front(new Borderline(IID_YELLOW_BORDER_LINE, LEFT_EDGE, i * SPRITE_HEIGHT, this));
        m_actors.push_front(new Borderline(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, i * SPRITE_HEIGHT, this));
    }
    for(int i = 0; i < VIEW_HEIGHT / (4*SPRITE_HEIGHT) + 1; i++)
    {
        m_actors.push_front(new Borderline(IID_WHITE_BORDER_LINE, LEFT_EDGE + (ROAD_WIDTH/3), i * (4*SPRITE_HEIGHT), this));
        m_actors.push_front(new Borderline(IID_WHITE_BORDER_LINE, RIGHT_EDGE - (ROAD_WIDTH/3), i * (4*SPRITE_HEIGHT), this));
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        (*it)->doSomething();
        if(!m_ghostRacer->isAlive())
        {
            decLives();
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
        if(m_soulsToSave == 0)
        {
            playSound(SOUND_FINISHED_LEVEL);
            increaseScore(m_bonus);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    double vert_speed = -4 - m_ghostRacer->getSpeed();
    m_border_y += vert_speed;
    m_ghostRacer->doSomething();
    
    // Delete the actors that are gone
    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end();)
    {
        if(!((*it)->isAlive()))
        {
            Actor* temp = *it;
            it = m_actors.erase(it);
            delete temp;
        }
        else
            it++;
    }
    
    // Add new border lines
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - m_border_y;
    double LEFT_EDGE = ROAD_CENTER - (ROAD_WIDTH / 2);
    double RIGHT_EDGE = ROAD_CENTER + (ROAD_WIDTH / 2);
    if(delta_y >= SPRITE_HEIGHT)
    {
        m_actors.push_front(new Borderline(IID_YELLOW_BORDER_LINE, LEFT_EDGE, new_border_y, this));
        m_actors.push_front(new Borderline(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, new_border_y, this));
    }
    if(delta_y >= 4 * SPRITE_HEIGHT)
    {
        m_actors.push_front(new Borderline(IID_WHITE_BORDER_LINE, LEFT_EDGE + (ROAD_WIDTH/3), new_border_y, this));
        m_actors.push_front(new Borderline(IID_WHITE_BORDER_LINE, RIGHT_EDGE - (ROAD_WIDTH/3), new_border_y, this));
        m_border_y = (new_border_y);
    }
    
    // Add new items
    if(willInsert(100))
        m_actors.push_front(new SoulGoodie(calcStartX(), VIEW_HEIGHT, this));
    if(willInsert(100 + 10 * getLevel()))
        m_actors.push_front(new HolyWaterGoodie(this, calcStartX(), VIEW_HEIGHT));
    if(willInsert(max(150 - getLevel() * 10, 40)))
        m_actors.push_front(new OilSlick(this, calcStartX(), VIEW_HEIGHT));
    if(willInsert(max(200 - getLevel() * 10, 30)))
        m_actors.push_front(new HumanPedestrian(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
    if(willInsert(max(100 - getLevel() * 10, 20)))
        m_actors.push_front(new ZombiePedestrian(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
    int zCabX = 0; int zCabY = 0; int speed = 0;
    if(willInsert(max(100 - getLevel() * 10, 20)) && addZombieCab(zCabX, zCabY, speed))
        m_actors.push_front(new ZombieCab(this, zCabX, zCabY, speed));
        

    m_bonus--;
    
    ostringstream o;
    o.setf(ios::fixed);
    o.precision(0);
    o << "Score : " << getScore() << " Lvl: " << getLevel() << " Souls2Save : " << getSoulCount() << " Lives: " << getLives() << " Health : " << getGR()->getHitPoints() << " Sprays: " << getGR()->numSprays() << " Bonus: " << getBonus();
    string s = o.str();
    setGameStatText(s);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_ghostRacer;
    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end();)
    {
            Actor* temp = *it;
            it = m_actors.erase(it);
            delete temp;
    }
}
