// Protocol.cpp: implementation of the CProtocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Protocol.h"
#include "FtpApp.h"
#include "time.h"
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const char *monthStr [] = 
{
	NULL,
	"Jan"
	,"Feb"
	,"Mar"
	,"Apr"
	,"May"
	,"Jun"
	,"Jul"
	,"Aug"
	,"Sep"
	,"Oct"
	,"Dec"
	,"Nov"
};
CProtocol::CProtocol():m_log(m_socka)
{
	m_bRun = TRUE;
	m_eType = A;
}

CProtocol::~CProtocol()
{

}


void CProtocol::Do()
{
	string str;
	if(!theApp.Allow())
	{
		str = "Permitted to access.";
		m_socka.Send(str.c_str(),str.length());
		delete this;
		return;
	}
	str = string("220 ") + "Richftp ftp (Version 3.0)." + CRLF;
	m_socka.Send(str.c_str(),str.length());

	_beginthread(theProc,0,this);
}

void CProtocol::theProc(void *p)
{
	CProtocol* pProto = (CProtocol*)p;
	pProto ->Run();
	delete pProto;
	_endthread();
}


BOOL CProtocol::Run()
{
	int nLen = 0;
	while(m_bRun)
	{
		char s[256] = {0};
		nLen = m_socka.Receive(s,sizeof(s));
		if(nLen < 0)
			return FALSE;

		const char* sKey = strtok(s," \r\n");
		const char* data = strtok(NULL, " \r\n");
		cout << s << endl;
		if(!m_log.IsLoged())
		{
			if(!m_log.Login(sKey,data))
				return FALSE;
			continue;
		}
		string str;
		if (data)
			str = data;
		if(!Process(sKey,str))
			return FALSE;
	}
	return TRUE;
}
BOOL CProtocol::SYST(LPCSTR sKey,string &str)
{
#ifdef _WIN32
	str = "215 Windows";
#else
	str = "215 LINUX";
#endif
	str += CRLF;
	m_socka.Send(str.c_str(),str.length());
	return TRUE;
}

BOOL CProtocol::Unknown(LPCSTR sKey,string &str)
{
	string szUnk("502 ");
	szUnk += sKey;
	szUnk += ' ';
	szUnk += str + " not implemented" + CRLF;
	m_socka.Send(szUnk.c_str(),szUnk.length());
	return TRUE;
}
BOOL CProtocol::IsPath(string str)
{
	str = m_log.GetPath()+str;
	_finddata_t fd;
	long lf = _findfirst(str.c_str(),&fd);
	if(-1 == lf)
		return FALSE;
	_findclose(lf);
	return TRUE;
}
BOOL CProtocol::CWD(LPCSTR sKey,string &str)
{
	if(m_szCWD.empty())
	{
		m_szCWD = "/";
	}
	if(str == "/" || str == ".")
	{
		m_szCWD = "/";
		str = string("250 ") + "\"" + m_szCWD + "\" is current directory" + CRLF;
		return 	m_socka.Send(str.c_str(),str.length()) > 0;
	}
	if(str == "..")
	{
		int i = m_szCWD.rfind("/");
		if(i > 0)
			m_szCWD = m_szCWD.substr(0,i-1);
		str = string("250 ") + "\"" + m_szCWD + "\" is current directory" + CRLF;
		return 	m_socka.Send(str.c_str(),str.length()) > 0;
	}
	if(str.c_str()[str.length()-1] == '/')
		str = str.substr(0,str.length()-1);
	if(str.c_str()[0] - '/')
		str = m_szCWD + str ;
	if(IsPath(str))
	{
		m_szCWD = str + "/";
		str = string("250 ") + "\"" + m_szCWD + "\" is current directory" + CRLF;
	}
	else
	{
		str = "550 Cannot change directory " + str + CRLF;
	}
	m_socka.Send(str.c_str(),str.length());
	return TRUE;
}

BOOL CProtocol::PWD(LPCSTR sKey,string &str)
{
	if(m_szCWD.empty())
	{
		m_szCWD = "/";
		_finddata_t fd;
		long lf = _findfirst(m_log.GetPath().c_str(),&fd);
		if(-1 == lf)
		{
			str = string("503 Root path not access.") + CRLF;
			m_socka.Send(str.c_str(),str.length());
			return TRUE;
		}
	}
	str = string("257 ") + "\"" + m_szCWD + "\" is current directory" + CRLF;
	m_socka.Send(str.c_str(),str.length());

	return TRUE;
}
BOOL CProtocol::TYPE(LPCSTR sKey,string &str)
{
	m_eType = (EType)str.c_str()[0];
	if(m_eType == A)
		str = string("200 text mode is set ")  + CRLF;
	else
		str = string("200 binary mode is set ")  + CRLF;
	m_socka.Send(str.c_str(),str.length());

	return TRUE;
}
BOOL CProtocol::PASV(LPCSTR sKey,string &str)
{
	m_sockp.Close();
	if(!m_sockp.Create())
	{
		str = string("425 Passive socket create failed!") + CRLF;
		return 	m_socka.Send(str.c_str(),str.length()) > 0;
	}
	char szIP[20];
	UINT nPort;
	m_socka.GetSockName(szIP,nPort);
	char * p = szIP;
	while(p = strchr(p,'.'))
		*p = ',';
	str = szIP;
	str += ',';
	m_sockp.GetSockName(szIP,nPort);
	sprintf(szIP,"%d",HIBYTE(nPort));
	str += szIP;
	sprintf(szIP,"%d",LOBYTE(nPort));
	str += ',';
	str += szIP;
	str = "227 Entering Passive Mode ("+str+")" + CRLF;
	m_socka.Send(str.c_str(),str.length());
	m_sockp.Listen();
	m_sockp.Accept(m_sockt);   
	return TRUE;
}
string CProtocol::GetTime(time_t ttime)
{
	string str;
	CTime time(ttime);
	char sTime[64] = {0};
	if(time.GetYear() == CTime::GetCurTime().GetYear())
		time.Format(sTime," %d %H:%m:%S ");
	else
		time.Format(sTime," %d %Y ");
	str += monthStr[time.GetMonth()];
	str += sTime;
	return str;


}
BOOL CProtocol::NLST(LPCSTR sKey,string &str)
{
	if(str.empty())
		str = m_log.GetPath() + m_szCWD + "/*.*";
	else
		str = m_log.GetPath() + m_szCWD + "/" + str + "/*.*";
	_finddata_t fd;
	long lf = _findfirst(str.c_str(),&fd);
	str = "150 File status okay"+ string(CRLF) ;
	m_socka.Send(str.c_str(),str.length());
	if(-1 - lf)
	{
		
		do
		{
			str = (fd.attrib & _A_SUBDIR)?'d':'-';
			str += "rwxrwxrwx 1 ";
			str += m_log.GetUser();
			str += "  nogroup ";
			char szSize[15];
			sprintf(szSize,"%d ",fd.size);
			str += szSize;
			str += GetTime(fd.time_write);
		
			//str += "Sep 2 02:20 ";
			str += fd.name;
			str += CRLF;
			m_sockt.Send(str.c_str(),str.length());
		}while( !_findnext( lf, &fd ));
	_findclose(lf);	
	}

	m_sockt.Close();
	m_sockp.Close();
	str = "226 Transfer finished successfully. Data connection closed" + string(CRLF);
	m_socka.Send(str.c_str(),str.length());

	return TRUE;
}
BOOL CProtocol::NOOP(LPCSTR sKey,string &str)
{
	str = "200 Command Ok , NOOP" + string(CRLF);
	return m_socka.Send(str.c_str(),str.length()) > 0 ;
}

BOOL CProtocol::DELE(LPCSTR sKey,string &str)
{
	str = m_log.GetPath() + m_szCWD + str;
	if(remove(str.c_str()) < 0)
		str = "553 DELE file failed." + string(CRLF);
	else
		str = "250 DELETE file success." + string(CRLF);
	m_socka.Send(str.c_str(),str.length());
	return TRUE;
}

BOOL CProtocol::RNFR(LPCSTR sKey,string &str)
{
	m_szRename = str;
	str = "350 Requested file new name." + string(CRLF);
	m_socka.Send(str.c_str(),str.length());

	return TRUE;
}
BOOL CProtocol::MKD(LPCSTR sKey,string &str)
{
	str = m_log.GetPath() + m_szCWD + str;
	if(_mkdir(str.c_str()) < 0)
		str = "553 Create directory failed." + string(CRLF);
	else
		str = "250 Create directory success." + string(CRLF);
	m_socka.Send(str.c_str(),str.length());
	return TRUE;
}

BOOL CProtocol::RNTO(LPCSTR sKey,string &str)
{
	string szOld = m_log.GetPath() + "/"+m_szCWD + m_szRename;
	string szNew = m_log.GetPath() + "/"+m_szCWD + str;
	if(!rename(szOld.c_str(),szNew.c_str()))
	{
		str = "250 file renamed frome " +m_szRename + "to " + str +CRLF;
		return m_socka.Send(str.c_str(),str.length()) > 0;
	}
	int i = GetLastError();
	str = "553 file rename failed:" + m_szRename + "." + CRLF;
	return m_socka.Send(str.c_str(),str.length()) > 0;
}
BOOL CProtocol::RMD(LPCSTR sKey,string &str)
{
	str = m_log.GetPath() + "/" + m_szCWD + str;
	if(!_rmdir(str.c_str()))
	{
		str = "250 file remove directory success." + string(CRLF);
		return m_socka.Send(str.c_str(),str.length()) > 0;
	}
	str = "553 file remove directory failed." + string(CRLF);
	return m_socka.Send(str.c_str(),str.length()) > 0;
}

BOOL CProtocol::Process(LPCSTR sKey,string &str)
{
	if(!strcmp(sKey,"SYST"))
		return SYST(sKey,str);
	
	if(!strcmp(sKey,"XPWD") || !strcmp(sKey,"PWD"))
		return PWD(sKey,str);
	if(!strcmp(sKey,"CWD"))
		return CWD(sKey,str);
	if(!strcmp(sKey,"TYPE"))
		return TYPE(sKey,str);
	if(!strcmp(sKey,"PASV"))
		return PASV(sKey,str);
	if(!strcmp(sKey,"NLST"))
		return NLST(sKey,str);
	if(!strcmp(sKey,"LIST"))
		return NLST(sKey,str);
	if(!strcmp(sKey,"NOOP"))
		return NOOP(sKey,str);
	if(!strcmp(sKey,"DELE"))
		return DELE(sKey,str);
	if(!strcmp(sKey,"RNFR"))
		return RNFR(sKey,str);
	if(!strcmp(sKey,"RNTO"))
		return RNTO(sKey,str);
	if(!strcmp(sKey,"MKD") ||!strcmp(sKey,"XMKD"))
		return MKD(sKey,str);
	if(!strcmp(sKey,"RMD")||!strcmp(sKey,"XRMD"))
		return RMD(sKey,str);
	return Unknown(sKey,str);
}
