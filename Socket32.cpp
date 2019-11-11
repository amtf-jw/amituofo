// Socket32.cpp: implementation of the CSocket32 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Socket32.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocket32::CSocket32()
{
#ifdef _WIN32
	WSAData wd;
	WSAStartup(0x0202,&wd);
#endif
	m_hSocket = INVALID_SOCKET;
}

CSocket32::~CSocket32()
{
	Close();
}
BOOL CSocket32::Create( UINT nSocketPort, int nSocketType ,
		LPCTSTR lpszSocketAddress )
{

	assert(m_hSocket == INVALID_SOCKET);

	m_hSocket = socket(AF_INET,nSocketType,IPPROTO_IP);
	if(	m_hSocket == INVALID_SOCKET)
		return FALSE;

	sockaddr_in addr = {AF_INET,htons(nSocketPort)};

	if (!lpszSocketAddress) 
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(lpszSocketAddress);
	
	if (!bind(m_hSocket,(sockaddr*)&addr,sizeof(addr)))
		return TRUE;
	Close();
	
	return FALSE;
}


int CSocket32::SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, LPCSTR lpszHostAddress , int nFlags)
{
	sockaddr_in addr = {
		AF_INET,
		htons(nHostPort),
	};
	addr.sin_addr.s_addr = inet_addr(lpszHostAddress);
	return sendto(m_hSocket,(char*)lpBuf,nBufLen,nFlags,(sockaddr*)&addr,sizeof(addr));
}

int CSocket32::ReceiveFrom( void* lpBuf, int nBufLen, char *rSocketAddress, UINT& rSocketPort, int nFlags )
{
    sockaddr_in from;
    socklen_t fromlen = sizeof(from);
	
	int nRet = recvfrom(m_hSocket,(LPSTR)lpBuf,nBufLen,nFlags,(sockaddr*)&from,&fromlen);
	if(nRet <= 0)
		return nRet;
	if(rSocketAddress)
	{
		strcpy(rSocketAddress,inet_ntoa(from.sin_addr));
		rSocketPort = htons(from.sin_port);
	}

	return nRet;
}

BOOL CSocket32::Accept( CSocket32& rConnectedSocket, LPSTR lpSockAddr ,UINT *nPort )
{
	sockaddr_in addr = {AF_INET};
	socklen_t nLen = sizeof(addr);
	rConnectedSocket.m_hSocket = accept(m_hSocket,(sockaddr*)&addr,&nLen);
	if(rConnectedSocket.m_hSocket == INVALID_SOCKET)
		return FALSE;
	if(lpSockAddr)
	{
		strcpy(lpSockAddr,inet_ntoa(addr.sin_addr));
		*nPort = htons(addr.sin_port);
	}
	return TRUE;	
}
BOOL CSocket32::Connect( LPCTSTR lpszHostAddress, UINT nHostPort )
{
	sockaddr_in addr = {AF_INET,htons(nHostPort)};
	addr.sin_addr.s_addr = inet_addr(lpszHostAddress);//. . . http:://www.xx.com computer
	if (addr.sin_addr.s_addr == INADDR_NONE)
	{
		hostent * lphost = gethostbyname(lpszHostAddress);
		if (lphost != NULL)
			addr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			//WSAGetLastError();
		//	WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}
	return !connect(m_hSocket,(sockaddr*)&addr,sizeof(addr));
}


BOOL CSocket32::GetSockName( char*  rSocketAddress, UINT& rSocketPort )
{
	sockaddr_in addr;
	socklen_t nLen = sizeof(addr);

	if(SOCKET_ERROR == getsockname(m_hSocket, (sockaddr*)&addr, &nLen))
		return FALSE;
	if(rSocketAddress)
	{
		strcpy(rSocketAddress,inet_ntoa(addr.sin_addr));
		rSocketPort = htons(addr.sin_port);
	}
	return TRUE;
}
BOOL CSocket32::GetPeerName(  char* rPeerAddress, UINT& rPeerPort )
{
	sockaddr_in addr;
	socklen_t nLen = sizeof(addr);

	if(SOCKET_ERROR == getpeername(m_hSocket, (sockaddr*)&addr, &nLen))
		return FALSE;
	if(rPeerAddress)
	{
		strcpy(rPeerAddress,inet_ntoa(addr.sin_addr));
		rPeerPort = htons(addr.sin_port);
	}
	return TRUE;
}
BOOL CSocket32::Bind( UINT nSocketPort, LPCTSTR lpszSocketAddress )
{
	return TRUE;
}

