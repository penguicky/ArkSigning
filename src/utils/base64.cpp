#include "utils/base64.h"
#include "utils/constants.h"
#include <string.h>

#define B0(a) (a & 0xFF)
#define B1(a) (a >> 8 & 0xFF)
#define B2(a) (a >> 16 & 0xFF)
#define B3(a) (a >> 24 & 0xFF)

ZBase64::ZBase64(void)
{
}

ZBase64::~ZBase64(void)
{
	// Smart pointers automatically clean up, no manual deletion needed
	m_arrEnc.clear();
	m_arrDec.clear();
}

ZBase64::ZBase64(ZBase64 &&other) noexcept
	: m_arrDec(std::move(other.m_arrDec)), m_arrEnc(std::move(other.m_arrEnc))
{
	// other's vectors are now empty due to move
}

ZBase64 &ZBase64::operator=(ZBase64 &&other) noexcept
{
	if (this != &other)
	{
		m_arrDec = std::move(other.m_arrDec);
		m_arrEnc = std::move(other.m_arrEnc);
		// other's vectors are now empty due to move
	}
	return *this;
}

char ZBase64::GetB64char(int nIndex)
{
	static const char szTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	if (nIndex >= 0 && nIndex < 64)
	{
		return szTable[nIndex];
	}
	return '=';
}

int ZBase64::GetB64Index(char ch)
{
	int index = -1;
	if (ch >= 'A' && ch <= 'Z')
	{
		index = ch - 'A';
	}
	else if (ch >= 'a' && ch <= 'z')
	{
		index = ch - 'a' + 26;
	}
	else if (ch >= '0' && ch <= '9')
	{
		index = ch - '0' + 52;
	}
	else if (ch == '+')
	{
		index = 62;
	}
	else if (ch == '/')
	{
		index = 63;
	}
	return index;
}

const char *ZBase64::Encode(const char *szSrc, int nSrcLen)
{
	if (0 == nSrcLen)
	{
		nSrcLen = (int)strlen(szSrc);
	}

	if (nSrcLen <= 0)
	{
		return "";
	}

	auto szEnc = unique_ptr<char[]>(new char[nSrcLen * 3 + ArkSigning::Constants::BASE64_BUFFER_PADDING]);
	char *p64 = szEnc.get();
	m_arrEnc.push_back(std::move(szEnc));

	int i = 0;
	unsigned char *psrc = reinterpret_cast<unsigned char*>(const_cast<char*>(szSrc));
	for (i = 0; i < nSrcLen - 3; i += 3)
	{
		unsigned long ulTmp = *reinterpret_cast<unsigned long*>(psrc);
		int b0 = GetB64char((B0(ulTmp) >> 2) & 0x3F);
		int b1 = GetB64char((B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F);
		int b2 = GetB64char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F);
		int b3 = GetB64char((B2(ulTmp) << 2 >> 2) & 0x3F);
		*reinterpret_cast<unsigned long*>(p64) = b0 | b1 << 8 | b2 << 16 | b3 << 24;
		p64 += 4;
		psrc += 3;
	}

	if (i < nSrcLen)
	{
		int rest = nSrcLen - i;
		unsigned long ulTmp = 0;
		for (int j = 0; j < rest; ++j)
		{
			*(reinterpret_cast<unsigned char*>(&ulTmp) + j) = *psrc++;
		}
		p64[0] = GetB64char((B0(ulTmp) >> 2) & 0x3F);
		p64[1] = GetB64char((B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F);
		p64[2] = rest > 1 ? GetB64char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F) : '=';
		p64[3] = rest > 2 ? GetB64char((B2(ulTmp) << 2 >> 2) & 0x3F) : '=';
		p64 += 4;
	}
	*p64 = '\0';
	return m_arrEnc.back().get();
}

const char *ZBase64::Encode(const string &strInput)
{
	return Encode(strInput.data(), strInput.size());
}

const char *ZBase64::Decode(const char *szSrc, int nSrcLen, int *pDecLen)
{
	if (0 == nSrcLen)
	{
		nSrcLen = (int)strlen(szSrc);
	}

	if (nSrcLen <= 0)
	{
		return "";
	}

	auto szDec = unique_ptr<char[]>(new char[nSrcLen]);
	char *pbuf = szDec.get();
	m_arrDec.push_back(std::move(szDec));

	int i = 0;
	unsigned char *psrc = reinterpret_cast<unsigned char*>(const_cast<char*>(szSrc));
	for (i = 0; i < nSrcLen - 4; i += 4)
	{
		unsigned long ulTmp = *reinterpret_cast<unsigned long*>(psrc);

		int b0 = (GetB64Index(static_cast<char>(B0(ulTmp))) << 2 | GetB64Index(static_cast<char>(B1(ulTmp))) << 2 >> 6) & 0xFF;
		int b1 = (GetB64Index(static_cast<char>(B1(ulTmp))) << 4 | GetB64Index(static_cast<char>(B2(ulTmp))) << 2 >> 4) & 0xFF;
		int b2 = (GetB64Index(static_cast<char>(B2(ulTmp))) << 6 | GetB64Index(static_cast<char>(B3(ulTmp))) << 2 >> 2) & 0xFF;

		*reinterpret_cast<unsigned long*>(pbuf) = b0 | b1 << 8 | b2 << 16;
		psrc += 4;
		pbuf += 3;
	}

	if (i < nSrcLen)
	{
		int rest = nSrcLen - i;
		unsigned long ulTmp = 0;
		for (int j = 0; j < rest; ++j)
		{
			*(reinterpret_cast<unsigned char*>(&ulTmp) + j) = *psrc++;
		}

		int b0 = (GetB64Index(static_cast<char>(B0(ulTmp))) << 2 | GetB64Index(static_cast<char>(B1(ulTmp))) << 2 >> 6) & 0xFF;
		*pbuf++ = b0;

		if ('=' != B1(ulTmp) && '=' != B2(ulTmp))
		{
			int b1 = (GetB64Index(static_cast<char>(B1(ulTmp))) << 4 | GetB64Index(static_cast<char>(B2(ulTmp))) << 2 >> 4) & 0xFF;
			*pbuf++ = b1;
		}

		if ('=' != B2(ulTmp) && '=' != B3(ulTmp))
		{
			int b2 = (GetB64Index(static_cast<char>(B2(ulTmp))) << 6 | GetB64Index(static_cast<char>(B3(ulTmp))) << 2 >> 2) & 0xFF;
			*pbuf++ = b2;
		}
	}
	*pbuf = '\0';

	if (nullptr != pDecLen)
	{
		*pDecLen = static_cast<int>(pbuf - m_arrDec.back().get());
	}

	return m_arrDec.back().get();
}

const char *ZBase64::Decode(const char *szSrc, string &strOutput)
{
	strOutput.clear();
	int nDecLen = 0;
	const char *p = Decode(szSrc, 0, &nDecLen);
	strOutput.append(p, nDecLen);
	return strOutput.data();
}
