#include "CProxyServer.h"

CProxyServer::CProxyServer(PCSTR port)
{
	m_strPort = port;
    m_pFinalAddrInfo = NULL;
    m_ListenSocket = INVALID_SOCKET;
    m_ClientSocket = INVALID_SOCKET;
    int iResult = GetModuleFileName(NULL, m_strExePath, MAX_PATH);
    if (iResult == 0)
    {
        printf("[*] Error getting exe path.\n");
        exit(0);
    }

    initAddrInfo(m_AddrInfo);
}

void CProxyServer::Init()
{
    printInfo(m_strPort);

    int iResult = getaddrinfo(NULL, m_strPort, &m_AddrInfo, &m_pFinalAddrInfo);
    if (iResult != 0)
        errorAddrInfo(iResult);

    m_ListenSocket = socket(m_pFinalAddrInfo->ai_family, m_pFinalAddrInfo->ai_socktype, m_pFinalAddrInfo->ai_protocol);
    if (m_ListenSocket == INVALID_SOCKET)
    {
        freeaddrinfo(m_pFinalAddrInfo);
        errorCreatingSocket();
    }

    iResult = bind(m_ListenSocket, m_pFinalAddrInfo->ai_addr, (int)m_pFinalAddrInfo->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("[*] Socket bind failed. Error code: %d\n", WSAGetLastError());
        freeaddrinfo(m_pFinalAddrInfo);
        closesocket(m_ListenSocket);
        WSACleanup();
        exit(0);
    }

    freeaddrinfo(m_pFinalAddrInfo);

    iResult = listen(m_ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("[*] Listen failed. Error code: %d\n", WSAGetLastError());
        closesocket(m_ListenSocket);
        WSACleanup();
        exit(0);
    }
}

void CProxyServer::Start()
{
    STARTUPINFO si = STARTUPINFO();
    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(STARTUPINFO);

    PROCESS_INFORMATION pi;
    
    while (true) 
    {
        m_ClientSocket = accept(m_ListenSocket, NULL, NULL);
        if (m_ClientSocket == INVALID_SOCKET)
        {
            printf("[*] Error accepting connection. Error code: %d\n", WSAGetLastError());
            closesocket(m_ListenSocket);
            WSACleanup();
            exit(0);
        }
        m_ClientSockets.push_back(m_ClientSocket);

        LPTSTR args = new TCHAR[200];
        wsprintf(args, "CTX accept %d", m_ClientSocket);

        printf("[*] Accepted connection\n");

        if (!CreateProcess(NULL, args, NULL, NULL, TRUE,
            0, NULL, NULL, &si, &pi))
        {
            printf("[*] Error creating child.\n");
            exit(0);
        }
        
    }
}
