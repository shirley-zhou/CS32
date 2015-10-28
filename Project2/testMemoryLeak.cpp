#include "Multiset.h"
#include <iostream>
#include <cassert>

using namespace std;

void test()
{
	  cerr << "test A" << endl;
	Multiset ms;
	  cerr << "test B" << endl;
	ms.insert(IntWrapper(10));
	  cerr << "test C" << endl;
	ms.insert(IntWrapper(20));
	  cerr << "test D" << endl;
	Multiset ms2;
	  cerr << "test E" << endl;
	ms2.insert(IntWrapper(30));
	  cerr << "test F" << endl;
	ms2 = ms;
	  cerr << "test G" << endl;
	ms2.insert(IntWrapper(40));
	  cerr << "test H" << endl;
}

int main()
{
	test();
	cerr << "DONE" << endl;
}
