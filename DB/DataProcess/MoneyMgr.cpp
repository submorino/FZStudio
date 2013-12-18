#include "stdafx.h"
#include "MoneyMgr.h"
#include "Lock.h"
#include "DBMgr.h"

CMoneyMgr* CMoneyMgr::m_pMoneyMgr = NULL;

CMoneyMgr::CMoneyMgr()
{
	InitializeCriticalSection(&m_csmoney);
}

CMoneyMgr::~CMoneyMgr()
{
	while(m_money.size())
	{
		money_info money;
		PopMoney(money);
	}

	Save();

	m_pMoneyMgr = NULL;
}

void CMoneyMgr::AddMoney(const money_info& money)
{
	CLock lock(m_csmoney);

	money_info *copymoney = new money_info();
	*copymoney = money;
	m_money.push(copymoney);
	//char buf[128];
	//sprintf(buf, "==%d\n", m_money.size());
	//OutputDebugStringA(buf);
}

void CMoneyMgr::PopMoney(money_info & money)
{
	CLock lock(m_csmoney);

	if(m_money.size() > 0)
	{
		money = *m_money.front();
		m_tosave.push_back(m_money.front());
		m_money.pop();
	}
}

void CMoneyMgr::Save()
{
	put_money_info_req req;
	put_money_info_ack ack;

	std::vector<money_info*>::iterator iter = m_tosave.begin();
	for ( ; iter!=m_tosave.end() ; ++iter)
	{
		req.vectorreq.push_back(*iter);
	}
	DBMGR->put_money_info(req, ack);

	m_tosave.clear();
}