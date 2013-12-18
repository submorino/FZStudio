#include "stdafx.h"
#include "InterOps.h"
#include "MoneyMgr.h"

void AddMoney(const money_info & money)
{
	MONEYMGR->AddMoney(money);
}

void AddPlayer(const player_info & player)
{
}

void ReadLogonData(std::vector<logon_info*> & logons)
{
	
}