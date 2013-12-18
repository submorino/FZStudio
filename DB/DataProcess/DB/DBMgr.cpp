#include "stdafx.h"
#include "DBMgr.h"
#include "Lock.h"

CDBMgr* CDBMgr::m_pDBMgr = NULL;

CDBMgr::CDBMgr()
{
	InitializeCriticalSection(&m_csLocalDB);

	try
	{
		m_db.Open(".\\local.db");
	}
	catch (CppSQLite3Exception& e)
	{
	}
}

CDBMgr::~CDBMgr()
{
	try
	{
		m_db.Close();
	}
	catch(CppSQLite3Exception& e)
	{
	}

	m_pDBMgr = NULL;
}

bool CDBMgr::put_logon_info(put_logon_info_req & req, put_logon_info_ack & ack)
{
	try
	{
		if (req.vectorreq.empty())
		{	//如果传进来的参数req没有记录的话，则返回错误
			ack.lRetCode = 0;
			return ack.lRetCode;
		}

		CLock lock(m_csLocalDB);

		m_db.BeginTransaction();

		std::vector<logon_info*>::iterator curitem;
		for (curitem=req.vectorreq.begin(); curitem!=req.vectorreq.end(); curitem++)
		{
			m_db.Cmd("select * from logon_table where id=%i", (*curitem)->id);
			if (m_db.More())
			{
				while (m_db.More()) {}
				//更新表中记录
				m_db.Cmd("update logon_table set logonname='%s', password='%s' where id=%i",
					(*curitem)->logonname.c_str(), (*curitem)->password.c_str(), (*curitem)->id);
				m_db.Exec();
			}
			else
			{
				//往表插入新记录
				m_db.Cmd("insert into logon_table(id, logonname, password) values(%i, '%s', '%s')", 
					(*curitem)->id, (*curitem)->logonname.c_str(), (*curitem)->password.c_str());
				m_db.Exec();
			}
		}

		m_db.Commit();

		ack.lRetCode = 1;
		return ack.lRetCode;
	}
	catch (CppSQLite3Exception& e)
	{
		//throw e;
		m_db.Rollback();
		//LOG_ERROR_INFO(e.ErrorCodeAsString(e.ErrorCode()));
		ack.lRetCode =  0;
		return ack.lRetCode;
	}
}

bool CDBMgr::get_logon_info(get_logon_info_req & req, get_logon_info_ack & ack)
{
	try
	{
		m_db.Cmd("select * from logon_table");
		m_db.Exec();
		while (m_db.More())
		{
			logon_info *account = new logon_info();
			account->id = m_db.GetLong(0);
			account->logonname = m_db.Get(1);
			account->password = m_db.Get(2);
			ack.vectorack.push_back(account);
		}
	}
	catch(CppSQLite3Exception& e)
	{
		return false;
	}

	return true;
}

bool CDBMgr::put_player_info(put_player_info_req & req, put_player_info_ack & ack)
{
	return false;
}

bool CDBMgr::get_player_info(get_player_info_req & req, get_player_info_ack & ack)
{
	return false;
}

bool CDBMgr::put_money_info(put_money_info_req & req, put_money_info_ack & ack)
{
	try
	{
		if (req.vectorreq.empty())
		{	//如果传进来的参数req没有记录的话，则返回错误
			ack.lRetCode = 0;
			return ack.lRetCode;
		}

		CLock lock(m_csLocalDB);

		m_db.BeginTransaction();

		std::vector<money_info*>::iterator curitem;
		for (curitem=req.vectorreq.begin(); curitem!=req.vectorreq.end(); curitem++)
		{
			m_db.Cmd("select * from money_table where id=%i", (*curitem)->id);
			if (m_db.More())
			{
				while (m_db.More()) {}
				//更新表中记录
				m_db.Cmd("update money_table set score=%i, gold=%i, ticketcount=%i where id=%i",
					(*curitem)->score, (*curitem)->gold, (*curitem)->ticketcount, (*curitem)->id);
				m_db.Exec();
			}
			else
			{
				//往表插入新记录
				m_db.Cmd("insert into money_table(id, score, gold, ticketcount) values(%i, %i, %i, %i)", 
					(*curitem)->id, (*curitem)->score, (*curitem)->gold, (*curitem)->ticketcount);
				m_db.Exec();
			}
		}

		m_db.Commit();

		ack.lRetCode = 1;
		return ack.lRetCode;
	}
	catch (CppSQLite3Exception& e)
	{
		//throw e;
		m_db.Rollback();
		//LOG_ERROR_INFO(e.ErrorCodeAsString(e.ErrorCode()));
		ack.lRetCode =  0;
		return ack.lRetCode;
	}
}

bool CDBMgr::get_money_info(get_money_info_req & req, get_money_info_ack & ack)
{
	try
	{
		m_db.Cmd("select * from money_table");
		m_db.Exec();
		while (m_db.More())
		{
			money_info *money = new money_info();
			money->id = m_db.GetLong(0);
			money->score = m_db.GetLong(1);
			money->gold = m_db.GetLong(2);
			money->ticketcount = m_db.GetLong(3);

			ack.vectorack.push_back(money);
		}
	}
	catch(CppSQLite3Exception& e)
	{
		return false;
	}

	return true;
}
