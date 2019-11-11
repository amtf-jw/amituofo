// Time.cpp: implementation of the CTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Time.h"
#include <assert.h>
#include <string.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTime::CTime()
{

}

CTime::~CTime()
{

}
CTime::CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST)
{
	tm tmTime;
	tmTime.tm_sec = nSec;
	tmTime.tm_min = nMin;
	tmTime.tm_hour = nHour;
	assert(nDay >= 1 && nDay <= 31);
	tmTime.tm_mday = nDay;
	assert(nMonth >= 1 && nMonth <= 12);//ÊäÈë13ÔÂ
	tmTime.tm_mon = nMonth-1;
	assert(nYear >= 1900);
	tmTime.tm_year = nYear-1900;
	tmTime.tm_isdst= nDST;
	m_time = mktime(&tmTime);
	assert(m_time != -1);       // indicates an illegal input time

}



int CTime::GetYear()
{
	tm *ptm = localtime(&m_time);
	return ptm ->tm_year+1900;
}

CTime::CTime(time_t time)
{
	m_time = time;
}

int CTime::GetMonth()
{
	return localtime(&m_time) ->tm_mon+1;
}

int CTime::GetDay()
{
	return localtime(&m_time) ->tm_mday;
}

void CTime::Format(char* szDest,const char* szFormat)
{
	char szBuffer[64];

	struct tm* ptmTemp = localtime(&m_time);
	if (ptmTemp == NULL ||
		!strftime(szBuffer, sizeof(szBuffer), szFormat, ptmTemp))
		szBuffer[0] = '\0';
	strcpy(szDest,szBuffer);
}

CTime CTime::GetCurTime()
{
	return time(NULL);
}
