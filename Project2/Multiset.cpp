#include "Multiset.h"

void exchange(int& a, int& b)
{
    int t = a;
    a = b;
    b = t;
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	result = ms1;
	for (int k = 0; k < ms2.uniqueSize(); k++) //loop through ms2 and insert into result
    {
		ItemType x;
        int n = ms2.get(k, x);
		for (int count = n; count > 0; count--) //insert all iterations of value
			result.insert(x);
    }
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	
	Multiset ms2_copy = ms2; //to avoid aliasing problems: for example, if result = ms2

	result = ms1;
	for (int m1 = 0; m1 < result.uniqueSize(); m1++)
	{
		ItemType m1_val;
		int n1 = result.get(m1, m1_val);
		for (int m2 = 0; m2 < ms2_copy.uniqueSize(); m2++)
		{
			ItemType m2_val;
			int n2 = ms2_copy.get(m2, m2_val);
			if (m1_val == m2_val)
			{
				if (n1 > n2)
					for (int count = n2; count > 0; count--) //delete n2 iterations of duplicate item
						result.erase(m1_val);
				else //if n1 <= n2, then delete entire node
				{
					result.eraseAll(m1_val); //if n2 > n1, then remove value completely from result Multiset
					m1--; //if entire node removed, then don't move cursor (otherwise, loop would skip next node)
				}
				break;
			}
		}
	}
}

Multiset::Multiset()
 : m_uniqueSize(0), m_size(0)
{
	m_head = new Node; //create a dummy node
	m_head->m_next = m_head;
	m_head->m_prev = m_head;
}

Multiset::Multiset(const Multiset& other)
 : m_uniqueSize(0), m_size(0)
{
	m_head = new Node; //create a dummy node
	m_head->m_next = m_head;
	m_head->m_prev = m_head;

	for (Node* p = other.m_head->m_next; p != other.m_head; p=p->m_next)
	{
		for (int count = p->m_count; count > 0; count--) //copy over each node m_count times
			insert(p->m_value);
	}
}

Multiset& Multiset::operator=(const Multiset& rhs)
{
    if (this != &rhs)
    {
        Multiset temp(rhs);
        swap(temp);
    }
    return *this;
}

Multiset::~Multiset()
{
	Node* temp = m_head->m_next;
	while (temp!=m_head)
	{
		Node* next = temp->m_next;
		delete temp;
		temp=next;
	}
	delete m_head;
}

bool Multiset::insert(const ItemType& value)
{
	Node* p;
	//loop through list to search for value
	for (p = m_head->m_next; p != m_head && p->m_value != value; p=p->m_next)
		;
	if (p==m_head) //either empty list or no value in list
	{
		p = new Node;
		p->m_value = value;
		p->m_count = 1;
		p->m_next = m_head;
		p->m_prev = m_head->m_prev; //point m_prev to what used to be the last node
		p->m_prev->m_next = p; //connect p to previous
		m_head->m_prev = p; //make p the last item

		m_uniqueSize++;
	}
	else //if value found, just add to counter
		p->m_count++;
	m_size++;
	return true;
}

int Multiset::erase(const ItemType& value)
{
	Node* p;
    //loop through list to search for value
	for (p = m_head->m_next; p != m_head && p->m_value != value; p=p->m_next)
		;
	if (p==m_head) //not found
		return 0;

	//erase a node
	if(p->m_count == 1)
	{
		p->m_prev->m_next = p->m_next;
		p->m_next->m_prev = p->m_prev;

		delete p;
		m_uniqueSize--;
	}

	//decrement 1 if value found and has > 1 instances
	else
		p->m_count--;

	m_size--;
	return 1;
}

int Multiset::eraseAll(const ItemType& value)
{
    Node* p;
    //loop through list to search for value
	for (p = m_head->m_next; p != m_head && p->m_value != value; p=p->m_next)
		;
	if (p==m_head) //not found
		return 0;

	//delete a node
	p->m_prev->m_next = p->m_next;
	p->m_next->m_prev = p->m_prev;
	
	int number = p->m_count; //number of instances of the item in node
	m_size -= number; //remove all instances
	m_uniqueSize--;
	delete p;
	return number;
}

int Multiset::count(const ItemType& value) const
{
    Node* p;
	//loop through list to search for value
	for (p = m_head->m_next; p != m_head && p->m_value != value; p=p->m_next)
		;
	if (p==m_head) //either empty list or no value in list
		return 0;
	else
		return p->m_count;
}

int Multiset::get(int i, ItemType& value) const
{
    if (i < 0  ||  i >= m_uniqueSize)
        return 0;
	Node* p;
	int k = 0;
	for (p = m_head->m_next; p != m_head && k < i; p=p->m_next, k++)
		;
    value = p->m_value;
    return p->m_count;
}

void Multiset::swap(Multiset& other)
{
	Node* t = m_head;
	m_head = other.m_head;
	other.m_head = t;

    exchange(m_uniqueSize, other.m_uniqueSize);
    exchange(m_size, other.m_size);
}

bool Multiset::contains(const ItemType& value) const
{
	Node* p;
	//loop through list to search for value
	for (p = m_head->m_next; p != m_head && p->m_value != value; p=p->m_next)
		;
	if (p==m_head) //either empty list or no value in list
		return false;
	return true;
}