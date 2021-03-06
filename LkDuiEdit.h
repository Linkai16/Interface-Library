#pragma once
#include "LkDuiBaseWnd.h"


namespace lkDui	
{

	class CLkDuiEdit :
		public CLkDuiBaseWnd
	{
	public:
		CLkDuiEdit(Color col);
		~CLkDuiEdit(void);
	public:
		UINT Create(const TCHAR* wnd_name, CLkDuiBase* pBefore, const RECT& rc) override;
		void DrawItem() override;
		/*void NotifyMouseLDown(BOOL bDwon,POINT pt);
		void NotifyMouseRDown(BOOL bDwon,POINT pt);*/
	protected:
		HWND hTxt;
		HANDLE handle_therad_edit_;
		static DWORD WINAPI theditThread(LPVOID lp);
	};
}

