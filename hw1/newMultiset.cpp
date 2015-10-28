#include "newMultiset.h"
#include <cstdlib>

// Create an empty multiset.
Multiset::Multiset(const int& max)
	:m_max(max), m_distinct(0), m_total(0)
{
	if (max < 0)
		exit(1);
	m_data = new Item[max];
}

Multiset::~Multiset()
{
	delete [] m_data;
}

Multiset::Multiset(const Multiset& other)
{
	m_max = other.m_max;
	m_distinct = other.m_distinct;
	m_total = other.m_total;

	m_data = new Item[m_max];
	for (int k = 0; k < m_distinct; k++)
	{
		m_data[k].m_item = other.m_data[k].m_item;
		m_data[k].m_instances = other.m_data[k].m_instances;
	}
}

Multiset& Multiset::operator=(const Multiset& other)
{
	Multiset temp(other);
	swap(temp);
	return *this;
}

// Return true if the multiset is empty, otherwise false.
bool Multiset::empty() const
{
	return (m_total == 0);
}

// Return the number of items in the multiset.  For example, the size
// of a multiset containing "cumin", "cumin", "cumin", "turmeric" is 4.
int Multiset::size() const
{
	return m_total;
}

// Return the number of distinct items in the multiset.  For example,
// the uniqueSize of a multiset containing "cumin", "cumin", "cumin",
// "turmeric" is 2.
int Multiset::uniqueSize() const
{
	return m_distinct;
}

// Insert value into the multiset.  Return true if the value was
// actually inserted.  Return false if the value was not inserted
// (perhaps because the multiset has a fixed capacity and is full).
bool Multiset::insert(const ItemType& value)
{
	for (int k = 0; k < m_distinct; k++)
	{
		if (value == m_data[k].m_item)
		{
			m_data[k].m_instances++; //if match already-existing item, then add to duplicate counter of that item
			m_total++;
			return true;
		}
	}
	//if item not found in array, add new distinct item to the end
	if (m_distinct == m_max)
		return false;
	m_data[m_distinct].m_item = value;
	m_data[m_distinct].m_instances = 1;
	m_distinct++;
	m_total++;
	return true;
}
          
// Remove one instance of value from the multiset if present.
// Return the number of instances removed, which will be 1 or 0.
int Multiset::erase(const ItemType& value)
{
	for (int k = 0; k < m_distinct; k++)
	{
		if (value == m_data[k].m_item)
		{
			m_data[k].m_instances--; //if match already-existing item, then subtract from duplicate counter of that item

			//if subtracting one results in 0 instances of that item, then remove by copying over last item of array
			if (m_data[k].m_instances == 0)
			{
				m_data[k].m_item = m_data[m_distinct-1].m_item;
				m_data[k].m_instances = m_data[m_distinct-1].m_instances;
				m_distinct--;
			}
			m_total--;
			return 1;
		}
	}
	//if item not found in array
	return 0;
}

// Remove all instances of value from the multiset if present.
// Return the number of instances removed.
int Multiset::eraseAll(const ItemType& value)
{
	int instances = 0;
	for (int k = 0; k < m_distinct; k++)
	{
		if (value == m_data[k].m_item)
		{
			instances = m_data[k].m_instances;
			m_total -= instances; //suptract all instances of that Item from the total
			m_data[k].m_item = m_data[m_distinct-1].m_item; //replace this Item in array with last Item
			m_data[k].m_instances = m_data[m_distinct-1].m_instances;
			m_distinct--;
			return instances;
		}
	}
	return instances;
}

// Return true if the value is in the multiset, otherwise false.  
bool Multiset::contains(const ItemType& value) const
{
	for (int k = 0; k < m_distinct; k++)
	{
		if (value == m_data[k].m_item)
			return true;
	}
	return false;
}

// Return the number of instances of value in the multiset.
int Multiset::count(const ItemType& value) const
{
	for (int k = 0; k < m_distinct; k++)
	{
		if (value == m_data[k].m_item)
			return m_data[k].m_instances;
	}
	return 0;
}

// If 0 <= i < uniqueSize(), copy into value an item in the
// multiset and return the number of instances of that item in
// the multiset.  Otherwise, leave value unchanged and return 0.
// (See below for details about this function.)
int Multiset::get(int i, ItemType& value) const
{
	if ( i >= 0 && i < uniqueSize() )
	{
		value = m_data[i].m_item;
		return m_data[i].m_instances;
	}
	return 0;
}

// Exchange the contents of this multiset with the other one.
void Multiset::swap(Multiset& other)
{
	//swap pointers
	Item* temp = m_data;
	m_data = other.m_data;
	other.m_data = temp;

	//swap other data members
	int temp1 = m_distinct;
	int temp2 = m_total;
	int temp3 = m_max;

	m_distinct = other.m_distinct;
	m_total = other.m_total;
	m_max = other.m_max;

	other.m_distinct = temp1;
	other.m_total = temp2;
	other.m_max = temp3;
}