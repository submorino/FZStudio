#pragma once
#include <queue>
#include "MyStruct.h"

class CMoneyMgr
{
public:
	~CMoneyMgr();

public:
	static CMoneyMgr* GetInstance() { return m_pMoneyMgr != NULL ? m_pMoneyMgr : m_pMoneyMgr = new CMoneyMgr; };

	void AddMoney(const money_info& money);

	void PopMoney(money_info & money);

	void Save();

private:
	CMoneyMgr();

private:
	static CMoneyMgr *m_pMoneyMgr;

	std::queue<money_info*> m_money;

	std::vector<money_info*> m_tosave;

	CRITICAL_SECTION m_csmoney;
};

#define MONEYMGR CMoneyMgr::GetInstance()