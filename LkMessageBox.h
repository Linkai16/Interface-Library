#pragma once
#include "LkDuiDef.h"
#include "LkDuiCore.h"
#include "LkDuiBaseWnd.h"
#include "LkDuiImgButton.h"

namespace lkDui
{
#define MSGBOX_TITLE_LEN  (80)
	class CLkMessageBox
	{
	public:
		CLkMessageBox(HWND hParent = NULL);
		~CLkMessageBox(void);
	public:
		UINT DoModule(TCHAR* tInfo,TCHAR* tTitle,UINT uType,DWORD dwBgID);
		static UINT CALLBACK  ButtonClick(CLkDuiBase* trage,POINT pt);
		static DWORD WINAPI   thCreateWnd(LPVOID);
	protected:
		DWORD					background_image_id_;
		HWND					hParent_;
		CLkDuiBaseWnd			main_wnd_;
		CLkDuiImgButton			ok_bt_;
		CLkDuiImgButton			clean_bt_;
		BOOL					is_created_;
		HANDLE                  hth_createwnd_;

		// 显示内容
		DWORD					w_Bg_ID_;
		UINT					u_Type_;
		TCHAR*                  p_tInfo_;
		TCHAR                   tTitle_[MSGBOX_TITLE_LEN];
	};
}

