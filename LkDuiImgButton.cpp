#include "LkDuiImgButton.h"


namespace lkDui
{
	CLkDuiImgButton::CLkDuiImgButton(Color col):
		CLkDuiBaseWnd(col),
			mouse_in_(0),
			bef_Avgelen_(0),
			bg_size_(0,0),
			bef_show_index_(0)
	{
		SetTxtColor(
			Color(ARGB(0xfeb6b6b6)),
			Color(ARGB(0xfe426286)),
			Color(ARGB(0xfeb6b6b6)));
	}

	CLkDuiImgButton::~CLkDuiImgButton(void)
	{
	}


#define BEF_IMAGE_COUNT (3)
	UINT CLkDuiImgButton::Create(
		const TCHAR* wnd_name,		// 窗口名称
		CLkDuiBase* pBefore,		// 上一级子窗口
		INT   x,
		INT   y,
		UINT uImageID,
		UINT nAvgelen)
	{
		UINT uRetID = 0;
		Bitmap* bef_image = NULL;
		if (CLkDuiDC::ImageFromSource(bef_image,NULL,uImageID, _T("PNG")))
		{
			bg_size_.Width = bef_image->GetWidth();
			bg_size_.Height = bef_image->GetHeight();
			bef_Avgelen_ = ((nAvgelen * BEF_IMAGE_COUNT) > bg_size_.Width)?bg_size_.Width/BEF_IMAGE_COUNT:nAvgelen;
			RECT rc = {x,y,x+bef_Avgelen_,y+bg_size_.Height};
			uRetID = CLkDuiBase::Create(wnd_name,pBefore,rc);
			RECT rcBg = {0,0,bg_size_.Width,bg_size_.Height};
			if (!dc_background_)
				dc_background_ = new CLkDuiDC(hWnd_,rcBg);
			if (!dc_show_)
				dc_show_ = new CLkDuiDC(hWnd_,rc);
			dc_background_->SetImage(bef_image,TRUE);
			dc_show_->SetTextColor(txt_color_[bef_show_index_]);
			delete bef_image;
		}
		
		return uRetID;
	}
	void CLkDuiImgButton::EnableWnd(BOOL bEnable,DWORD dwID)
	{
		
		if (!bEnable)
		{
			Bitmap* img = NULL;
			is_erase_bg_ = FALSE;
			if (CLkDuiDC::ImageFromSource(img,NULL,dwID, _T("PNG")))
			{
				dc_show_->SetImage(img,TRUE);
				delete img;
				img = NULL;
			}
			OnPaint();
		}
		else
		{
			if (is_enable_ != bEnable)
			{
				is_erase_bg_ = TRUE;
				OnPaint();
			}
		}
		is_enable_ = bEnable;
	}
	void CLkDuiImgButton::SetTxtColor(Color col1,Color col2,Color col3)
	{
		txt_color_[0] = col1;
		txt_color_[1] = col2;
		txt_color_[2] = col3;
		if (dc_show_)
			dc_show_->SetTextColor(txt_color_[bef_show_index_]);
		
	}

	void CLkDuiImgButton::SetBackground(DWORD dwimgid,UINT nAvgelen,BOOL bErase,BOOL bRefalsh)
	{
		Bitmap* bef_image = NULL;
		if (CLkDuiDC::ImageFromSource(bef_image,NULL,dwimgid, _T("PNG")))
		{
			bg_size_.Height = bef_image->GetHeight();
			bg_size_.Width = bef_image->GetWidth();
			RECT rc = {rect_.left,rect_.top, rect_.left + nAvgelen,rect_.top + bg_size_.Height};
			if (SetWndRect(rc))
			{
				bef_Avgelen_ = nAvgelen;
				RECT rcBg = {0,0,bg_size_.Width,bg_size_.Height};
				EnterCriticalSection(&dc_lock_);	
				if (dc_background_)
					delete dc_background_;
				dc_background_ = new CLkDuiDC(hWnd_,rcBg);	
				dc_background_->SetImage(bef_image,bErase);	
				LeaveCriticalSection(&dc_lock_);
				is_erase_bg_ = TRUE;
			}	
			if (bRefalsh)
				OnPaint();
		}
		delete bef_image;
		bef_image = NULL;
	}
	void CLkDuiImgButton::DrawItem()
	{
		dc_show_->EraseDC();
		dc_show_->Add(dc_background_,0,0,bef_Avgelen_*bef_show_index_,0,bef_Avgelen_,0);
		dc_show_->Add(dc_beforeground_);
		
		if (wnd_text_[0] != '\0')
			dc_show_->DrawText(wnd_text_,0,0,txt_align_);

		is_erase_bg_ = FALSE;
	}

	void CLkDuiImgButton::NotifyMouseIn(BOOL bIn)
	{
		if (!is_enable_)
			return;
		mouse_in_ = bIn;
		bef_show_index_ = mouse_in_ ? 1:0;
		dc_show_->SetTextColor(txt_color_[bef_show_index_]);
		is_erase_bg_ = TRUE;
		OnPaint();
	}
	

	void CLkDuiImgButton::NotifyMouseLDown(BOOL bDwon,POINT pt)
	{
		if (!is_enable_)
			return;
		if (bDwon)
		{	
			bef_show_index_ = 2;
		}
		else
		{
			bef_show_index_ = mouse_in_ ? 1:0;
		}
		dc_show_->SetTextColor(txt_color_[bef_show_index_]);
		is_erase_bg_ = TRUE;
		OnPaint();
		CLkDuiBaseWnd::NotifyMouseLDown(bDwon,pt);
	}

	void CLkDuiImgButton::NotifyMouseRDown(BOOL bDwon,POINT pt)
	{
		if (!is_enable_)
			return;
	}
}

