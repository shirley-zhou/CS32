/*
#include <cassert>
#include <iostream>
using namespace std;
*/

//bool somePredicate(double x);

// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{

	if (n <= 0)
		return false;
	if (somePredicate(a[0]))
		return true;
	return anyTrue(a+1, n-1);

}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
	if (n <= 0)
		return 0;
	int total = countTrue(a, n-1);
	if ( somePredicate(a[n-1]) )
		total++;
	return total;
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (somePredicate(a[0]))
		return 0;
	int first = firstTrue(a+1, n-1);
	if (first != -1)
		return first+1;
	else
		return -1;
}

// Return the subscript of the smallest element in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (n == 1)
		return 0;
	int smallest = indexOfMin(a, n-1);
	if (a[n-1] < a[smallest])
		smallest = n-1;
	return smallest;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n1 <= 0 && n2 > 0)
		return false;
	if (n2 <= 0)
		return true; //but what if n2 is negative??
	if (n1 < n2)
		return false;
	if (a2[0] != a1[0])
		return includes(a1+1, n1-1, a2, n2);
	else
		return includes(a1+1, n1-1, a2+1, n2-1);
}
/*
bool somePredicate(double x)
{
	return x < 0;
}

int main()
{
	double array[5] = {1, 0, 1.1, -5, 6};
	double array2[3] = {0, -5, 6};
	double array3[3]={};
	double array4[1]={1};
	double array5[2]={1, 2};
	double array6[3]={4, -5, -7};
	assert(anyTrue(array, 5) && !anyTrue(array3, 0) && !anyTrue(array4, 1) && !anyTrue(array5, 2));

	assert(countTrue(array, 5) == 1 && countTrue(array3, 0) == 0);
	assert(countTrue(array4, 1) == 0 && countTrue(array5, 2) == 0 && countTrue(array6, 3) == 2 );
	assert(firstTrue(array, 5) == 3 && firstTrue(array3, 0) == -1);
	assert(firstTrue(array4, 1) == -1 && firstTrue(array5, 2) == -1 && firstTrue(array6, 3) == 1);
	assert(indexOfMin(array, 5) == 3 && indexOfMin(array3, 0) == -1);
	assert(indexOfMin(array4, 1) == 0 && indexOfMin(array5, 2) == 0 && indexOfMin(array6, 3) == 2);
	assert(includes(array, 5, array2, 3) && includes(array, 5, array3, 0) && includes(array, 5, array4, 1));
	assert(!includes(array2, 3, array5, 2) && !includes(array3, 0, array5, 2) && includes(array5, 2, array4, 1));
	cout << "Passed all tests." << endl;
}
*/