// FtpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include  "FtpApp.h"
#include "socket32.h"
#include <iostream>
using namespace std;

/*
Recv2=USER test
Recv4=PASS test
Recv6=opts utf8 on
Recv8=syst
Recv10=site help
Recv12=PWD
Recv14=TYPE A
Recv16=PASV
Recv18=TYPE A
Recv20=PORT 127,0,0,1,4,84
Recv22=LIST

*/
/*const char* g_pSend[]=
{
"220 127.0.0.1 FtpD for free",
"331 Password required for test .",
"230 User test logged in , proceed",
"502 Command OPTS utf8 on not implemented",
"215 UNIX",
"502 Command SITE help not implemented",
"257 \"subling/\" is current directory",
"200 Type set to ASCII.",
"425 Cannot open data connection (192.168.1.13 0 create failed)",
"200 Type set to ASCII.",
"200 Port command okay.",
"150 Opening ASCII NO-PRINT mode data connection for ls -l.",//中间不收
"226 Transfer finished successfully. Data connection closed.",
};*/
CFtpApp theApp;

int main(int argc, char* argv[])
{
	try
	{
	if(!theApp.InitInstance())
		return -1;
	}catch(...)
	{
	
	}
	if(!theApp.Start())
		return -1;
	return 0;
}

