// Time.h: interface for the CTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIME_H__2716A686_73D2_4652_B1F3_CDF32431871F__INCLUDED_)
#define AFX_TIME_H__2716A686_73D2_4652_B1F3_CDF32431871F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <time.h>

class CTime  
{
	time_t m_time;
public:
	static CTime GetCurTime();
	int GetDay();
	int GetMonth();
	void Format(char* szDest,const char* szFormat);
	CTime( time_t time );
	int GetYear();
	CTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1 );
	CTime();
	virtual ~CTime();
};

#endif // !defined(AFX_TIME_H__2716A686_73D2_4652_B1F3_CDF32431871F__INCLUDED_)
