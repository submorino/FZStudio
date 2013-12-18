#include "stdafx.h"
#include "common.h"
#include "../TinyXML/tinyxml.h"
#include"iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib")

void GetAppRunDir(std::string& strPath)
{
	char buf[MAX_PATH] = {0};
	::GetModuleFileName(NULL, buf, MAX_PATH);

	strPath = buf;
	strPath = strPath.substr(0, strPath.rfind('\\'));
}

void GetPlazaVersion(std::string& strVersion)
{
	// read from site.ini
	char buf[256] = {0};
	std::string strPath;

	GetAppRunDir(strPath);
	strPath += "\\..\\config\\site.ini";

	DWORD nRet = GetPrivateProfileString("Site", "vertion", "4", buf, sizeof(buf)/sizeof(buf[0]), strPath.c_str());
	strVersion = buf;
}

void GetRegisterIdenty(std::string& strRegisterId)
{
	// read from site.ini
	char buf[256] = {0};

	std::string strPath;

	GetAppRunDir(strPath);
	strPath += "\\..\\config\\site.ini";

	DWORD nRet = GetPrivateProfileString("Site", "registerIdentity", "启东市", buf, sizeof(buf)/sizeof(buf[0]), strPath.c_str());
	strRegisterId = buf;
}

void GetMachineID(std::string& strMachineID)
{
	// find where it is. 
}

void GetComputerName(std::string& strName)
{
	char buf[MAX_COMPUTERNAME_LENGTH+1] = {0};
	DWORD nSize = MAX_COMPUTERNAME_LENGTH+1;
	BOOL bRet = ::GetComputerName(buf, &nSize);
	strName = buf;
}

void GetMacAddress(std::string& strMacAddr)
{
}

void GetGameInfo(std::string& strGameInfo)
{
	try
	{
		std::string strPath;
		GetAppRunDir(strPath);
		strPath += "\\..\\config\\ServerTree.xml";
		TiXmlDocument *myDocument = new TiXmlDocument(strPath.c_str());
		if (!myDocument) return;
		myDocument->LoadFile();

		TiXmlElement *RootElement = myDocument->RootElement();
		if (!RootElement) { delete myDocument; return; }
		TiXmlNode *elem = NULL;
		if (elem = RootElement->FirstChild("ServerList"))
		{
			TiXmlElement *pChildElem = elem->FirstChildElement();
			std::string gameid, nowVersion;
			if (pChildElem)
			{
				gameid = pChildElem->Attribute("gameid");
				nowVersion = pChildElem->Attribute("NowVer");
				strGameInfo = gameid + "=" + nowVersion;
			}
			while (pChildElem = pChildElem->NextSiblingElement())
			{
				gameid = pChildElem->Attribute("gameid");
				nowVersion = pChildElem->Attribute("NowVer");
				strGameInfo += "&" + gameid + "=" + nowVersion;
			} 
		}

		delete myDocument;
	}
	catch (...)
	{
	}
}

void Encrypt(const std::string& in, std::string& out)
{
	// Encrpypt here
}

void Decrypt(const std::string& in, std::string& out)
{
	// Decrypt here
}

void GetMacAddress()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter=NULL;
	DWORD dwRetVal=0;
	pAdapterInfo=(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen=sizeof(IP_ADAPTER_INFO);
	if(GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS)
	{
		GlobalFree(pAdapterInfo);
		pAdapterInfo=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}

	if((dwRetVal=GetAdaptersInfo(pAdapterInfo,&ulOutBufLen))==NO_ERROR)
	{
		pAdapter=pAdapterInfo;
		while(pAdapter)
		{
			if(
				strstr(pAdapter->Description,"PCI")>0 //pAdapter->Description中包含"PCI"为：物理网卡
				||pAdapter->Type==71	//pAdapter->Type是71为：无线网卡
				)
			{
				printf("------------------------------------------------------------n");
				printf("AdapterName:t%sn",pAdapter->AdapterName);
				printf("AdapterDesc:t%sn",pAdapter->Description);
				printf("AdapterAddr:t");
				for(UINT i=0;i<pAdapter->AddressLength;i++)
				{
					printf("%02X%c",pAdapter->Address[i],
						i==pAdapter->AddressLength-1?'n':'-');
				}
				printf("AdapterType:t%dn",pAdapter->Type);
				printf("IPAddress:t%sn",pAdapter->IpAddressList.IpAddress.String);
				printf("IPMask:t%sn",pAdapter->IpAddressList.IpMask.String);
			}
			pAdapter=pAdapter->Next;
		}
	}
	else
	{
		printf("CalltoGetAdaptersInfofailed.n");
	}
}