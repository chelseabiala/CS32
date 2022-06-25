#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED

#include <string>

using ItemType = std::string;

class Sequence
{
  public:
    Sequence();
    Sequence(const Sequence& s);
    ~Sequence();
    Sequence& operator=(const Sequence& n);
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    void dump() const;
    void eraseEvens();
    
  private:
    struct Node
    {
        ItemType val;
        Node* next;
        Node* prev;
    };
    Node* m_head;
    int m_size;
    void uncheckedInsert(int pos, const ItemType& value);
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif
