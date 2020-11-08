#pragma once
#include "Info.h"
#include <list>

class CProxyServer
{
// methods
//----------------------------------------
public:
	CProxyServer(PCSTR port);

	void Init();
	void Start();
protected:
private:

// attributes
//----------------------------------------
public:
protected:
private:
	PCSTR m_strPort;
	struct addrinfo m_AddrInfo;
	struct addrinfo* m_pFinalAddrInfo;
	SOCKET m_ListenSocket;
	std::list<SOCKET> m_ClientSockets;
	SOCKET m_ClientSocket;
	WSADATA m_WsaData;
	char m_strExePath[MAX_PATH];
};

