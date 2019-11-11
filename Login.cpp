// Login.cpp: implementation of the CLogin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Login.h"
#include "Protocol.h"
#include "Socket32.h"
#include "FtpApp.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CError
{
	const char* m_pError;
public:
	CError(const char* p)
	{
		m_pError = p;
	}
	const char* what(){return m_pError;}

};
CLogin::CLogin(CSocket32& socka):m_socka(socka),m_bLogin(FALSE)
{
}

CLogin::~CLogin()
{

}

BOOL CLogin::Login(LPCSTR sKey, LPCSTR data)
{
	if(m_bLogin)return TRUE;
	try
	{
		if(!strcmp(sKey,"USER"))
			if(USER(data))return TRUE;
		if(!strcmp(sKey,"PASS"))
			if(PASS(data))return TRUE;
	}catch(CError &e)
	{
		string str = "530 Login incorrect for " + string(e.what()) + "." + CRLF;
		m_socka.Send(str.c_str(),str.length());
	}
	return FALSE;
}

BOOL CLogin::USER(const char* data)
{
	char s[256];
	strcpy(s,data);
	_strupr(s);
	if(!theApp.m_map.count(s))
	{
		throw CError("username");
	}
		
	m_szUser = s;
	string str = data;
	str = "331 Password required for "+str+"." + CRLF;
	m_socka.Send(str.c_str(),str.length());

	return TRUE;

}
BOOL CLogin::PASS(const char* data)
{
	if(theApp.m_map[m_szUser].szPass != data)
		throw CError("password");
	m_bLogin = TRUE;
	m_szPass = data;
	m_szPath = theApp.m_map[m_szUser].szPath;
	string str = "230 User "+m_szUser +" logged in , proceed." +CRLF;
	m_socka.Send(str.c_str(),str.length());

	return TRUE;
	
}


