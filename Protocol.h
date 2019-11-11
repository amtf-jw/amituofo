// Protocol.h: interface for the CProtocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOL_H__DB03E75F_7182_4981_B9D3_0C70B976CAFC__INCLUDED_)
#define AFX_PROTOCOL_H__DB03E75F_7182_4981_B9D3_0C70B976CAFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Socket32.h"
#include "define.h"
#include "Login.h"
class CProtocol  
{
	BOOL Run();
	BOOL m_bRun;
	
	BOOL Process(LPCSTR sKey,string &str);
	BOOL MKD(LPCSTR sKey,string &str);
	BOOL RMD(LPCSTR sKey,string &str);
	BOOL RNFR(LPCSTR sKey,string &str);
	BOOL RNTO(LPCSTR sKey,string &str);
	BOOL DELE(LPCSTR sKey,string &str);
	BOOL NOOP(LPCSTR sKey,string &str);
	BOOL NLST(LPCSTR sKey,string &str);
	BOOL PASV(LPCSTR sKey,string &str);
	BOOL TYPE(LPCSTR sKey,string &str);
	BOOL SYST(LPCSTR sKey,string &str);
	BOOL PWD(LPCSTR sKey,string &str);
	BOOL CWD(LPCSTR sKey,string &str);

	BOOL Unknown(LPCSTR sKey,string &str);
	BOOL IsPath(string str);
	enum EType{A='A',I='I'};
	string GetTime(time_t time);
	static void theProc(void *);
	CSocket32 m_socka;
	CLogin m_log;
	string m_szCWD;
	string m_szRename;
	CSocket32 m_sockp;
	CSocket32 m_sockt;

	EType m_eType;
public:
	CSocket32 &Socket(){return m_socka;}
	void Do();
	
	CProtocol();
	virtual ~CProtocol();

};

#endif // !defined(AFX_PROTOCOL_H__DB03E75F_7182_4981_B9D3_0C70B976CAFC__INCLUDED_)
