#include "LkDuiEdit.h"

namespace lkDui
{

	CLkDuiEdit::CLkDuiEdit(Color col) : 
			CLkDuiBaseWnd(col),
				hTxt(NULL)
	{
	}

	CLkDuiEdit::~CLkDuiEdit(void)
	{
	}

	UINT CLkDuiEdit::Create(const TCHAR* wnd_name, CLkDuiBase* pBefore, const RECT& rc)
	{
		UINT uID = CLkDuiBaseWnd::Create(wnd_name,pBefore,rc);
		//ClientToScreen(GetMainHWnd(),&rc);
/*
		handle_therad_edit_	= CreateThread(NULL,0,theditThread,this,NULL,NULL);
		Sleep(1000);
		if (hTxt)
		{

			HDC hdc;
			HFONT hFont;
			LOGFONT lf;

			hdc = GetDC(hTxt);

			lf.lfHeight         = GetDeviceCaps(hdc, LOGPIXELSY)*10/72;
			lf.lfWidth          = 0 ;
			lf.lfEscapement     = 0 ;
			lf.lfOrientation    = 0 ;
			lf.lfWeight         = 400 ;
			lf.lfItalic         = 0 ;
			lf.lfUnderline      = 0 ;
			lf.lfStrikeOut      = 0 ;
			lf.lfCharSet        = GB2312_CHARSET;
			lf.lfOutPrecision   = 3 ;
			lf.lfClipPrecision  = 2;
			lf.lfQuality        = 1 ;
			lf.lfPitchAndFamily = 2 ;	
			lstrcpy (lf.lfFaceName, L"宋体") ;

			hFont = CreateFontIndirect(&lf);
			
			SelectObject(hdc,hFont);
			GetObject(hFont,sizeof(LOGFONT),&lf);

			::SendMessage(hTxt,WM_SETFONT,WPARAM(hFont),0);
			ReleaseDC(hTxt,hdc);
		}
		DWORD dwError = GetLastError();*/
		return uID;
	}
		
	DWORD WINAPI CLkDuiEdit::theditThread(LPVOID lp)
	{
		CLkDuiEdit* pthis = (CLkDuiEdit*)lp;
		RECT rc;
		pthis->GetLastRect(&rc);
		rc.top +=3;
		rc.left +=3 ;
		rc.right -= 10;
		rc.bottom -= 8;
		pthis->hTxt = CreateWindow(_T("Edit"),_T("edit"),WS_VISIBLE | WS_POPUP,rc.left,rc.top,
			RC_WIDTH(rc),RC_HIGH(rc), NULL,NULL,0,NULL);
		
		MSG msg;
		// Run the application message loop.
		while (::IsWindow(pthis->hTxt) && GetMessage(&msg, pthis->hTxt, 0, 0)) {
			// Allow processing of find dialog messages.
			/*
			if (hFindDlg && IsDialogMessage(hFindDlg, &msg))
			continue;*/

			if (!TranslateMessage(&msg)) 
			{
				if (msg.message == WM_CTLCOLOREDIT)
				{
					int a = 0;
					a = 10;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_DESTROY)
				break;

		}
		pthis->hTxt = NULL;
		//ShowWindow(hTxt,SW_SHOW);
		return static_cast<int>(msg.wParam);
	}

	void CLkDuiEdit::DrawItem()
	{
		dc_show_->EraseDC();

		dc_show_->Copy(dc_background_,0,0);
		//dc_background_->TurnDC(Point(10,10),30);
		dc_show_->Add(dc_beforeground_,0,0);	
		//dc_show_->TurnAdd(dc_beforeground_,60);
		if (wnd_text_[0] != '\0')
			dc_show_->DrawText(wnd_text_,10,0, txt_align_,FALSE);
		is_erase_bg_ = FALSE;
	}

}

