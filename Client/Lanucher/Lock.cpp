// Lock.cpp: implementation of the CLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Lock.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLock::CLock(CRITICAL_SECTION *m_cs)
{
    m_pcs=m_cs;
	if(m_pcs)
	{
      EnterCriticalSection(m_pcs);
	}
	
}

CLock::~CLock()
{
   if(m_pcs)
   {
	   LeaveCriticalSection(m_pcs);
   }
   m_pcs=NULL;
}

CLock::CLock(CRITICAL_SECTION &m_cs)
{
	m_pcs=&m_cs;
	EnterCriticalSection(&m_cs);
}

