#include "ScoreList.h"
#include "Sequence.h"

ScoreList::ScoreList()
{}

bool ScoreList::add(unsigned long score)
{
    if(score < 0 || score > 100 || m_sequence.size() == DEFAULT_MAX_ITEMS)
        return false;
    m_sequence.insert(score);
    return true;
}

bool ScoreList::remove(unsigned long score)
{
    int pos = m_sequence.find(score);
    if(pos == -1)
        return false;
    m_sequence.erase(pos);
    return true;
}

int ScoreList::size() const
{
    return m_sequence.size();
}

unsigned long ScoreList::minimum() const
{
    if(m_sequence.size() == 0)
        return NO_SCORE;
    unsigned long tempMin = 100;
    unsigned long min;
    for(int i = 0; i < m_sequence.size(); i++)
    {
        m_sequence.get(i, min);
        if(tempMin > min)
            m_sequence.get(i, tempMin);
    }
    return tempMin;
}

unsigned long ScoreList::maximum() const
{
    if(m_sequence.size() == 0)
        return NO_SCORE;
    unsigned long tempMax = 0;
    unsigned long max;
    for(int i = 0; i < m_sequence.size(); i++)
    {
        m_sequence.get(i, max);
        if(max > tempMax)
            m_sequence.get(i, tempMax);
    }
    return tempMax;
}
