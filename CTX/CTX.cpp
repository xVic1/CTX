#include "CProxyServer.h"
#include "CConnectionHandler.h"

int main(int argc, char *argv[])
{

#ifdef WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("Error initializing winsock\n");
        exit(0);
    }
#endif

    PCSTR strPort;

    if (argc != 2 && argc != 3)
    {
        printUsage();
        exit(0);
    }

    if (argc == 3 )
    {
        int client = atoi(argv[2]);
        CConnectionHandler* handler = new CConnectionHandler(client);
        handler->HandleConnection();
        return 0;
    }

    strPort = argv[1];
    CProxyServer* pProxyServer = new CProxyServer(strPort);
    pProxyServer->Init();
    pProxyServer->Start();
    return 0;
}