
#include "newSequence.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Sequence::Sequence()
:m_maxItems(DEFAULT_MAX_ITEMS), m_size(0)
{
    m_array = new ItemType[DEFAULT_MAX_ITEMS];
}

Sequence::Sequence(int s)
:m_size(0)
{
    if(s < 0)
    {
        cout << "Invalid value entered";
        exit(1);
    }
    m_maxItems = s;
    m_array = new ItemType[s];
}

Sequence::Sequence(const Sequence& n)
{
    m_size = n.m_size;
    m_maxItems = n.m_maxItems;
    m_array = new ItemType[n.m_maxItems];
    for(int i = 0; i < m_size; i++)
        m_array[i] = n.m_array[i];
}

Sequence& Sequence::operator=(const Sequence& n)
{
    if(this != &n)
    {
        delete m_array;
        m_size = n.m_size;
        m_maxItems = n.m_maxItems;
        m_array = new ItemType[n.m_maxItems];
        for(int i = 0; i < m_size; i++)
            m_array[i] = n.m_array[i];
    }
    return *this;
}

Sequence::~Sequence()
{
    delete[] m_array;
}

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
    if(pos < 0 || pos > m_size || m_size == m_maxItems)
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
    if(m_size == m_maxItems)
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

//Again, do we have to delete the original object at m_array[pos}?

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
    int tempCapacity = other.m_maxItems;
    other.m_maxItems = m_maxItems;
    m_maxItems = tempCapacity;
    int tempSize = other.m_size;
    other.m_size = m_size;
    m_size = tempSize;
    ItemType* temp = other.m_array;
    other.m_array = m_array;
    m_array = temp;
}

void Sequence::dump() const
{
    for(int i = 0; i < m_size; i++)
    {
        cerr << m_array[i] << " ";
    }
    cerr << endl;
}
