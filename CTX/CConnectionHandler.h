#pragma once
#include "Info.h"
#include "CRequest.h"

using namespace std;

class CConnectionHandler
{
// methods
//----------------------------------------
public:
	CConnectionHandler(int client);
	void HandleConnection();
	void CreateServerSocket();
	void WriteToServer(string message);
	void WriteToClient(string message);
	void WriteToClientConnect(string message);
	void GetServerResponse();
protected:
private:
// attributes
//----------------------------------------
public:
protected:
private:
	SOCKET m_Client;
	SOCKET m_Server;
	int m_iMaxBufferSize;
	int m_iTotalSize;
	char* m_strBuffer;
	string m_strRequest;
	CRequest* m_pRequest;
};

