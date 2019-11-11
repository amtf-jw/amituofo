// FtpApp.h: interface for the CFtpApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPAPP_H__0267EDD2_9DB8_4968_8FB0_0CFD91B9B74C__INCLUDED_)
#define AFX_FTPAPP_H__0267EDD2_9DB8_4968_8FB0_0CFD91B9B74C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "define.h"
#include <map>
struct SInfo
{
	string szPass;
	string szPath;
};
class CFtpApp 
{
public:
	map<string ,SInfo> m_map;
//static string GetString(LPSTR s,LPSTR sKey = NULL);
	BOOL Allow();
	string m_szServer;
	BOOL Start();
	BOOL InitInstance();
	CFtpApp();
	virtual ~CFtpApp();

};

#endif // !defined(AFX_FTPAPP_H__0267EDD2_9DB8_4968_8FB0_0CFD91B9B74C__INCLUDED_)
