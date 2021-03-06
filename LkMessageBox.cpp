#include "LkMessageBox.h"
#include "../resource.h"


namespace lkDui
{
	CLkMessageBox::CLkMessageBox(HWND hParent) :
hParent_(hParent),
main_wnd_(Color(0,0,0,0)),
ok_bt_(Color(0,0,0,0)),
clean_bt_(Color(0,0,0,0)),
is_created_(FALSE),
hth_createwnd_(NULL),
// msgbox内容
w_Bg_ID_(NULL),
u_Type_(MB_OK),
p_tInfo_(NULL)
	{
		ZeroMemory(tTitle_,sizeof(tTitle_));
	}

	CLkMessageBox::~CLkMessageBox(void)
	{

	}

	UINT CALLBACK CLkMessageBox::ButtonClick(CLkDuiBase* trage,POINT pt)
	{
		CLkDuiBase* pParent = trage->GetParent();
		if (!pParent)
			return 1;
		if (trage->IsWnd(_T("bt_jx")))
			pParent->SetData((LPVOID)IDYES);
		else
			pParent->SetData((LPVOID)IDNO);
		PostMessage(pParent->GetMainHWnd(), WM_DESTROY, 0, 0);
		return 0;
	}
	UINT CLkMessageBox::DoModule(TCHAR* tInfo,TCHAR* tTitle,UINT uType,DWORD dwBgID)
	{
		int nLen = _tcslen(tInfo)+1;
		p_tInfo_ = new TCHAR[nLen];
		_tcscpy(p_tInfo_,tInfo);
		_tcscpy(tTitle_,tTitle);
		u_Type_ = uType;
		w_Bg_ID_ = dwBgID;
		::EnableWindow(hParent_, FALSE);
		hth_createwnd_ = CreateThread(NULL,0,thCreateWnd,this,NULL,NULL);
		if (hth_createwnd_)
		{
			do 
			{
				Sleep(1);
			} while (!is_created_);
			SetWindowPos(main_wnd_.GetMainHWnd(),HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			WaitForSingleObject(hth_createwnd_,0xFFFFFFFF);
		}
		::EnableWindow(hParent_, TRUE);
		delete p_tInfo_;
		return (UINT)main_wnd_.GetData();
	}


	DWORD CLkMessageBox::thCreateWnd(LPVOID lpvoid)
	{
		CLkMessageBox* pThis = (CLkMessageBox*)lpvoid;
		if (pThis)
		{
			RECT rcScreen = {0,0,GetSystemMetrics ( SM_CXSCREEN ) ,GetSystemMetrics ( SM_CYSCREEN ) };
			RECT rc1 = {0,0,462,277};
			RC_XMID(&rcScreen,&rc1);
			RC_YMID(&rcScreen,&rc1);
			pThis->main_wnd_.Create(_T("MainMsgbox"),NULL,rc1);
			pThis->main_wnd_.SetBackground(pThis->w_Bg_ID_,TRUE,FALSE);


			if (pThis->u_Type_ == MB_OK)
			{
				pThis->ok_bt_.Create(_T("bt_jx"),&pThis->main_wnd_,290,190,IDB_PNG_BT_MSG_OK,121);
				pThis->ok_bt_.AddCallback(ButtonClick);
				pThis->ok_bt_.ShowWnd(TRUE);
			}
			else if (pThis->u_Type_ == MB_YESNO)
			{
				pThis->ok_bt_.Create(_T("bt_jx"),&pThis->main_wnd_,240,220,IDB_PNG_BT_BTNBG,185);
				pThis->ok_bt_.SetTxtColor(Color(0xfe,0x4c,0x62,0x86),Color(0xfe,0xff,0xff,0xff),Color(0xfe,0x00,0x34,0x71));	
				pThis->ok_bt_.SetFont(_T("微软雅黑"),11.0f);
				pThis->ok_bt_.SetWndText(_T("继续安装"),HORIZONTAL_CENTER|VERTICAL_CENTER);
				pThis->ok_bt_.AddCallback(ButtonClick);
				pThis->ok_bt_.ShowWnd(TRUE);

				pThis->clean_bt_.Create(_T("bt_qx"),&pThis->main_wnd_,375,225,IDB_PNG_BT_NO2,82);
				pThis->clean_bt_.SetFont(_T("微软雅黑"),11.0f);
				pThis->clean_bt_.SetWndText(_T("取消"),HORIZONTAL_CENTER|VERTICAL_CENTER);
				pThis->clean_bt_.AddCallback(ButtonClick);
				pThis->clean_bt_.ShowWnd(TRUE);
			}


				// system button
			/*CLkDuiButton  btClose(Color(0,212,64,39));
			btClose.AddCallback(OnClose);
			btClose.Create(_T("uninsert"),&main_wnd_,462 - 40,11,IDB_PNG_SYS_CLOSE,29);*/
			
			
			pThis->main_wnd_.OnPaint();
			pThis->main_wnd_.ShowWnd(TRUE);
			
			//AddFocus(main_wnd_.GetMainHWnd());
			RegeditMsgloop(&pThis->main_wnd_);
			pThis->is_created_ = TRUE;
			RunMessageLoop(pThis->main_wnd_.GetMainHWnd());
		}
		return 0;
	}
}
