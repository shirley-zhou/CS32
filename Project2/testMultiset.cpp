/*
#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{

Multiset a;
a.insert("This");
a.insert("is");
a.insert("a");
a.insert("test");
a.insert("test");
//a: This, is, a, test, test

assert(a.size() == 5 && a.uniqueSize() == 4);
cout << "Test a: This, is, a, test, test" << endl;
for (int k = 0; k < a.uniqueSize(); k++)
    {
        string x;
        int n = a.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

Multiset b;
//empty

assert(b.size() == 0 && b.uniqueSize() == 0);
assert(b.count("hi") == 0 && b.contains("hi") == false);
cout << "Test b: (empty)" << endl;
	for (int k = 0; k < b.uniqueSize(); k++)
	{
        string x = "empty";
        int n = b.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

//test copy empty
Multiset d = b;
assert(d.size() == 0 && d.uniqueSize() == 0);
assert(d.count("hi") == 0 && d.contains("hi") == false);

Multiset c=a; //copy
assert(c.size() == 5 && c.uniqueSize() == 4);
c.insert("hi");
//c: This, is, a, test, test, hi

assert(c.size() == 6 && c.uniqueSize() == 5);
//print c, print a (inserting in c does not affect a)
cout << "Test c: This, is, a, test, test, hi" << endl;
for (int k = 0; k < c.uniqueSize(); k++)
	{
        string x;
        int n = c.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }
cout << "Changing c does not change a: This, is, a, test, test" << endl;
for (int k = 0; k < a.uniqueSize(); k++)
	{
        string x;
        int n = a.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

//test swap:
b.swap(a);
cout << "Test: a should be empty" << endl;
for (int k = 0; k < a.uniqueSize(); k++)
	{
        string x = "empty";
        int n = a.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }
cout << "Test b: This, is, a, test, test" << endl;
for (int k = 0; k < b.uniqueSize(); k++)
	{
        string x = "empty";
        int n = b.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }
assert(a.size() == 0 && a.uniqueSize() == 0 && b.size() == 5 && b.uniqueSize() == 4);
b.swap(a);

b=a; //assignment
b.insert("a");
b.insert("!!!");
//b: This, is, a, a, test, test, !!!

cout << "Test b: This, is, a, a, test, test, !!!" << endl;
for (int k = 0; k < b.uniqueSize(); k++)
	{
        string x = "empty";
        int n = b.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }
assert(a.contains("!!!") == false); //changing b does not affect a

//test count:
assert(a.count("This") == 1);
assert(a.count("test") == 2);
assert( a.count("hi") == 0);

//test contains:
assert(a.contains("This"));
assert(a.contains("test"));
assert(!a.contains("hi"));

//test get:
ItemType x = "hi";
assert(b.get(-1, x) == 0 && x == "hi"); //negative number
assert(b.get(10, x) == 0 && x == "hi"); //exceed size

//test erase:
assert(a.erase("test") == 1); //decrement
assert(a.size() == 4 && a.uniqueSize() == 4 && a.count("test") == 1);
assert(a.erase("is") == 1); //remove middle
assert(a.size() == 3 && a.uniqueSize() == 3 && a.count("is") == 0);
assert(a.erase("This") == 1); //remove beginning
assert(a.size() == 2 && a.uniqueSize() == 2 && a.count("This") == 0);
assert(a.erase("test") == 1); //remove end
assert(a.size() == 1 && a.uniqueSize() == 1 && a.count("test") == 0);
assert(a.erase("hi") == 0); //not found, check vars after

cout << endl << endl << "B: ";
	for (int k = 0; k < b.uniqueSize(); k++)
	{
        string x = "empty";
        int n = b.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

//test eraseAll:
assert(b.eraseAll("hi") == 0); //not found
assert(b.size() == 7 && b.uniqueSize() == 5 && b.count("hi") == 0);
assert(b.eraseAll("This") == 1); //remove beginning
assert(b.size() == 6 && b.uniqueSize() == 4 && b.count("This") == 0);
assert(b.eraseAll("test") == 2); //remove middle
assert(b.size() == 4 && b.uniqueSize() == 3 && b.count("test") == 0);
assert(b.eraseAll("!!!")); //remove end
assert(b.size() == 3 && b.uniqueSize() == 2 && b.count("!!!") == 0);

cout << "Passed all tests." << endl;

//test destructor?

}
*/

/*
//make unsigned long
#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	//test combine:
	cout << "TEST COMBINE FUNCT" << endl;
	Multiset m1;
	m1.insert(2);
	m1.insert(2);
	m1.insert(2);
	m1.insert(8);
	m1.insert(3);
	m1.insert(3);
	m1.insert(9);
	m1.insert(5);
	//m1: 2 2 2 8 3 3 9 5

	Multiset m2;
	m2.insert(6);
	m2.insert(3);
	m2.insert(8);
	m2.insert(8);
	m2.insert(5);
	m2.insert(10);
	//m2: 6 3 8 8 5 10

	//test combine with empty result1
	Multiset result1;
	combine(m1, m2, result1);
	assert(result1.size() == 14 && result1.uniqueSize() == 7);
	cout << "Result1" << endl;
	for (int k = 0; k < result1.uniqueSize(); k++)
	{
        ItemType x = 0;
        int n = result1.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

	//test combine with non-empty result2
	Multiset result2;
	result2.insert(100);
	result2.insert(200);
	result2.insert(300);
	result2.insert(400);
	result2.insert(500);
	result2.insert(600);
	combine(m1, m2, result2);
	assert(result2.size() == 14 && result2.uniqueSize() == 7);
	cout << "Result2" << endl;
	for (int k = 0; k < result2.uniqueSize(); k++)
	{
        ItemType x = 0;
        int n = result2.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

	//test combine with empty m_1, m_2
	Multiset m_1;
	Multiset m_2;
	combine(m_1, m_2, result2);
	assert(result2.size() == 0 && result2.uniqueSize() == 0);
	cout << "Result2.2" << endl;
	for (int k = 0; k < result2.uniqueSize(); k++)
	{
        ItemType x = 0;
        int n = result2.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

	//test combine with ms1 = ms2
	m1=m2;
	//both: 6 3 8 8 5 10
	combine(m1, m2, result1);
	assert( result1.size() == 12 && result1.uniqueSize() == 5); //result should contain double everything

	cout << endl;

	//----------------------------
	//test subtract
	cout << "TEST SUBTRACT FUNCT" << endl;

	m_1.insert(2);
	m_1.insert(2);
	m_1.insert(2);
	m_1.insert(8);
	m_1.insert(3);
	m_1.insert(3);
	m_1.insert(9);
	m_1.insert(5);
	//m_1: 2 2 2 8 3 3 9 5

	m_2.insert(6);
	m_2.insert(3);
	m_2.insert(8);
	m_2.insert(8);
	m_2.insert(5);
	m_2.insert(10);
	//m_2: 6 3 8 8 5 10

	//test with empty result3
	Multiset result3;
	subtract(m_1, m_2, result3);
	assert(result3.size() == 5 && result3.uniqueSize() == 3);
	cout << "Result1" << endl;
	for (int k = 0; k < result3.uniqueSize(); k++)
	{
        ItemType x = 0;
        int n = result3.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

	//test with non-empty result1
	subtract(m_1, m_2, result1);
	assert(result1.size() == 5 && result1.uniqueSize() == 3);
	cout << "Result1" << endl;
	for (int k = 0; k < result1.uniqueSize(); k++)
	{
        ItemType x = 0;
        int n = result1.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

	//test with m1=m2
	subtract(m1, m2, result1);
	assert(result1.size() == 0 && result1.uniqueSize() == 0);

	//test with ms1 = result (aliasing)
	subtract(m_1, m_2, m_1);
	assert(m_1.size() == 5 && m_1.uniqueSize() == 3);
	cout << "Aliasing1" << endl;
	for (int k = 0; k < m_1.uniqueSize(); k++)
	{
        ItemType x = 0;
        int n = m_1.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

	Multiset m_11;
	m_11.insert(2);
	m_11.insert(2);
	m_11.insert(2);
	m_11.insert(8);
	m_11.insert(3);
	m_11.insert(3);
	m_11.insert(9);
	m_11.insert(5);
	//m_11: 2 2 2 8 3 3 9 5

	Multiset m_22;
	m_22.insert(6);
	m_22.insert(3);
	m_22.insert(8);
	m_22.insert(8);
	m_22.insert(5);
	m_22.insert(10);
	//m_22: 6 3 8 8 5 10

	//test with ms2=result (aliasing)
	subtract(m_11, m_22, m_22);
	assert(m_22.size() == 5 && m_22.uniqueSize() == 3);
	cout << "Aliasing2" << endl;
	for (int k = 0; k < m_22.uniqueSize(); k++)
	{
        ItemType x = 0;
        int n = m_22.get(k, x);
        cout << x << " occurs " << n << " times." << endl;
    }

	cout << "Passed all tests." << endl;
}
*/

	/*
 #include "Multiset.h"

        #define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }

        void thisFunctionWillNeverBeCalled()
        {
            Multiset();
            (Multiset(Multiset()));
            CHECKTYPE(&Multiset::operator=,  Multiset& (Multiset::*)(const ItemType&));
            CHECKTYPE(&Multiset::empty,      bool (Multiset::*)() const);
            CHECKTYPE(&Multiset::size,       int  (Multiset::*)() const);
            CHECKTYPE(&Multiset::uniqueSize, int  (Multiset::*)() const);
            CHECKTYPE(&Multiset::insert,     bool (Multiset::*)(const ItemType&));
            CHECKTYPE(&Multiset::erase,      int  (Multiset::*)(const ItemType&));
            CHECKTYPE(&Multiset::eraseAll,   int  (Multiset::*)(const ItemType&));
            CHECKTYPE(&Multiset::contains,   bool (Multiset::*)(const ItemType&) const);
            CHECKTYPE(&Multiset::count,      int  (Multiset::*)(const ItemType&) const);
            CHECKTYPE(&Multiset::get,        int  (Multiset::*)(int, ItemType&) const);
            CHECKTYPE(&Multiset::swap,       void (Multiset::*)(Multiset&));
            CHECKTYPE(combine,  void (*)(const Multiset&, const Multiset&, Multiset&));
            CHECKTYPE(subtract, void (*)(const Multiset&, const Multiset&, Multiset&));
        }

        int main()
        {}
		*/

	/*
#include "Multiset.h"
        #include <iostream>
        #include <cassert>
        using namespace std;

        void test()
        {
            Multiset sms;
            assert(sms.insert("cumin"));
            assert(sms.insert("turmeric"));
            assert(sms.insert("cumin"));
            assert(sms.insert("coriander"));
            assert(sms.insert("cumin"));
            assert(sms.insert("turmeric"));
            assert(sms.size() == 6  &&  sms.uniqueSize() == 3);
            assert(sms.count("turmeric") == 2);
            assert(sms.count("cumin") == 3);
            assert(sms.count("coriander") == 1);
            assert(sms.count("cardamom") == 0);
        }

        int main()
        {
            test();
            cout << "Passed all tests" << endl;
        }
*/

/*
#include "Multiset.h"
        #include <iostream>
        #include <cassert>
        using namespace std;

        void test()
        {
            Multiset ulms;
            assert(ulms.insert(20));
            assert(ulms.insert(10));
            assert(ulms.insert(20));
            assert(ulms.insert(30));
            assert(ulms.insert(20));
            assert(ulms.insert(10));
            assert(ulms.size() == 6  &&  ulms.uniqueSize() == 3);
            assert(ulms.count(10) == 2);
            assert(ulms.count(20) == 3);
            assert(ulms.count(30) == 1);
            assert(ulms.count(40) == 0);
        }

        int main()
        {
            test();
            cout << "Passed all tests" << endl;
        }
	*/