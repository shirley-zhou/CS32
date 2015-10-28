#include "provided.h"
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	//convert numbers to bit string
	string bitString="";
	for (int k=0; k < numbers.size(); k++)
		bitString += convertNumberToBitString(numbers[k]);

	//encode bit string as spaces and tabs
	//spaces = 0, tabs = 1
	string encodedString="";
	for (int k=0; k < bitString.size(); k++)
	{
		switch (bitString[k])
		{
		case '0':
			encodedString += ' ';
				break;
		case '1':
			encodedString += '\t';
			break;
		}
	}
	return encodedString;
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
	numbers.clear(); //clear vector before adding items

	//string must contain blocks of 16 characters
	if (bitString.size() % BITS_PER_UNSIGNED_SHORT != 0)
		return false;

	//convert encoded bitstring of spaces and tabs to 0s and 1s
	string currentString = "";
	for (int k = 0, count = 1; k < bitString.size(); k++, count++)
	{
		switch (bitString[k])
		{
		case ' ':
			currentString += '0';
			break;
		case '\t':
			currentString += '1';
			break;
		default:
			return false; //invalid character
		}

		//after each block of 16 bits processed, convert the sequence to a number and add to vector
		if (count == BITS_PER_UNSIGNED_SHORT)
		{
			unsigned short num;
			convertBitStringToNumber(currentString, num);
			numbers.push_back(num);
			currentString = "";
			count = 0; //reset counter
		}
	}
	return true;
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
