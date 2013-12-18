#pragma once

#include "CppSQLite3.h"
#include "MyStruct.h"
#include "mdbStruct.h"


class CDBMgr
{
public:
	~CDBMgr();

	static CDBMgr* GetInstance() { return m_pDBMgr != NULL ? m_pDBMgr : m_pDBMgr = new CDBMgr; };

public:
	bool put_logon_info(put_logon_info_req & req, put_logon_info_ack & ack);
	bool get_logon_info(get_logon_info_req & req, get_logon_info_ack & ack);

	bool put_player_info(put_player_info_req & req, put_player_info_ack & ack);
	bool get_player_info(get_player_info_req & req, get_player_info_ack & ack);

	bool put_money_info(put_money_info_req & req, put_money_info_ack & ack);
	bool get_money_info(get_money_info_req & req, get_money_info_ack & ack);

private:
	CDBMgr();

private:
	static CDBMgr *m_pDBMgr;

	CppSQLite3DB m_db;

	CRITICAL_SECTION    m_csLocalDB;	//localDBËø
};

#define DBMGR	CDBMgr::GetInstance()