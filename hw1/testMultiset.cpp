#include "Multiset.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
	Multiset ms;
	assert(ms.empty());
	assert(ms.size() == 0  &&  ms.uniqueSize() == 0);
	unsigned long x = 999;
	assert(ms.get(0, x) == 0  &&  x == 999);  // x unchanged by get failure
	assert( ! ms.contains(42));
	ms.insert(42);
	ms.insert(42);
	assert(ms.size() == 2  &&  ms.uniqueSize() == 1);
	assert(ms.get(1, x) == 0  &&  x == 999);  // x unchanged by get failure
	assert(ms.get(0, x) == 2  &&  x == 42);
	ms.insert(20);
	assert(ms.erase(20) == 1 && ms.erase(5) == 0);
	assert(!ms.contains(20) && ms.uniqueSize() == 1 && ms.size() == 2);
	assert(ms.eraseAll(42) == 2);
	assert(!ms.contains(42) && ms.uniqueSize() == 0 && ms.size() == 0);

	//test swap
	Multiset s1;
	s1.insert(1);
	s1.insert(1);
	s1.insert(2);
	s1.insert(5);
	s1.insert(7);
	Multiset s2;
	s2.insert(200);
	s2.insert(500);
	s2.insert(200);
	s1.swap(s2);

	ItemType item1;
    int n1 = s1.get(0, item1);
    assert(item1 == 200  &&  n1 == 2);

	ItemType item4;
	int n4 = s2.get(3, item4);
	assert(item4 == 7 && n4 == 1);
	assert(s1.size() == 3  &&  s1.uniqueSize() == 2);
	assert(s2.size() == 5  &&  s2.uniqueSize() == 4);
	
	/*
	//test strings:
	Multiset ms;
    ms.insert("fennel");
    ms.insert("fennel");
    ms.insert("fenugreek");
    ms.insert("fennel");
    for (int k = 0; k < ms.uniqueSize(); k++)
    {
        string x;
        int n = ms.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

	Multiset ms3;
    ms3.insert("cinnamon");
    ms3.insert("galangal");
    ms3.insert("cinnamon");
    string s1;
    int n1 = ms3.get(1, s1);
    assert((s1 == "cinnamon"  &&  n1 == 2)  ||  (s1 == "galangal"  &&  n1 == 1));
    string s2;
    int n2 = ms3.get(1, s2);
    assert(s2 == s1  &&  n2 == n1);

	Multiset ms1;
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("turmeric");

    Multiset ms2;
    ms2.insert("coriander");
    ms2.insert("cumin");
    ms2.insert("cardamom");
    ms1.swap(ms2);  // exchange contents of ms1 and ms2
    assert(ms1.size() == 3  &&  ms1.count("coriander") == 1  &&
                ms1.count("cumin") == 1  &&  ms1.count("cardamom") == 1);
    assert(ms2.size() == 4  &&  ms2.count("cumin") == 3  &&
                ms2.count("turmeric") == 1);
	*/
	cout << "Passed all tests" << endl;
}