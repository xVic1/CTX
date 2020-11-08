#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <exception>
#include <string>

#include "Debug.h"

#pragma comment (lib, "Ws2_32.lib")

void printUsage();
void printInfo(PCSTR port);
void errorSocket();
void errorWSAStartup(int err);
void errorAddrInfo(int err);
void errorCreatingSocket();
void initAddrInfo(struct addrinfo & addrInfo);
