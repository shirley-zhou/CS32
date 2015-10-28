#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
	//retrieve webpage from url
	string webpage;
	if ( !HTTP().get(url, webpage) )
		return false;

	//compress, encode, and embed msg in webpage text
	if (!Steg::hide(webpage, msg, host) )
		return false;
	return true;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
	string webpage;
	if ( !HTTP().get(url, webpage) )
		return false;

	if (!Steg::reveal(webpage, msg) )
		return false;
	return true;
}
