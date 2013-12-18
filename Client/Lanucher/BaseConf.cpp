#include "StdAfx.h"
#include "BaseConf.h"

CBaseConf::CBaseConf()
: m_filepath("")
{
}

CBaseConf::CBaseConf(const std::string& filepath)
: m_filepath(filepath)
{
}

CBaseConf::~CBaseConf()
{
}
