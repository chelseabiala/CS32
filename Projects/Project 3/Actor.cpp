#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// ACTOR IMPLEMENTATIONS

Actor::Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, double speed, bool c, StudentWorld* sw)
: GraphObject(imageID, startX, startY, dir, size, depth), m_isAlive(true), m_speed(speed), sw(sw)
{}

bool Actor::isAlive() const
{
    return m_isAlive;
}

double Actor::getSpeed() const
{
    return m_speed;
}

void Actor::die()
{
    m_isAlive = false;
}

void Actor::setSpeed(double s)
{
    m_speed = s;
}

StudentWorld* Actor::getSW() const
{
    return sw;
}

bool Actor::isOffScreen() const
{
    return (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT);
}

bool Actor::isCollisionAvoidanceWorthy() const
{
    return m_isCollisionAvoidanceWorthy;
}

// Moves actor a certain distance based on current speed of GhostRacer
void Actor::moveRelativeToGhostRacer(double dx)
{
    double gr_speed = sw->getGR()->getSpeed();
    double vert_speed = m_speed - gr_speed;
    moveTo(getX() + dx, getY() + vert_speed);
    if(isOffScreen())
        die();
}

// Checks if two Actors are overlapping
bool Actor::areOverlapping(const Actor* a) const
{
    double delta_x = abs(getX() - a->getX());
    double delta_y = abs(getY() - a->getY());
    double radius_sum = getRadius() + a->getRadius();
    return (delta_x < radius_sum * 0.25 && delta_y < radius_sum * 0.6);
}

void Actor::getSprayed()
{}

// LIVE ACTOR IMPLEMENTATIONS

LiveActor::LiveActor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, double speed, int hp, bool c, StudentWorld* sw)
: Actor(imageID, startX, startY, dir, size, depth, speed, c, sw), m_hitPoints(hp)
{}

int LiveActor::getHitPoints() const
{
    return m_hitPoints;
}

void LiveActor::changeHP(int h)
{
    m_hitPoints += h;
    if(m_hitPoints <= 0)
        die();
}

// BORDERLINE IMPLEMENTATIONS

Borderline::Borderline(int imageID, double startX, double startY, StudentWorld* sw)
: Actor(imageID, startX, startY, 0, 2, 1, -4, false, sw)
{}

void Borderline::doSomething()
{
    if(!isAlive())
        return;
    moveRelativeToGhostRacer(0);
}

bool Borderline::isSprayable() const
{
    return false;
}

// GHOST RACER IMPLEMENTATIONS

GhostRacer::GhostRacer(StudentWorld* sw)
: LiveActor(IID_GHOST_RACER, 128, 32, 90, 4.0, 0, 0, 100, true, sw), m_holyWaters(10)
{}

int GhostRacer::soundWhenDie()
{
    return SOUND_PLAYER_DIE;
}

int GhostRacer::soundWhenHurt()
{
    return SOUND_VEHICLE_CRASH;
}

int GhostRacer::numSprays() const
{
    return m_holyWaters;
}

void GhostRacer::addSprays(int num)
{
    m_holyWaters += num;
}

bool GhostRacer::isSprayable() const
{
    return false;
}

// Changes GhostRacer's direction when it hits an Oil Slick
void GhostRacer::spin()
{
    int c = randInt(1,2);
    int amount = randInt(5,20);
    if(c == 1)
    {
        if(getDirection() - amount > 60)
            setDirection(getDirection() - amount);
        else
            setDirection(60);
    }
    else
    {
        if(getDirection() + amount < 120)
            setDirection(getDirection() + amount);
        else
            setDirection(120);
    }
}

void GhostRacer::doSomething()
{
    if(!isAlive())
        return;
    if(getX() <= ROAD_CENTER - (ROAD_WIDTH / 2))
    {
        if(getDirection() > 90)
        {
            changeHP(-10);
            setDirection(82);
            getSW()->playSound(soundWhenHurt());
        }
    }
    else if(getX() >= ROAD_CENTER + (ROAD_WIDTH / 2))
    {
        if(getDirection() < 90)
        {
            changeHP(-10);
            setDirection(98);
            getSW()->playSound(soundWhenHurt());
        }
    }
    else
    {
        int key;
        if(getSW()->getKey(key))
        {
            int d;
            switch(key)
            {
                case KEY_PRESS_SPACE:
                    if(m_holyWaters > 0)
                    {
                        d = getDirection() * (M_PI / 180);
                        getSW()->addHolyWater(getX() + SPRITE_HEIGHT * cos(d), getY() + SPRITE_HEIGHT * sin(d), getDirection());
                        getSW()->playSound(SOUND_PLAYER_SPRAY);
                        m_holyWaters--;
                    }
                    break;
                case KEY_PRESS_LEFT:
                    if(getDirection() < 114)
                        setDirection(getDirection() + 8);
                    break;
                case KEY_PRESS_RIGHT:
                    if(getDirection() > 66)
                        setDirection(getDirection() - 8);
                    break;
                case KEY_PRESS_UP:
                    if(getSpeed() < 5)
                        setSpeed(getSpeed() + 1);
                    break;
                case KEY_PRESS_DOWN:
                    if(getSpeed() > -1)
                        setSpeed(getSpeed() - 1);
                    break;
                default:
                    break;
            }
        }
    }
    double max_shift_per_tick = 4.0;
    double direction = (getDirection() * (M_PI/180));
    double delta_x = cos(direction) * max_shift_per_tick;
    double currX = getX(); int currY = getY();
    moveTo(currX + delta_x, currY);
}

// PEDESTRIAN IMPLEMENTATIONS

Pedestrian::Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size)
: LiveActor(imageID, x, y, 0, size, 0, -4, 2, true, sw), m_horizontalSpeed(0), m_movementPlanDistance(0)
{}

int Pedestrian::getHorizSpeed() const
{
    return m_horizontalSpeed;
}

void Pedestrian::setHorizSpeed(int s)
{
    m_horizontalSpeed = s;
}

// Orients pedestrian's random movements
void Pedestrian::moveAndPossiblyPickPlan()
{
    if(!isAlive())
        return;
    moveRelativeToGhostRacer(m_horizontalSpeed);
    m_movementPlanDistance--;
    if(m_movementPlanDistance > 0)
        return;
    int speed = randInt(-3, 3);
    while(speed == 0)
        speed = randInt(-3, 3);
    m_horizontalSpeed = speed;
    m_movementPlanDistance = randInt(4, 32);
    if(m_horizontalSpeed < 0)
        setDirection(180);
    else
        setDirection(0);
}

void Pedestrian::takeDamageAndPossiblyDie(int hp)
{
    changeHP(-hp);
    if(getHitPoints() < 0)
        die();
}

int Pedestrian::soundWhenHurt()
{
    return SOUND_PED_HURT;
}

int Pedestrian::soundWhenDie()
{
    return SOUND_PED_DIE;
}

// HUMAN PEDESTRIAN IMPLEMENTATIONS

HumanPedestrian::HumanPedestrian(StudentWorld* sw, double x, double y)
: Pedestrian(sw, IID_HUMAN_PED, x, y, 2)
{}

void HumanPedestrian::doSomething()
{
    if(!isAlive())
        return;
    if(areOverlapping(getSW()->getGR()))
    {
        getSW()->getGR()->die();
        return;
    }
    moveAndPossiblyPickPlan();
}

void HumanPedestrian::getSprayed()
{
    setHorizSpeed(getHorizSpeed() * -1);
    setDirection(getDirection() + 180);
    getSW()->playSound(soundWhenHurt());
}

bool HumanPedestrian::isSprayable() const
{
    return true;
}

// ZOMBIE PEDESTRIAN IMPLEMENTATIONS

ZombiePedestrian::ZombiePedestrian(StudentWorld* sw, double x, double y)
: Pedestrian(sw, IID_ZOMBIE_PED, x, y, 3), m_ticksBeforeGrunt(0)
{}


// Determines if the pedestiran is close enough to chase GhostRacer
bool ZombiePedestrian::isCloseToGhostRacer() const
{
    bool first = abs(getX() - getSW()->getGR()->getX()) < 30;
    bool second = getY() > getSW()->getGR()->getY();
    return first && second;
}

bool ZombiePedestrian::isSprayable() const
{
    return true;
}

void ZombiePedestrian::doSomething()
{
    if(!isAlive())
        return;
    if(areOverlapping(getSW()->getGR()))
    {
        getSW()->getGR()->changeHP(-5);
        takeDamageAndPossiblyDie(2);
        return;
    }
    if(isCloseToGhostRacer())
    {
        setDirection(270);
        if(getX() < getSW()->getGR()->getX())
            setHorizSpeed(1);
        else if(getX() > getSW()->getGR()->getX())
            setHorizSpeed(-1);
        else
            setHorizSpeed(0);
        m_ticksBeforeGrunt--;
        if(m_ticksBeforeGrunt <= 0)
        {
            getSW()->playSound(SOUND_ZOMBIE_ATTACK);
            m_ticksBeforeGrunt = 20;
        }
    }
    moveAndPossiblyPickPlan();
}

void ZombiePedestrian::getSprayed()
{
    changeHP(-1);
    if(getHitPoints() <= 0)
    {
        die();
        getSW()->playSound(soundWhenDie());
        if(!areOverlapping(getSW()->getGR()))
        {
            int chance = randInt(1, 5);
            if(chance == 1)
            {
                getSW()->addHealingGoodie(getX(), getY());
            }
            getSW()->increaseScore(150);
        }
    }
}

// ZOMBIE CAB IMPLEMENTATIONS

ZombieCab::ZombieCab(StudentWorld* sw, double x, double y, double speed)
: LiveActor(IID_ZOMBIE_CAB, x, y, 90, 4.0, 0, speed, 3, true, sw), m_planLength(0), m_hasDamagedGhostRacer(false)
{}

int ZombieCab::soundWhenHurt()
{
    return SOUND_VEHICLE_HURT;
}

int ZombieCab::soundWhenDie()
{
    return SOUND_VEHICLE_DIE;
}

int ZombieCab::getHorizSpeed() const
{
    return m_horizSpeed;
}

void ZombieCab::setHorizSpeed(int h)
{
    m_horizSpeed = h;
}

bool ZombieCab::isSprayable() const
{
    return true;
}

void ZombieCab::doSomething()
{
    if(!isAlive())
        return;
    if(areOverlapping(getSW()->getGR()))
    {
        if(!m_hasDamagedGhostRacer)
        {
            getSW()->playSound(SOUND_VEHICLE_CRASH);
            getSW()->getGR()->changeHP(-20);
            if(getX() <= getSW()->getGR()->getX())
            {
                setHorizSpeed(-5);
                setDirection(120 + randInt(0, 19));
            }
            else
            {
                setHorizSpeed(5);
                setDirection(60 - randInt(0, 19));
            }
            m_hasDamagedGhostRacer = true;
        }
        
    }
    if(!m_hasDamagedGhostRacer)
        moveRelativeToGhostRacer(0); // Possible should be getHorizSpeed
    else
        moveRelativeToGhostRacer(getHorizSpeed());
    int distance;
    bool isInFront;
    bool isGhostRacer;
    if(getSpeed() > getSW()->getGR()->getSpeed() && getSW()->collisionAvoidanceWorthyActorInLane(this, distance, isInFront, isGhostRacer) && isInFront)
    {
        if(distance < 96)
        {
            setSpeed(getSpeed() - 0.5);
            return;
        }
    }
    else if(getSpeed() <= getSW()->getGR()->getSpeed() && getSW()->collisionAvoidanceWorthyActorInLane(this, distance, isInFront, isGhostRacer) && !isInFront)
    {
        if(distance < 96 && !isGhostRacer)
        {
            setSpeed(getSpeed() + 0.5);
            return;
        }
    }
    m_planLength--;
    if(m_planLength > 0)
        return;
    m_planLength = randInt(4, 32);
    setSpeed(getSpeed() + randInt(-2, 2));
}

void ZombieCab::getSprayed()
{
    changeHP(-1);
    if(getHitPoints() < 0)
    {
        die();
        getSW()->playSound(soundWhenDie());
        int chance = randInt(0, 4);
        if(chance == 0)
        {
            getSW()->addOilSlick(getX(), getY());
        }
        getSW()->increaseScore(200);
        return;
    }
    getSW()->playSound(soundWhenHurt());
}

// GHOSTRACERACTIVATEDOBJECT IMPLEMENTATIONS

GhostRacerActivatedObject::GhostRacerActivatedObject(int imageID, double startX, double startY, int dir, double size, StudentWorld* sw)
: Actor(imageID, startX, startY, dir, size, 2, -4, false, sw)
{}

void GhostRacerActivatedObject::doSomething()
{
    if(!isAlive())
        return;
    moveRelativeToGhostRacer(0);
    doActivity(getSW()->getGR());
    if(areOverlapping(getSW()->getGR()))
    {
        getSW()->increaseScore(getScoreIncrease());
        getSW()->playSound(getSound());
        if(selfDestructs())
            die();
    }
}

// SOUL GOODIE IMPLEMENTATIONS

SoulGoodie::SoulGoodie(double startX, double startY, StudentWorld* sw)
: GhostRacerActivatedObject(IID_SOUL_GOODIE, startX, startY, 0, 4.0, sw)
{}

// SoulGoodie's specific activity when overlapping with GhostRacer
void SoulGoodie::doActivity(GhostRacer* gr)
{
    if(areOverlapping(gr))
        getSW()->decSouls();
    setDirection(getDirection() - 10);
}

int SoulGoodie::getScoreIncrease() const
{
    return 100;
}

int SoulGoodie::getSound() const
{
    return SOUND_GOT_SOUL;
}

bool SoulGoodie::selfDestructs() const
{
    return true;
}

bool SoulGoodie::isSprayable() const
{
    return false;
}

// HEALING GOODIE IMPLEMENTATIONS

HealingGoodie::HealingGoodie(double startX, double startY, StudentWorld* sw)
: GhostRacerActivatedObject(IID_HEAL_GOODIE, startX, startY, 0, 1.0, sw)
{}

// HealingGoodie's specific activity when overlapping with GhostRacer
void HealingGoodie::doActivity(GhostRacer* gr)
{
    if(areOverlapping(gr))
    {
        if(getSW()->getGR()->getHitPoints() + 10 < 100)
            getSW()->getGR()->changeHP(10);
    }
}

int HealingGoodie::getSound() const
{
    return SOUND_GOT_GOODIE;
}

int HealingGoodie::getScoreIncrease() const
{
    return 250;
}

bool HealingGoodie::selfDestructs() const
{
    return true;
}

bool HealingGoodie::isSprayable() const
{
    return true;
}

void HealingGoodie::getSprayed()
{
    die();
}

// HOLY WATER GOODIE IMPLEMENTATIONS

HolyWaterGoodie::HolyWaterGoodie(StudentWorld* sw, double x, double y)
: GhostRacerActivatedObject(IID_HOLY_WATER_GOODIE, x, y, 90, 2, sw)
{}

// HolyWaterGoodie's specific activity when overlapping with GhostRacer
void HolyWaterGoodie::doActivity(GhostRacer *gr)
{
    if(areOverlapping(gr))
        gr->addSprays(10);
}

void HolyWaterGoodie::getSprayed()
{
    die();
}

int HolyWaterGoodie::getScoreIncrease() const
{
    return 50;
}

bool HolyWaterGoodie::selfDestructs() const
{
    return true;
}

bool HolyWaterGoodie::isSprayable() const
{
    return true;
}

int HolyWaterGoodie::getSound() const
{
    return SOUND_GOT_GOODIE;
}

// OIL SLICK IMPLEMENTATIONS

OilSlick::OilSlick(StudentWorld* sw, double x, double y)
: GhostRacerActivatedObject(IID_OIL_SLICK, x, y, 0, randInt(2,4), sw)
{}

// OilSlick's specific activity when overlapping with GhostRacer
void OilSlick::doActivity(GhostRacer* gr)
{
    if(areOverlapping(gr))
        gr->spin();
}

int OilSlick::getScoreIncrease() const
{
    return 0;
}

int OilSlick::getSound() const
{
    return SOUND_OIL_SLICK;
}

bool OilSlick::selfDestructs() const
{
    return false;
}

bool OilSlick::isSprayable() const
{
    return false;
}

// SPRAY IMPLEMENTATIONS

Spray::Spray(StudentWorld* sw, double x, double y, int dir)
: Actor(IID_HOLY_WATER_PROJECTILE, x, y, dir, 1, 1, 0, false, sw), m_maxTravelDistance(160)
{}

void Spray::doSomething()
{
    if(!isAlive())
        return;
    Actor* a = getSW()->holyWaterActivated(this);
    if(a != nullptr)
    {
        a->getSprayed();
        die();
        return;
    }
    for(int i = 0; i < SPRITE_HEIGHT; i++)
    {
        moveForward();
        m_maxTravelDistance --;
    }
    if(isOffScreen())
        die();
    if(m_maxTravelDistance <= 0)
        die();
}

bool Spray::isSprayable() const
{
    return false;
}
