#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//Hash Functions
unsigned int computeHash(string key)
{
	unsigned int h = 2166136261U;
	for (unsigned int k = 0; k < key.size(); k++)
	{
		h += key[k];
		h *= 16777619;
	}
	return h;
}

unsigned int computeHash(unsigned short key)
{
	return key;
}

//compress string of characters to unsigned short values
//based on LZW compression algorithm
void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
	numbers.clear();

	//create a hash table to map strings to unsigned shorts
	int length = s.size();
	int capacity = min(length/2 + 512, 16384);
	int buckets = capacity/0.5; //max load factor = 0.5
	HashTable<string, unsigned short> H(buckets, capacity);
	
	//add associations for 256 chars
	for (int k = 0; k < 256; k++)
	{
		string current(1, static_cast<char>(k));
		H.set(current, k, true);
	}

	//add other associations based on input string
	int nextFreeID = 256;
	string runSoFar = "";
	for (unsigned int k = 0; k < s.size(); k++)
	{
		char c = s[k];
		string expandedRun = runSoFar + c;
		unsigned short x;
		if ( H.get(expandedRun, x) )
		{
			runSoFar = expandedRun;
			continue;
		}
		H.get(runSoFar, x);
		numbers.push_back(x);
		H.touch(runSoFar);
		runSoFar="";

		unsigned short cv;
		H.get(string(1, c), cv);
		numbers.push_back(cv);
		if (!H.isFull())
		{
			H.set(expandedRun, nextFreeID);
			nextFreeID++;
		}
		else //table at capacity
		{
			string k;
			unsigned short value;
			H.discard(k, value );
			H.set(expandedRun, value);
		}
	}

	if (runSoFar.size() != 0)
	{
		unsigned short x;
		H.get(runSoFar, x);
		numbers.push_back(x);
	}
	numbers.push_back(capacity);
	
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	//error: vector contains either nothing or only a hashtable capacity number
	if (numbers.size() <=1)
		return false;

	//create a hash table to map unsigned shorts to strings
	int capacity = numbers.back();
	int buckets = capacity/0.5; //max load factor = 0.5
	HashTable<unsigned short, string> H(buckets, capacity);

	//add associations for 256 chars
	for (int k = 0; k < 256; k++)
	{
		string current(1, static_cast<char>(k));
		H.set(k, current, true);
	}

	//add other associations based on input string
	int nextFreeID = 256;
	string runSoFar;
	for (unsigned int k = 0; k < numbers.size()-1; k++)
	{
		unsigned short us = numbers[k];
		if (us <=255) //one byte character
		{
			string value;
			H.get(us, value);
			s += value;
			if ( runSoFar == "")
			{
				runSoFar += value;
				continue;
			}
			string expandedRun = runSoFar + value;
			if (!H.isFull())
			{
				H.set(nextFreeID, expandedRun);
				nextFreeID++;
			}
			else
			{
				unsigned short key;
				string value;
				H.discard(key, value );
				H.set(key, expandedRun);
			}
			runSoFar = "";
			continue;
		}

		//otherwise us represents a multi-string char
		string value;
		if (!H.get(us, value))
			return false; //error: key not found
		H.touch(us);
		s += value;
		runSoFar = value;
	}
	return true;
}
