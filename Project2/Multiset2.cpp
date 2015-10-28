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
	//make sure result is empty
	if (result.size() != 0)
	{
		Multiset temp;
		result = temp;
	}

	for (int m1 = 0; m1 < ms1.uniqueSize(); m1++)
	{
		ItemType m1_val;
		int n1 = ms1.get(m1, m1_val);
		for (int m2 = 0; m2 < ms2.uniqueSize(); m2++)
		{
			ItemType m2_val;
			int n2 = ms2.get(m2, m2_val);

			if (m1_val == m2_val && n1 > n2)
			{
				for (int count = n1-n2; count > 0; count--) //insert n1-n2 number of iterations
					result.insert(m1_val);
			}
		}
	}
}

Multiset::Multiset()
 : m_uniqueSize(0), m_size(0), m_head(nullptr), m_tail(nullptr)
{}

Multiset::Multiset(const Multiset& other)
 : m_uniqueSize(0), m_size(0), m_head(nullptr), m_tail(nullptr) //set head tail or not???
{
	//if (other.m_head == nullptr)
	/*Node* cursor = other.m_head;
	while (cursor != null)
	{
		insert(cursor->m_value);
		cursor=cursor->m_next;
	}*/

	for (Node* p = other.m_head; p != nullptr; p=p->m_next)
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
	/*Node* next;
	for(Node*temp=m_head; temp != nullptr; temp = next)
	{
		next = temp->m_next;
		delete temp;
	}*/

	Node* temp = m_head;
	while (temp!=nullptr)
	{
		Node* next = temp->m_next;
		delete temp;
		temp=next;
	}
}

bool Multiset::insert(const ItemType& value)
{
	Node* p;
	//loop through list to search for value
	for (p = m_head; p != nullptr && p->m_value != value; p=p->m_next)
		;
	if (p==nullptr) //either empty list or no value in list
	{
		p = new Node;
		p->m_value = value;
		p->m_count = 1;
		p->m_next = nullptr;
		p->m_prev = m_tail;

		if (m_head == nullptr)
			m_head = p;
		else
			p->m_prev->m_next = p;
		m_tail=p;
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
	for (p = m_head; p != nullptr && p->m_value != value; p=p->m_next)
		;
	if (p==nullptr) //not found
		return 0;

	//erase a node
	if(p->m_count == 1)
	{
		if (p->m_prev == nullptr) //delete beginning node
		{
			m_head = p->m_next;
			m_head->m_prev = nullptr;
		}
		else if (p->m_next == nullptr) //delete last node
		{
			m_tail = p->m_prev;
			m_tail->m_next = nullptr;
		}
		else //delete middle node
		{
			p->m_prev->m_next = p->m_next;
			p->m_next->m_prev = p->m_prev;
		}
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
	for (p = m_head; p != nullptr && p->m_value != value; p=p->m_next)
		;
	if (p==nullptr) //not found
		return 0;

	//delete a node
	if (p->m_prev == nullptr) //delete beginning node
	{
		m_head = p->m_next;
		m_head->m_prev = nullptr;
	}
	else if (p->m_next == nullptr) //delete last node
	{
		m_tail = p->m_prev;
		m_tail->m_next = nullptr;
	}
	else //delete middle node
	{
		p->m_prev->m_next = p->m_next;
		p->m_next->m_prev = p->m_prev;
	}
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
	for (p = m_head; p != nullptr && p->m_value != value; p=p->m_next)
		;
	if (p==nullptr) //either empty list or no value in list
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
	for (p = m_head; p != nullptr && k < i; p=p->m_next, k++)
		;
    value = p->m_value;
    return p->m_count;
}

void Multiset::swap(Multiset& other)
{
    Node* t1 = m_head;
	Node* t2 = m_tail;
	m_head = other.m_head;
	m_tail = other.m_tail;
	other.m_head = t1;
	other.m_tail = t2;
      // Swap uniqueSize, size, and capacity.

    exchange(m_uniqueSize, other.m_uniqueSize);
    exchange(m_size, other.m_size);
}

bool Multiset::contains(const ItemType& value) const
{
	Node* p;
	//loop through list to search for value
	for (p = m_head; p != nullptr && p->m_value != value; p=p->m_next)
		;
	if (p==nullptr) //either empty list or no value in list
		return false;
	return true;
}

//COPIED OVER:





