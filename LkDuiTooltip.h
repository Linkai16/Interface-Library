#pragma once
#include "LkDuiBaseWnd.h"

namespace lkDui
{
	class CLkDuiTooltip :
		public CLkDuiBaseWnd
	{
	public:
		CLkDuiTooltip(void);
		~CLkDuiTooltip(void);
	public:
		virtual UINT Show(const POINT& pt,DWORD dwMaxWide = 800);
		virtual void Hide();
		virtual void SetShowText(TCHAR* szInfo);
		virtual void DrawItem();
	private:
		virtual UINT Create(const TCHAR* wnd_name, CLkDuiBase* pBefore, const RECT& rc) override;
		static DWORD WINAPI thRunMainWnd(LPVOID lparam);
		POINT show_pt_;
		DWORD Max_Wide_;
		HANDLE hWndThread_;
		TCHAR  font_name_[80];
		float  font_size_;
	};
}

