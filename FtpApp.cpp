// FtpApp.cpp: implementation of the CFtpApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FtpApp.h"
#include "Socket32.h"
#include "Protocol.h"
#include <iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//LPCSTR CFtpApp::CRLF = "\r\n";
CFtpApp::CFtpApp()
{
	m_szServer = "richmain.net";
	//测试用具
	m_map["TEST"].szPass = "test";
	m_map["TEST"].szPath = "F:/home/test";

	m_map["LVXIN"].szPass = "123456";
	m_map["LVXIN"].szPath = "F:/home/lvxin";
	m_map["RICHMAIN"].szPass = "123456";
	m_map["RICHMAIN"].szPath = "F:/home/richmain";
}

CFtpApp::~CFtpApp()
{

}
//string CFtpApp::GetString(LPSTR s,LPSTR sKey)
//{
//	char* p = strchr(s,' ');
//	if(!p)
//	{
//		p = strstr(s,CRLF);
//		if(!p)return string();
//		strncpy(sKey,s,p-s);
//		_strupr(sKey);
//		return string();
//	}
//	if(sKey)
//	{
//		strncpy(sKey,s,p-s);
//		_strupr(sKey);
//	}
//	while(*p == ' ')
//		p++;
//	string str(p);
//	LPCSTR q = strstr(p,CRLF);
//	if(q)
//		str = str.substr(0,q-p);
//	return str;
//}	
BOOL CFtpApp::InitInstance()
{

	throw TRUE;
}
BOOL CFtpApp::Allow()
{
	return TRUE;
}

BOOL CFtpApp::Start()
{
	CSocket32 sock;
	if(!sock.Create(21))
	{
		cout << "Ftp Listen Port Create Error!" << endl;
		return FALSE;
	}
	sock.Listen();
	char szIP[20];
	UINT nPort;
	CProtocol *pProto = NULL;

	while(true)
	{
		pProto = new CProtocol;
		if(!sock.Accept(pProto ->Socket(),szIP,&nPort))
			break;
		pProto ->Do();//启动子线程

	}
	if(pProto)
		delete pProto;
	return TRUE;
}
/*
	char s[512];
	int i = -1;
	while(++i < 11)
	{
		strcpy(s,g_pSend[i]);
		strcat(s,"\r\n");
		socka.Send(s,strlen(s));
		cout << s ;cout.flush();
		memset(s,0,sizeof(s));
		socka.Receive(s,sizeof(s));
		cout << s ;cout .flush();
	}

	strcpy(s,g_pSend[++i]);
	strcat(s,"\r\n");
	socka.Send(s,strlen(s));
	cout << s ;cout.flush();
	memset(s,0,sizeof(s));
	socka.Receive(s,sizeof(s));
	cout << s ;cout .flush();*/

