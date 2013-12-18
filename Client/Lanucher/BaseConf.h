#pragma once

#include <string>

class CBaseConf
{
public:
	CBaseConf();
	CBaseConf(const std::string& filepath);
	virtual ~CBaseConf();

protected:
	virtual bool LoadConf() = 0;

protected:
	std::string m_filepath;
};
