#include "CConnectionHandler.h"

CConnectionHandler::CConnectionHandler(int client)
{
	m_Client = SOCKET(client);
    m_Server = INVALID_SOCKET;
    m_iMaxBufferSize = 5096;
    m_iTotalSize = 0;
    m_strBuffer = new char[m_iMaxBufferSize];
    m_strRequest = "";
}

void CConnectionHandler::CreateServerSocket()
{
    struct addrinfo addrInfo;
    struct addrinfo* finalAddrInfo;

    ZeroMemory(&addrInfo, sizeof(addrInfo));
    addrInfo.ai_family = AF_UNSPEC;
    addrInfo.ai_socktype = SOCK_STREAM;
    addrInfo.ai_protocol = IPPROTO_TCP;
    //addrInfo.ai_flags = AI_PASSIVE;
    
    int iResult = getaddrinfo(m_pRequest->getHost().c_str(), m_pRequest->getPort().c_str(), &addrInfo, &finalAddrInfo);
    if (iResult != 0)
        errorAddrInfo(iResult);

    m_Server = socket(finalAddrInfo->ai_family, finalAddrInfo->ai_socktype, finalAddrInfo->ai_protocol);
    
    if (m_Server == INVALID_SOCKET)
    {
        freeaddrinfo(finalAddrInfo);
        errorCreatingSocket();
    }

    if (connect(m_Server, finalAddrInfo->ai_addr, finalAddrInfo->ai_addrlen) < 0)
    {
        printf("[*] Socket connect failed. Error code: %d\n", WSAGetLastError());
        freeaddrinfo(finalAddrInfo);
        closesocket(m_Server);
        WSACleanup();
        exit(0);
    }


    freeaddrinfo(finalAddrInfo);
}

void CConnectionHandler::WriteToServer(string message)
{

    string strTmp;
    strTmp.append(message);

    int iTotalSent = 0;
    int iSentEach;
    int iMessageLength = message.length();

    while (iTotalSent < iMessageLength) 
    {
        if ((iSentEach = send(m_Server, (message.c_str() + iTotalSent), iMessageLength - iTotalSent, 0)) < 0) 
        {
            cout << "[*] Error sending data to server" << endl;
            return;
        }
        iTotalSent += iSentEach;
    }

}

void CConnectionHandler::WriteToClient(string message)
{

    string strTmp;
    strTmp.append(message);

    int iTotalSent = 0;
    int iSentEach;
    int iMessageLength = message.length();

    while (iTotalSent < iMessageLength)
    {
        if ((iSentEach = send(m_Client, (message.c_str() + iTotalSent), iMessageLength - iTotalSent, 0)) < 0)
        {
            cout << "[*] Error sending data to server" << endl;
            return;
        }
        iTotalSent += iSentEach;
    }

}

void CConnectionHandler::WriteToClientConnect(string message)
{

    string strTmp;
    strTmp.append(message);

    int iTotalSent = 0;
    int iSentEach;
    int iMessageLength = message.length();
    int iRecv;
    char strBuffer[10000];

    while (iTotalSent < iMessageLength)
    {
        if ((iSentEach = send(m_Client, (message.c_str() + iTotalSent), iMessageLength - iTotalSent, 0)) < 0)
        {
            cout << "[*] Error sending data to server" << endl;
            return;
        }
        iTotalSent += iSentEach;
    }

    while ((iRecv = recv(m_Client, strBuffer, 10000, 0)) > 0)
    {
        cout << "CONNECT RESPONSE(CLIENT): ======== " << strBuffer << endl;
        string strResult(strBuffer);
        WriteToServer(strResult);
        memset(strBuffer, 0, sizeof(strBuffer));
    }

    if (iRecv < 0)
    {
        cout << "[*] Error writing to client" << endl;
        return;
    }

    GetServerResponse();
}

void CConnectionHandler::GetServerResponse()
{
    int iRecv;
    char strBuffer[10000];

    cout << "[*] Getting server response " << endl;

    while ((iRecv = recv(m_Server, strBuffer, 10000, 0)) > 0)
    {
        cout << "RESPONSE: ======== " << strBuffer << endl;
        string strResult(strBuffer);
        WriteToClient(strResult);
        memset(strBuffer, 0, sizeof(strBuffer));
    }

    if (iRecv < 0)
    {
        cout << "[*] Error writing to client" << endl;
        return;
    }
}

void CConnectionHandler::HandleConnection()
{
    int iResult;

    while (m_strRequest.find("\r\n\r\n") == string::npos)
    {
        iResult = recv(m_Client, m_strBuffer, m_iMaxBufferSize, 0);
        if (iResult == 0)
        {
            break;
        }
        else if (iResult < 0)
        {
            printf("[*] Error receiving data\n");
            exit(0);
        }
        else
        {
            m_iTotalSize += iResult;
            m_strBuffer[iResult] = '\0';
        }

        m_strRequest += m_strBuffer;
    }

    m_pRequest = new CRequest(m_strRequest);
    string test = m_pRequest->getRequest();

    CreateServerSocket();

    if ((iResult = m_pRequest->getMethod().find("CONNECT")) != string::npos)
    {
        cout << "CONNECT detected" << endl;
        test = "200 OK\r\n";
        WriteToClientConnect(test);
    }
    else
    {
        WriteToServer(m_strRequest);
        GetServerResponse();
    }
    // closesocket(m_Server);
    // closesocket(m_Client);
}