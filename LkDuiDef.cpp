#include <Windows.h>
#include <tchar.h>
#include "LkDuiDef.h"



namespace lkDui
{
	void lkDuiTrace(TCHAR* lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf;
		TCHAR szBuffer[512];

		nBuf = _vsntprintf_s(szBuffer, _countof(szBuffer), lpszFormat, args);

		// was there an error? was the expanded string too long?
		OutputDebugString(szBuffer);

		va_end(args);
	}

	void RC_XMID(const RECT* rc1,RECT* rc2)
	{
		INT tempcx = RC_WIDTH(*rc2);
		rc2->left = (RC_WIDTH(*rc1) - tempcx)/2;
		rc2->right = rc2->left + tempcx;
	}

	void RC_YMID(const RECT* rc1,RECT* rc2)
	{
		INT tempcy = RC_HIGH(*rc2);
		rc2->top = (RC_HIGH(*rc1) - tempcy)/2;
		rc2->bottom = rc2->left + tempcy;
	}
}
