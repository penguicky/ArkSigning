#pragma once

#include <string>
#include <vector>
#include <memory>

using namespace std;

class ZBase64
{
public:
	ZBase64(void);
	~ZBase64(void);

	// Move semantics
	ZBase64(ZBase64 &&other) noexcept;
	ZBase64 &operator=(ZBase64 &&other) noexcept;

	// Disable copy semantics (can be enabled if needed)
	ZBase64(const ZBase64 &) = delete;
	ZBase64 &operator=(const ZBase64 &) = delete;

public:
	const char *Encode(const char *szSrc, int nSrcLen = 0);
	const char *Encode(const string &strInput);
	const char *Decode(const char *szSrc, int nSrcLen = 0, int *pDecLen = nullptr);
	const char *Decode(const char *szSrc, string &strOutput);

private:
	inline int GetB64Index(char ch);
	inline char GetB64char(int nIndex);

private:
	vector<unique_ptr<char[]>> m_arrDec;
	vector<unique_ptr<char[]>> m_arrEnc;
};
