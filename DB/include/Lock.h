// Lock.h: interface for the CLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCK_H__B7C06BD5_0890_4BBA_82FE_45FE636A57E8__INCLUDED_)
#define AFX_LOCK_H__B7C06BD5_0890_4BBA_82FE_45FE636A57E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLock  
{
//construction	
public:
	CLock(CRITICAL_SECTION &m_cs);
	CLock(CRITICAL_SECTION *m_cs);
    ~CLock();
//attributes
private:
	CRITICAL_SECTION *m_pcs;

};

#endif // !defined(AFX_LOCK_H__B7C06BD5_0890_4BBA_82FE_45FE636A57E8__INCLUDED_)
