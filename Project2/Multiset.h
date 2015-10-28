#ifndef MULTISET_INCLUDED
#define MULTISET_INCLUDED

#include <string>

typedef std::string ItemType;

class Multiset
{
 public:
    Multiset();

    bool empty() const;
    int size() const;
    int uniqueSize() const;
    bool insert(const ItemType& value);
    int erase(const ItemType& value);
    int eraseAll(const ItemType& value);
    bool contains(const ItemType& value) const;
    int count(const ItemType& value) const;
    int get(int i, ItemType& value) const;
    void swap(Multiset& other);

	~Multiset();
    Multiset(const Multiset& other);
    Multiset& operator=(const Multiset& rhs);

private:

	// Since this structure is used only by the implementation of the
    // Multiset class, we'll make it private to Multiset.
    struct Node
    {
        ItemType m_value;
        int      m_count;

		Node* m_next;
		Node* m_prev;
    };

	Node* m_head;

    int   m_uniqueSize; // how many distinct items in the multiset
    int   m_size;       // total number of items in the multiset

      // At any time, the elements of m_data indexed from 0 to m_uniqueSize-1
      // are in use.  m_size is the sum of the m_counts of those elements.

    int find(const ItemType& value) const;
      // Return index of the node in m_data whose m_value == value if there is
      // one, else -1

    int doErase(const ItemType& value, bool all);
      // Remove one or all instances of value from the multiset if present,
      // depending on the second parameter.  Return the number of instances
      // removed.
	//----------------------
};

//Other functions
void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result);

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result);

// Inline implementations

inline
int Multiset::size() const
{
    return m_size;
}

inline
int Multiset::uniqueSize() const
{
    return m_uniqueSize;
}

inline
bool Multiset::empty() const
{
    return size() == 0;
}

#endif //MULTISET_INCLUDED