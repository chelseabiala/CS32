#include "Sequence.h"
#include <iostream>
using namespace std;

Sequence::Sequence()
 : m_size(0)
{
    m_head = new Node();    // Head begins as an arbitrary node
    m_head->next = nullptr;
    m_head->prev = nullptr;
}

Sequence::Sequence(const Sequence& s)
{
    m_size = s.m_size;
    m_head = new Node();
    m_head->val = s.m_head->val;       // Copies head node
    m_head->next = s.m_head->next;
    Node* n1 = m_head; Node* n2 = s.m_head->next;
    while(n2 != nullptr)    // Copies all other nodes in list
    {
        Node* newNode = new Node();
        newNode->val = n2->val;
        n1->next = newNode;
        newNode->prev = n1;
        n2 = n2->next;
        n1 = n1->next;
    }
    n1->next = nullptr;
}

Sequence::~Sequence()
{
    Node* p = m_head;
    while(p != nullptr)     // Destroys each individual node
    {
        Node* temp = p;
        p = p->next;
        delete temp;
    }
}

Sequence& Sequence::operator=(const Sequence& n)
{
    if(this != &n)
    {
        Sequence temp(n);
        swap(temp);
    }
    return *this;
}

int Sequence::size() const
{
    return m_size;
}

bool Sequence::empty() const
{
    return size() == 0;
}

int Sequence::insert(int pos, const ItemType& value)
{
    if (pos < 0  ||  pos > size())
        return -1;
    uncheckedInsert(pos, value);
    return pos;
}

int Sequence::insert(const ItemType& value)
{
    Node* p = m_head;
    int pos = 0;
    for (int i = 0; i < size()  &&  value > p->val; i++)  // Find where value fits
    {
        p = p->next;
        pos = i+1;
    }
    uncheckedInsert(pos, value);
    return pos;
}
     
bool Sequence::erase(int pos)
{
    if (pos < 0  ||  pos >= size())
        return false;
    if(pos == 0)        // Deleting first node in sequence
    {
        Node* toBeDeleted = m_head;
        m_head = m_head->next;
        if(m_size != 1)     // If deleting first node of multiple
            m_head->prev = nullptr;
        else    //If deleting only node in sequence
        {
            m_head = new Node();
            m_head->next = nullptr;
            m_head->prev = nullptr;
        }
        delete toBeDeleted;
    }
    else
    {
        Node* p = m_head;
        for(int i = 0; i < pos; i++)        // Find node to be deleted
        {
            p = p->next;
        }
        Node* toBeDeleted = p;
        p->prev->next = p->next;
        if(p->next != nullptr)
            p->next->prev = p->prev;
        delete toBeDeleted;     // Delete said node
    }
    m_size--;
    return true;
}

int Sequence::remove(const ItemType& value)
{
    int keepPos = find(value);  // Find position of node to be deleted
    if (keepPos == -1)
        return 0;
    int count = 0;
    while(keepPos != -1)    // Until there are no more nodes to delete
    {
        Node* p = m_head;
        for(int i = 0; i < keepPos; i++)
            p = p->next;
        erase(keepPos);     // Delete the node with that value
        keepPos = find(value);
        count++;
    }
    return count;
}
     
bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0  ||  pos >= size())
        return false;
    Node* p = m_head;
    for(int i = 0; i < pos; i++)        // Find node at position
        p = p->next;
    value = p->val;     // Assign value
    return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0  ||  pos >= size())
        return false;
    Node* p = m_head;
    for(int i = 0; i < pos; i++)        // Find node at position
        p = p->next;
    p->val = value;     // Change value of node
    return true;
}

int Sequence::find(const ItemType& value) const
{
    Node* p = m_head;
    for (int pos = 0; pos < size(); pos++)      // Traverse elements
    {
        if (p->val == value)
            return pos;
        p = p->next;
    }
    return -1;
}

void Sequence::swap(Sequence& other)
{
      // Swap elements.  Since the only elements that matter are those up to
      // m_size and other.m_size, only they have to be moved.

    Node* tempHead = m_head;
    m_head = other.m_head;
    other.m_head = tempHead;

      // Swap sizes

    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

void Sequence::uncheckedInsert(int pos, const ItemType& value)
{
    if(m_size == 0)     // Insert if list is empty
        m_head->val = value;
    else
    {
        Node* newNode = new Node();
        newNode->val = value;
        if(pos == 0)        // Insert at beginning of list
        {
            m_head->prev = newNode;
            newNode->next = m_head;
            m_head = newNode;
        }
        else
        {
            Node* p = m_head;
            for(int i = 0; i < pos-1; i++)      // Find position at which to insert
                p = p->next;
            if(pos == m_size)       // Insert at end of list
            {
                p->next = newNode;
                newNode->prev = p;
                newNode->next = nullptr;
            }
            else        // Insert in middle of list
            {
                p->next->prev = newNode;
                newNode->prev = p;
                newNode->next = p->next;
                p->next = newNode;
            }
        }
    }
    m_size++;
}

void Sequence::dump() const
{
    Node* p = m_head;
    while(p != nullptr)
    {
        cerr << p->val;
        p = p->next;
    }
    cerr << endl;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    if(seq2.empty() || seq1.empty())    // If both lists are empty
        return -1;
    int pos = -1; bool done = false;
    for(int i = 0; i < seq1.size(); i++)
    {
        for(int j = 0; j < seq2.size();)
        {
            ItemType s1; bool one = seq1.get(i,s1);
            ItemType s2; bool two = seq2.get(j,s2);
            if(s1 == s2 && one && two)      // Finds matches between two sequences
            {
                pos = i;
                i++; j++;       // Advances through s2
            }
            else
            {
                pos = -1;
                break;
            }
            if(j == seq2.size())
                done = true;
        }
        if(done)
            break;
    }
    if(pos == -1)
        return pos;
    else
        return pos - seq2.size() + 1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    while(!result.empty())      // Makes result empty
    {
        result.erase(0);
    }
    if(seq1.empty() && seq2.empty())        // Returns empty result if both are empty
        return;
    if(seq1.size() >= seq2.size())
    {
        int pos = 0; int count1 = 0; int count2 = 0;
        for(int i = seq2.size() - 1; i >= 0; i--)       // Puts seq1 into result
        {
            ItemType val;
            seq1.get(i, val);
            result.insert(0, val); pos++;
            result.insert(0, val); pos++;
            count1++; count2++;
        }
        pos = 1;
        for(int i = 0; i < seq2.size(); i++)        // Puts seq2 in between seq1
        {
            ItemType val;
            seq2.get(i, val);
            result.set(pos, val); pos+=2; count2++;
        }
        for(int i = seq1.size() - 1; i >= count1; i--)  // Puts rest of seq1
        {
            ItemType val;
            seq1.get(i, val);
            result.insert(count2, val);
        }
    }
    else
    {
        int pos = 0; int count1 = 0; int count2 = 0;
        for(int i = seq1.size() - 1; i >= 0; i--)       // Puts seq1 in result
        {
            ItemType val;
            seq1.get(i, val);
            result.insert(0, val); pos++;
            result.insert(0, val); pos++;
            count1++; count2++;
        }
        pos = 1;
        for(int i = 0; i < seq1.size(); i++)    // Puts seq1 in between
        {
            ItemType val;
            seq2.get(i, val);
            result.set(pos, val); pos+=2; count2++;
        }
        for(int i = seq2.size() - 1; i >= count1; i--)      // Puts in rest of seq2
        {
            ItemType val;
            seq2.get(i, val);
            result.insert(count2, val);
        }
    }
}

void Sequence::eraseEvens()
{
    if(m_head == nullptr || m_head->next == nullptr)
        return;
    for(Node* h = m_head; h != nullptr && h->next != nullptr;)
    {
        Node* toBeDeleted = h->next;
        h->next = h->next->next;
        delete toBeDeleted;
        h = h->next;
    }
}
