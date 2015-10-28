#include "StudentMultiset.h"
#include <cassert>
using namespace std;

int main()
{
	StudentMultiset list; // Create an empty student multiset.
	assert(list.size() == 0);
	list.print();
	
	assert(list.add(1));
	assert(list.size() == 1);
	assert(list.add(1));
	assert(list.size() == 2);
	assert(list.add(200));
	assert(list.add(200));
	assert(list.add(200));
	assert(list.add(40));
	assert(list.size() == 6);
	//list: {1, 1, 200, 200, 200, 40}
	
	list.print();

	//test default copy constructor
	StudentMultiset list2 = list;
	assert(list2.add(50));
	list2.print();
	list.print();		//test: adding to list2 does not change original list

	//test default assignment operator
	StudentMultiset list3;
	list3 = list2;
	assert(list3.add(75));
	list3.print();
	list2.print();		//test: adding to list3 does not change list2
}