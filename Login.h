// Login.h: interface for the CLogin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGIN_H__DBE3FCF1_1453_4676_8266_B56F0463CC42__INCLUDED_)
#define AFX_LOGIN_H__DBE3FCF1_1453_4676_8266_B56F0463CC42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "define.h"
class CSocket32;
class CLogin  
{	
	BOOL m_bLogin;
	string m_szUser;
	string m_szPass;
	string m_szPath;
	BOOL USER(LPCSTR data);
	BOOL PASS(LPCSTR data);
	CSocket32 &m_socka;
public:
	inline string &GetUser() {return m_szUser;}
	inline string &GetPath() {return m_szPath;}
	inline BOOL IsLoged(){return m_bLogin;}
	BOOL Login(LPCSTR sKey, LPCSTR data);
	CLogin(CSocket32& socka);
	virtual ~CLogin();

};

#endif // !defined(AFX_LOGIN_H__DBE3FCF1_1453_4676_8266_B56F0463CC42__INCLUDED_)
