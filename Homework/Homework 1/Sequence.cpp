
#include "Sequence.h"
#include <iostream>
using namespace std;

Sequence::Sequence()
:m_size(0)
{}

bool Sequence::empty() const
{
    return m_size == 0;
}

int Sequence::size() const
{
    return m_size;
}

int Sequence::insert(int pos, const ItemType& value)
{
    if(pos < 0 || pos > m_size || m_size == DEFAULT_MAX_ITEMS)
        return -1;
    for(int i = m_size; i > pos; i--)
    {
        m_array[i] = m_array[i - 1];
    }
    m_array[pos] = value;
    m_size++;
    return pos;
}

int Sequence::insert(const ItemType& value)
{
    int pos = 0;
    for(int i = 0; i < m_size; i++)
    {
        if(m_array[i] >= value)
            break;
        pos++;
    }
    if(m_size == DEFAULT_MAX_ITEMS)
        return -1;
    insert(pos, value);
    return pos;
}

bool Sequence::erase(int pos)
{
    if(pos < 0 || pos >= m_size)
        return false;
    for(int i = pos; i < m_size - 1; i++)
    {
        m_array[i] = m_array[i + 1];
    }
    m_size--;
    return true;
}

int Sequence::remove(const ItemType& value)
{
    int removed = 0;
    for(int i = 0; i < m_size;)
    {
        if(m_array[i] == value)
        {
            erase(i);
            removed++;
        }
        else
            i++;
    }
    return removed;
}

bool Sequence::get(int pos, ItemType& value) const
{
    if(pos < 0 || pos >= m_size)
        return false;
    value = m_array[pos];
    return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
    if(pos < 0 || pos >= m_size)
        return false;
    m_array[pos] = value;
    return true;
}

int Sequence::find(const ItemType& value) const
{
    for(int i = 0; i < m_size; i++)
    {
        if(m_array[i] == value)
            return i;
    }
    return -1;
}

void Sequence::swap(Sequence& other)
{
    int s;
    if(other.m_size > m_size)
        s = other.m_size;
    else
        s = m_size;
    for(int i = 0; i < s; i++)
    {
        ItemType tempItem = other.m_array[i];
        other.m_array[i] = m_array[i];
        m_array[i] = tempItem;
    }
    int tempSize = other.m_size;
    other.m_size = m_size;
    m_size = tempSize;
}
 

void Sequence::dump() const
{
    for(int i = 0; i < m_size; i++)
    {
        cerr << m_array[i] << " ";
    }
    cerr << endl;
    
}
