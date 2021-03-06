#include "LkDuiTooltip.h"
#include "LkDuiCore.h"


namespace lkDui
{
CLkDuiTooltip::CLkDuiTooltip(void)
	: CLkDuiBaseWnd(Color(254,255,255,225))
	, Max_Wide_(300)
	, hWndThread_(NULL)
	, font_size_(10.0f)
{
	_tcscpy(font_name_,_T("宋体"));
	is_ToolWnd_ = TRUE;
}

CLkDuiTooltip::~CLkDuiTooltip(void)
{
	Hide();
	//Destroy();
}

UINT CLkDuiTooltip::Create(const TCHAR* wnd_name, CLkDuiBase* pBefore, const RECT& rc)
{
	return CLkDuiBase::Create(wnd_name,NULL,rc);
}

UINT CLkDuiTooltip::Show(const POINT& pt,DWORD dwMaxWide)
{
	if (hWndThread_ && 
		hWndThread_ != INVALID_HANDLE_VALUE)
	  return 1;
	show_pt_ = pt;
	Max_Wide_ = dwMaxWide;
	hWndThread_ = CreateThread(NULL,0,thRunMainWnd,this,NULL,NULL);
	return 1;

}

void CLkDuiTooltip::Hide()
{
	if (::IsWindow(hWnd_))
	{
		::PostMessage(hWnd_,WM_DESTROY,0,0);
		DWORD dw = WaitForSingleObject(hWndThread_,1000);
		if (dw == WAIT_TIMEOUT)
		{
			// 增加额外处理代码？
		}
	}
	
}

DWORD CLkDuiTooltip::thRunMainWnd(LPVOID lparam)
{
	UINT uRet = 0;

	CLkDuiTooltip* pThis = (CLkDuiTooltip*)lparam;
	
	RECT rc = {pThis->show_pt_.x,pThis->show_pt_.y,pThis->show_pt_.x,pThis->show_pt_.y};
	uRet = pThis->Create(_T("wnd_text_"),NULL,rc);
	if (uRet)
	{
		HDC hdc = GetDC(pThis->GetMainHWnd());
		if (hdc)
		{
			RECT rcWnd = {pThis->show_pt_.x,pThis->show_pt_.y,pThis->show_pt_.x + pThis->Max_Wide_,pThis->show_pt_.y + 1000},
				rcOut = {0,0,0,0};
			CLkDuiDC::GetTextOutRect(pThis->wnd_text_,hdc,rcWnd,rcOut,pThis->font_name_,pThis->font_size_);
			rcOut.top += 15;
			rcOut.bottom += 15;
			int modify = RC_WIDTH(rcOut)/2;
			rcOut.left -= modify;
			rcOut.right -= modify;
			pThis->SetWndRect(rcOut);
			pThis->Invalid(TRUE);
			pThis->ShowWnd(TRUE);
			RegeditMsgloop(pThis);
			RunMessageLoop(pThis->GetMainHWnd());
		}
	}
	pThis->hWndThread_ = NULL;
	return uRet;
}

void CLkDuiTooltip::SetShowText(TCHAR* szInfo)
{
	_tcscpy(wnd_text_,szInfo);
}

void CLkDuiTooltip::DrawItem()
{
	dc_show_->SetColor(bg_color_,TRUE);
	dc_show_->DrawText(wnd_text_,0,0,HORIZONTAL_LEFT | VERTICAL_LEFT);
	is_erase_bg_ = TRUE;
}
}
