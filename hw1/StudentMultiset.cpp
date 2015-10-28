#include "StudentMultiset.h"
#include "Multiset.h" //include?
#include <iostream>
using namespace std;

// Create an empty student multiset.
StudentMultiset::StudentMultiset()			//Is this necessary??
{}

// Add a student id to the StudentMultiset.  Return true if and only
// if the id was actually added.
bool StudentMultiset::add(unsigned long id)
{
	return m_multiset.insert(id);
}

// Return the number of items in the StudentMultiset.  If an id was
// added n times, it contributes n to the size.
int StudentMultiset::size() const
{
	return m_multiset.size();
}

// Print to cout every student id in the StudentMultiset one per line;
// print as many lines for each id as it occurs in the StudentMultiset.
void StudentMultiset::print() const
{
	for (int i = 0; i < m_multiset.uniqueSize(); i++)
	{
		int instances = 0;
		ItemType value;
		instances = m_multiset.get(i, value);
		for (; instances > 0; instances--)
			cout << value << endl;
	}
}
	