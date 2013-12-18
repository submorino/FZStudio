#pragma once

#include <string>
#include <map>
#include <vector>


void GetPlazaVersion(std::string& strVersion);

void GetRegisterIdenty(std::string& strRegisterId);

void GetMachineID(std::string& strMachineID);

void GetComputerName(std::string& strName);

void GetMacAddress(std::string& strMacAddr);

void GetGameInfo(std::string& strGameInfo);

void Encrypt(const std::string& in, std::string& out);

void Decrypt(const std::string& in, std::string& out);

void GetMacAddress();
