#pragma once
#include <string>
#include <map>
#include <iostream>
#include "Debug.h"

#define MIN_LEN 4
#define MAX_LEN 65535

using namespace std;

class CRequest
{
	// methods
	//----------------------------------------
public:
	CRequest(string request);

	string getRequest();
	string getStrProtocol() { return m_strProtocol; }
	string getHost() { return m_strHost; }
	string getPort() { return m_strPort; }
	string getMethod() { return m_strMethod; }
protected:
private:
// attributes
//----------------------------------------
public:
	int Parse(string request);
protected:
private:
	string m_strProtocol;
	string m_strMethod;
	string m_strHost;
	string m_strPort;
	string m_strPath;
	string m_strVersion;
	string m_strBuffer;
	// key:value
	map<string, string> m_Headers;
};

