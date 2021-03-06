#include "LkDuiButton.h"


namespace lkDui
{
#define TIMER_BUTTON             (0x20)
#define TIMER_BUTTON_MOUSE_IN    (TIMER_BUTTON+1)
#define TIMER_BUTTON_MOUSE_OUT    (TIMER_BUTTON+2)

	CLkDuiButton::CLkDuiButton(Color col) : 
		CLkDuiBaseWnd(col),
			mouse_in_(0),
			bef_Avgelen_(0),
			bef_size_(0,0),
			bef_show_index_(0)
	{
		timer_ = new CLkDuiTimer(this);
	}

	CLkDuiButton::~CLkDuiButton(void)
	{
		delete timer_;
	}


	UINT CLkDuiButton::Create(
		const TCHAR* wnd_name,		// 窗口名称
		CLkDuiBase* pBefore,		// 上一级子窗口
		INT   x,
		INT   y,
		UINT uImageID,
		UINT nAvgelen)
	{
		UINT uRetID = 0;
		Bitmap* bmp = NULL;
		if (CLkDuiDC::ImageFromSource(bmp,NULL,uImageID,L"PNG"))
		{
			bef_size_.Width =bmp->GetWidth();
			bef_size_.Height = bmp->GetHeight();
			bef_Avgelen_ = nAvgelen;
			RECT rc = {x,y,x+bef_Avgelen_,y+bef_size_.Height};
			UINT uRetID = CLkDuiBase::Create(wnd_name,pBefore,rc);

			RECT rcBef = {0,0,bef_size_.Width,bef_size_.Height};
			dc_background_ = new CLkDuiDC(hWnd_,rc);
			dc_beforeground_ = new CLkDuiDC(hWnd_,rcBef);
			dc_background_->SetColor(bg_color_,TRUE);
			dc_beforeground_->SetImage(bmp,TRUE);
			delete bmp;

		}
		return uRetID;
	}

	void CLkDuiButton::SetBeforeground(DWORD dwimgid,UINT nAvgelen,BOOL bErase,BOOL bRefalsh)
	{
		Bitmap* bmp = NULL;
		if (CLkDuiDC::ImageFromSource(bmp,NULL,dwimgid,L"PNG"))
		{
			Size sImg(bmp->GetWidth(),bmp->GetHeight());
			bef_size_ = sImg;
			bef_Avgelen_ = nAvgelen;
			dc_beforeground_->SetImage(bmp,bErase);
			delete bmp;
			is_erase_bg_ = TRUE;
			if (bRefalsh)
				OnPaint();
		}
	}

	void CLkDuiButton::DrawItem()
	{
		dc_show_->EraseDC();
		dc_show_->Copy(dc_background_);
		dc_show_->Add(dc_beforeground_,0,0,bef_Avgelen_*bef_show_index_,0,bef_Avgelen_,0);
		is_erase_bg_ = FALSE;
	}

	
	void CLkDuiButton::NotifyMouseLDown(BOOL bDwon,POINT pt)
	{
		bef_show_index_ =  bDwon ? 1 : 0;
		is_erase_bg_ = TRUE;
		OnPaint();
		CLkDuiBaseWnd::NotifyMouseLDown(bDwon,pt);
	}

	void CLkDuiButton::NotifyMouseRDown(BOOL bDwon,POINT pt)
	{
		return;
	}

	void CLkDuiButton::NotifyMouseIn(BOOL bIn)
	{
		if (bIn)
		{
			if (mouse_in_ == 0)
			{
				timer_->RemoveTimer(GetMainHWnd(),TIMER_BUTTON_MOUSE_OUT);
				timer_->AddTimer(GetMainHWnd(),TIMER_BUTTON_MOUSE_IN,20);
			}
		}
		else
		{
			if (mouse_in_ > 0)
			{
				timer_->RemoveTimer(GetMainHWnd(),TIMER_BUTTON_MOUSE_IN);
				timer_->AddTimer(GetMainHWnd(),TIMER_BUTTON_MOUSE_OUT,20);
			}
		}
		
	}

#define SHOW_STEP (10)
	void CLkDuiButton::OnTime(UINT uTimerID)
	{
		if (uTimerID == TIMER_BUTTON_MOUSE_IN)
		{
			BYTE a = (FULL_ALPHA / SHOW_STEP) * ++mouse_in_;
			Color colTemp(a > FULL_ALPHA?FULL_ALPHA:a,bg_color_.GetR(),bg_color_.GetG(),bg_color_.GetB());
			SetBackground(colTemp,TRUE);
			if (mouse_in_ == SHOW_STEP)
			{
				timer_->RemoveTimer(GetMainHWnd(),TIMER_BUTTON_MOUSE_IN);
			}
			
		}
		else if (uTimerID == TIMER_BUTTON_MOUSE_OUT)
		{
			BYTE a = (FULL_ALPHA / SHOW_STEP) * --mouse_in_;
			Color colTemp(a > FULL_ALPHA?FULL_ALPHA:a,bg_color_.GetR(),bg_color_.GetG(),bg_color_.GetB());
			SetBackground(colTemp,TRUE);
			if (mouse_in_ == 0)
			{
				timer_->RemoveTimer(GetMainHWnd(),TIMER_BUTTON_MOUSE_OUT);
			}
		}
	}
}

