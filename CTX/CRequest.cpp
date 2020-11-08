#include "CRequest.h"

CRequest::CRequest(string request)
{
	m_strMethod = "";
	m_strProtocol = "";
	m_strHost = "";
	m_strPort = "";
	m_strPath = "";
	m_strVersion = "";
	m_strBuffer = "";
	this->Parse(request);
}

int CRequest::Parse(string request)
{
	if (request.find("hackerwatch") != string::npos)
		exit(1);

	if ((unsigned int)request.length() < MIN_LEN || (unsigned int)request.length() > MAX_LEN)
		return -1;

	size_t iPosition = request.find("\r\n");

	if (iPosition == string::npos)
		return -1;

	m_strBuffer = request.substr(0, iPosition);

#ifdef DEBUG
	cout << endl << "[*] Parsing received data" << endl;
	DBGMSG(cout, request);
#endif // DEBUG

	iPosition = m_strBuffer.find(" ");
	m_strMethod = m_strBuffer.substr(0, iPosition);

	//if (m_strMethod.find("CONNECT") != string::npos)
	//	return -1;

	m_strBuffer.erase(0, iPosition + 1);
	iPosition = m_strBuffer.find(" ");
	m_strHost = m_strBuffer.substr(0, iPosition);
	
	m_strBuffer.erase(0, iPosition + 1);
	iPosition = m_strBuffer.find(" ");
	m_strVersion = m_strBuffer.substr(0, iPosition);

	if (m_strVersion.find("HTTP") == string::npos)
		return -1;

	m_strBuffer = m_strHost;
	iPosition = m_strBuffer.find(":");


	m_strHost = m_strBuffer.substr(0, iPosition);
	m_strBuffer = m_strBuffer.substr(iPosition+1);

	m_strProtocol = m_strBuffer;

	m_strBuffer = request;
	string strTmp = "";
	string headerName = "";
	string headerVal = "";

	iPosition = m_strBuffer.find("\r\n");
	strTmp = m_strBuffer.substr(0, iPosition);
	m_strBuffer.erase(0, iPosition + 2);

	do
	{
		iPosition = m_strBuffer.find("\r\n");
		strTmp = m_strBuffer.substr(0, iPosition);
		m_strBuffer.erase(0, iPosition + 2);

		iPosition = strTmp.find(":");

		if (iPosition == string::npos)
			break;
		
		headerName = strTmp.substr(0, iPosition);
		headerVal = strTmp.substr(iPosition+2);

		if (headerName.find("Host") != string::npos)
		{
			m_strHost = headerVal;
			while ((iPosition = m_strHost.find(" ")) != string::npos)
			{
				m_strHost.replace(iPosition, 1, "");
			}

			if ((iPosition = m_strHost.find(":")) != string::npos)
			{
				m_strPort = m_strHost.substr(iPosition + 1);
				m_strHost.erase(iPosition);
			}
			else
			{
				if (request.find(":443") != string::npos)
					m_strPort = "443";
				else
					m_strPort = "80";
			}
		}

		m_Headers.insert(pair<string, string>(headerName, headerVal));

	} while (strTmp[0] != '\0' && !(strTmp[0] == '\r'
		&& strTmp[1] == '\n'));

#ifdef DEBUG
	DBGMSG(cout, m_strMethod);
	DBGMSG(cout, m_strHost);
	DBGMSG(cout, m_strVersion);
	DBGMSG(cout, m_strProtocol);
	DBGMSG(cout, m_strPort);
	/*cout << "[*] Headers: " << endl;
	for (auto const& x : m_Headers)
	{
		cout << "[*] " << x.first << ":" << x.second << endl;
	}*/
	cout << "========================" << endl;
#endif //  DEBUG

	return 0;
}

string CRequest::getRequest()
{
	string request = "";

	request += m_strMethod + " " + m_strHost + " " + m_strVersion;
	request += "\r\n";

	// request += "Host :" + m_strHost + "\r\n";
	// request += "Connection: keep-alive\r\n";

	for (auto const& x : m_Headers)
	{
		request += x.first + ": " + x.second + "\r\n";
	}

	return request;
}