#include "Info.h"

void printUsage()
{
	printf("[*] Usage: CTX.exe <port number>\n");
	printf("[*] Where port number is a port which server should bind to\n");
}

void printInfo(PCSTR port)
{
	printf("[*] CTX, 2020 Wiktor Cudek\n");
	printf("[*] DEBUG option defined\n");
	printf("[*] CTX starting on port %s...\n", port);
}

void errorSocket()
{
	printf("[*] Socket creation failed. Please re-check socket number.\n");
	exit(0);
}

void errorWSAStartup(int err)
{
	printf("[*] WSAStartup failed. Error code: %d\n", err);
	exit(0);
}

void errorAddrInfo(int err)
{
	printf("[*] GetAddrInfo failed. Error code: %d\n", err);
	WSACleanup();
	exit(0);
}

void errorCreatingSocket()
{
	printf("[*] Socket creation failed. Error code: %ld\n", WSAGetLastError());
	WSACleanup();
	exit(0);
}

void initAddrInfo(struct addrinfo& addrInfo)
{
	ZeroMemory(&addrInfo, sizeof(addrInfo));
	addrInfo.ai_family = AF_INET;
	addrInfo.ai_socktype = SOCK_STREAM;
	addrInfo.ai_protocol = IPPROTO_TCP;
	addrInfo.ai_flags = AI_PASSIVE;
}