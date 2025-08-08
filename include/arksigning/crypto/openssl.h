#pragma once
#include "utils/json.h"
#include "modern/optional.h"

bool GetCertSubjectCN(const string &strCertData, string &strSubjectCN);
bool GetCMSInfo(uint8_t *pCMSData, uint32_t uCMSLength, JValue &jvOutput);
bool GetCMSContent(const string &strCMSDataInput, string &strContentOutput);
bool GenerateCMS(const string &strSignerCertData, const string &strSignerPKeyData, const string &strCDHashData, const string &strCDHashPlist, string &strCMSOutput);

// Modern optional-based versions for safer null handling
Optional<string> GetCertSubjectCNOptional(const string &strCertData);
Optional<string> GetCMSContentOptional(const string &strCMSDataInput);
Optional<string> GenerateCMSOptional(const string &strSignerCertData, const string &strSignerPKeyData, const string &strCDHashData, const string &strCDHashPlist);

class arksigningAsset
{
public:
	arksigningAsset();

public:
	bool GenerateCMS(const string &strCDHashData, const string &strCDHashesPlist, const string &strCodeDirectorySlotSHA1, const string &strAltnateCodeDirectorySlot256, string &strCMSOutput);
	bool Init(const string &strSignerCertFile, const string &strSignerPKeyFile, const string &strProvisionFile, const string &strEntitlementsFile, const string &strPassword);

public:
	string m_strTeamId;
	string m_strSubjectCN;
	string m_strProvisionData;
	string m_strEntitlementsData;

private:
	void *m_evpPKey;
	void *m_x509Cert;
};