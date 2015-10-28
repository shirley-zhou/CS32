#include "provided.h"
#include <string>
#include <vector>
using namespace std;

//hide a msg string in the html text of a webpage
//by encoding message as spaces & tabs and appending to the end of each line
bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if (hostIn == "")
		return false;

	vector<string> lines;
	string current; //stores current line being processed
	string substring; //to store chars that might or might not be added
	for (int k = 0; k < hostIn.size(); k++)
	{
		char c = hostIn[k];
		switch(c)
		{
		case '\n':
			//finished processing a line
			//ignore all chars in substring (spaces, tabs, '\r' right before end of line are discarded)
			substring = "";
			lines.push_back(current);
			current = "";
			break;
		case '\r':
		case ' ':
		case '\t':
			//these chars might or might not be added depending on whether they are at the end of a line
			substring += c;
			break;
		default:
			//reached a valid char, not at end of line
			//so any spaces, tabs, etc. previously stored in substring are permissible
			//add them to current string
			current += substring;
			substring = "";
			current += c; //add char to current line
			break;
		}
	}

	//in case text doesn't end with '\n'
	if (current != "")
		lines.push_back(current);

	vector<unsigned short> nums;
	Compressor::compress(msg, nums);

	string encodedString = BinaryConverter::encode(nums);

	int L = encodedString.size(); //total chars to be distributed
	int N = lines.size(); //number of lines in hostIn


	//separate encodedString into substrings and append to ends of lines
	//the first L%N substrings have length (L/N)+1
	//remaining substrings have length L/N
	int counter = 0;
	substring = "";
	hostOut =""; //make sure hostOut is empty
	for (int k = 0, i = 0; k < encodedString.size(); k++)
	{
		substring += encodedString[k];
		if ( (counter < L%N && substring.size() == (L/N + 1))
			|| (counter >= L%N && substring.size() == L/N) )
		{
			hostOut += lines[i];
			hostOut += substring;
			hostOut += '\n';
			substring = "";
			i++;
			counter++;
		}
	}
	return true;
}

//given a hostIn string with a hidden message encoded as tabs and spaces
//decode and store the message in string msg
bool Steg::reveal(const string& hostIn, string& msg) 
{
	if (hostIn == "")
		return false;

	//concatenate spaces and tabs at the end of each line to get an encoded msg
	string encodedMsg;
	string current=""; //to store chars that might or might not be added
	for (int k = 0; k < hostIn.size(); k++)
	{
		char c = hostIn[k];
		switch(c)
		{
		case '\n':
			//finished processing a line
			//add current spaces/tabs to encodedMsg string
			encodedMsg += current;
			current = "";
			break;
		case '\r': //ignore??
			break;
		case ' ':
		case '\t':
			current += c;
			break;
		default:
			//reached a char not at end of line, disregard all spaces/tabs collected so far in current string
			//spaces/tabs in the middle of a line are not part of an encoded secret message
			current="";
			break;
		}
	}

	vector<unsigned short> nums;
	if (!BinaryConverter::decode(encodedMsg, nums) ) //invalid string, unable to decode
		return false;
	if (!Compressor::decompress(nums, msg) ) //problem decompressing msg
		return false;
	return true;
}
