#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class GhostRacer;

// ACTOR DECLARATIONS

class Actor : public GraphObject
{
    public:
    Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, double speed, bool c, StudentWorld* sw);
    virtual void doSomething() = 0;
    bool isAlive() const;
    double getSpeed() const;
    void die();
    void setSpeed(double s);
    StudentWorld* getSW() const;
    bool isOffScreen() const;
    bool areOverlapping(const Actor* a) const;
    bool isCollisionAvoidanceWorthy() const;
    virtual void getSprayed();
    void moveRelativeToGhostRacer(double dx);
    virtual bool isSprayable() const = 0;

    private:
    bool m_isAlive;
    double m_speed;
    bool m_isCollisionAvoidanceWorthy;
    
    StudentWorld* sw;
};

// LIVE ACTOR DECLARATIONS

class LiveActor : public Actor
{
    public:
    LiveActor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, double speed, int hp, bool c, StudentWorld* sw);
    int getHitPoints() const;
    void changeHP(int h);
    virtual int soundWhenHurt() = 0;
    virtual int soundWhenDie() = 0;
    
    private:
    int m_hitPoints;
};

// GHOSTRACER DECLARATIONS

class GhostRacer : public LiveActor
{
    public:
    GhostRacer(StudentWorld* sw);
    void doSomething();
    int soundWhenDie();
    int soundWhenHurt();
    int numSprays() const;
    void addSprays(int num);
    void spin();
    bool isSprayable() const;
    
    private:
    int m_holyWaters;
};

// PEDESTRIAN DECLARATIONS

class Pedestrian : public LiveActor
{
    public:
    Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size);
    int soundWhenHurt();
    int soundWhenDie();
    int getHorizSpeed() const;
    void setHorizSpeed(int s);
    void moveAndPossiblyPickPlan();
    void takeDamageAndPossiblyDie(int hp);
    
    private:
    int m_horizontalSpeed;
    int m_movementPlanDistance;
};

// HUMAN PEDESTRIAN DECLARATIONS

class HumanPedestrian : public Pedestrian
{
    public:
    HumanPedestrian(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    void getSprayed();
    bool isSprayable() const;
};

// ZOMBIE PEDESTRIAN DECLARATIONS

class ZombiePedestrian : public Pedestrian
{
    public:
    ZombiePedestrian(StudentWorld* sw, double x, double y);
    virtual void doSomething();
    virtual void getSprayed();
    bool isCloseToGhostRacer() const;
    bool isSprayable() const;
    
    private:
    int m_ticksBeforeGrunt;
};

// ZOMBIE CAB DECLARATIONS

class ZombieCab : public LiveActor
{
    public:
    ZombieCab(StudentWorld* sw, double x, double y, double speed);
    virtual void doSomething();
    virtual void getSprayed();
    int getHorizSpeed() const;
    void setHorizSpeed(int h);
    int soundWhenHurt();
    int soundWhenDie();
    bool isSprayable() const;
    
    private:
    int m_horizSpeed;
    int m_planLength;
    bool m_hasDamagedGhostRacer;
};

// GHOSTRACERACTIVATEDOBJECT DECLARATIONS

class GhostRacerActivatedObject : public Actor
{
    public:
    GhostRacerActivatedObject(int imageID, double startX, double startY, int dir, double size, StudentWorld* sw);
    void doSomething();
    virtual void doActivity(GhostRacer* gr) = 0;
    virtual int getScoreIncrease() const = 0;
    virtual int getSound() const = 0;
    virtual bool selfDestructs() const = 0;
    
    private:
    int m_scoreIncrease;
};

// BORDERLINE DECLARATIONS

class Borderline : public Actor
{
    public:
    Borderline(int imageID, double startX, double startY, StudentWorld* sw);
    void doSomething();
    void hitRacer(int points);
    bool isSprayable() const;
};

// SOUL GOODIE DECLARATIONS

class SoulGoodie : public GhostRacerActivatedObject
{
    public:
    SoulGoodie(double startX, double startY, StudentWorld* sw);
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

// HEALING GOODIE DECLARATIONS

class HealingGoodie : public GhostRacerActivatedObject
{
    public:
    HealingGoodie(double startX, double startY, StudentWorld* sw);
    virtual int getSound() const;
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    void getSprayed();
};

// HOLY WATER GOODIE DECLARATIONS

class HolyWaterGoodie : public GhostRacerActivatedObject
{
    public:
    HolyWaterGoodie(StudentWorld* sw, double x, double y);
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
    virtual int getSound() const;
    void getSprayed();
};

// OIL SLICK DECLARATIONS

class OilSlick : public GhostRacerActivatedObject
{
    public:
    OilSlick(StudentWorld* sw, double x, double y);
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

// SPRAY DECLARATIONS


class Spray : public Actor
{
    public:
    Spray(StudentWorld* sw, double x, double y, int dir);
    virtual void doSomething();
    bool isSprayable() const;
    
    private:
    int m_maxTravelDistance;
};

#endif


