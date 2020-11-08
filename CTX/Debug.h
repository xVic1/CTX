#pragma once
#define DEBUG

#define DBGMSG( os, var ) \
	(os) << "[*] " << #var << " = [" << (var) << "]" << std::endl;
