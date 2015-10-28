
#include "provided.h"
#include "HashTable.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	/*
	//test encode
	vector<unsigned short> v;
	v.push_back(1);
	v.push_back(5);
	string hiddenMessage = BinaryConverter::encode(v);
	assert(hiddenMessage == "               \t             \t \t");
	cout << hiddenMessage;

	//test decode
	string msg = "               \t             \t \t";
	if (BinaryConverter::decode(msg, v))
	{
		cout << "The vector has " << v.size() << " numbers:";
		for (int k = 0; k != v.size(); k++)
			cout << ' ' << v[k];
		cout << endl;
	}
	else
		cout << "The string has a bad character or a bad length.";
	*/

	/*
	//test hash table
	HashTable<int, string> test(5, 5);
	
	test.set(4, "first", false);
	test.set(1, "second", true);

	string value;
	test.get(4, value);
	cout << value;
	*/

	/*
	//test compressor
	string compressorTest = "AAAAAAAAAB";
	vector<unsigned short> nums;
	
	Compressor::compress(compressorTest, nums);
	
	for (int k = 0; k < nums.size(); k++)
		cout << nums[k] << ' ';
	
	
	//test decompressor
	string decompressed="";
	Compressor::decompress(nums, decompressed);
	cout << decompressed;
	*/

	/*
	string htmlIn = "<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ";
	string msg = "Haha this works!!";
	string htmlOut;
	Steg::hide(htmlIn, msg, htmlOut);

	string decodedMsg;
	bool success = Steg::reveal(htmlOut, decodedMsg);
	cout <<decodedMsg << endl;
	*/
}