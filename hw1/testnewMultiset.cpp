#include "newMultiset.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
	Multiset a(1000);   // a can hold at most 1000 distinct items
	Multiset b(5);      // b can hold at most 5 distinct items
	Multiset c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
	ItemType v[6] = { 6, 5, 4, 3, 2, 1 };
	// No failures inserting 5 distinct items twice each into b
	for (int k = 0; k < 5; k++)
	{
		assert(b.insert(v[k]));
		assert(b.insert(v[k]));
	}
	assert(b.size() == 10  &&  b.uniqueSize() == 5  &&  b.count(v[0]) == 2);
	// Failure if we try to insert a sixth distinct item into b
	assert(!b.insert(v[5]));

	// When two Multisets' contents are swapped, their capacities are swapped
	// as well:
	a.swap(b);
	assert(!a.insert(v[5])  &&  b.insert(v[5]));

	//Test copy constructor
	Multiset test1;
	Multiset test2 = test1;
	assert(test1.empty() && test2.empty());
	assert(test1.insert(5));
	assert(test1.insert(5));
	assert(!test1.empty() && test1.size() == 2 && test1.uniqueSize() == 1);
	assert(test2.empty() && test2.size() == 0 && test2.uniqueSize() == 0);

	//Test assignment operator
	Multiset test3;
	test3 = test2;
	assert(test3.empty() && test3.size() == 0 && test3.uniqueSize() == 0);
	assert(test2.insert(15));
	assert(test3.empty() && test3.size() == 0 && test3.uniqueSize() == 0); //adding to test2 has no effect on test3
	test3 = test1;
	assert(!test3.empty() && test3.size() == 2 && test3.uniqueSize() == 1);
	
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
}