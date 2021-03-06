#include "LkDuiCheckBox.h"



namespace lkDui
{
	CLkDuiCheckBox::CLkDuiCheckBox(Color col)
		:CLkDuiBaseWnd(col)
		,bg_show_index_(0)
		,check_style_(CHECK_STYLE_ALL)
		,bg_index_limit_(2)
	{
	}

	CLkDuiCheckBox::~CLkDuiCheckBox(void)
	{
	}


	UINT CLkDuiCheckBox::Create(
		const TCHAR* wnd_name,		// 窗口名称
		CLkDuiBase* pBefore,		// 上一级子窗口
		RECT rc,					// 窗口大小
		UINT ubkImageID,			// 背景图片
		UINT nbkAvgelen,			// 背景图片长度
		UINT ubefImageID,			// 前景图片
		UINT nbefAvgelen)
	{

		UINT uRetID = CLkDuiBase::Create(wnd_name,pBefore,rc);
		Bitmap* bg_image = NULL;
		if (CLkDuiDC::ImageFromSource(bg_image,NULL,ubkImageID, _T("PNG")))
		{
			bg_size_.Width = nbkAvgelen;
			bg_size_.Height = bg_image->GetHeight();
			dc_background_ = new CLkDuiDC(hWnd_,Size(bg_image->GetWidth(),bg_image->GetHeight()));
			dc_background_->SetImage(bg_image,TRUE);
			delete bg_image;
		}
		


		Bitmap* bef_image = NULL;
		if (CLkDuiDC::ImageFromSource(bef_image,NULL,ubefImageID, _T("PNG")))
		{
			bef_size_.Width = nbefAvgelen;
			bef_size_.Height= bef_image->GetHeight();
			dc_beforeground_ = new CLkDuiDC(hWnd_,Size(bef_image->GetWidth(),bef_image->GetHeight()));
			dc_beforeground_->SetImage(bef_image,TRUE);
		}
		delete bef_image;
		SetFont(_T("宋体"),10.0f);
		SetTxtColor(Color(0xfe,0xa6,0xb3,0xc4));
		return uRetID;
	}


#define IMG_TXT_GAP  (3)		// 图片和字符间隔像素
	void CLkDuiCheckBox::DrawItem()
	{
		dc_show_->EraseDC();	
		dc_show_->Add(dc_background_,
			0,
			(RC_HIGH(rect_) - dc_background_->GetCY())/2,
			bg_size_.Width * bg_show_index_,0,bg_size_.Width,0);

		dc_show_->Add(dc_beforeground_,
			(bg_size_.Width - bef_size_.Width)/2,
			(RC_HIGH(rect_) - dc_beforeground_->GetCY())/2,
			bef_size_.Width * check_style_,0,bg_size_.Width,0);

		dc_show_->DrawText(wnd_text_,bg_size_.Width + IMG_TXT_GAP,(RC_HIGH(rect_) - dc_background_->GetCY())/2 + 1
			,txt_align_);
		is_erase_bg_ = FALSE;
	}



	void CLkDuiCheckBox::NotifyMouseIn(BOOL bIn)
	{
		bg_show_index_ = bIn ? 1 : 0;
		is_erase_bg_ = TRUE;
		OnPaint();
	}

	void CLkDuiCheckBox::NotifyMouseLDown(BOOL bDwon,POINT pt)
	{
		if (!bDwon && bg_index_limit_ > 1)
		{
			switch (check_style_)
			{
			case CHECK_STYLE_ALL:
				check_style_ = CHECK_STYLE_MID;
				break;
			case CHECK_STYLE_MID:
				check_style_ = CHECK_STYLE_NONE;
				break;
			case CHECK_STYLE_NONE:
				check_style_ = CHECK_STYLE_ALL;
				break;
			}
			if (check_style_+1 == bg_index_limit_)
				check_style_ = CHECK_STYLE_NONE;
			SetCheck(check_style_);

			CLkDuiBaseWnd::NotifyMouseLDown(bDwon,pt);
		}
	}

	void CLkDuiCheckBox::NotifyMouseRDown(BOOL bDwon,POINT pt)
	{
/*
		if (!bDwon)
		{
			switch (check_style_)
			{
			case CHECK_STYLE_ALL:
				check_style_ = CHECK_STYLE_MID;
				break;
			case CHECK_STYLE_MID:
				check_style_ = CHECK_STYLE_NONE;
				break;
			case CHECK_STYLE_NONE:
				check_style_ = CHECK_STYLE_ALL;
				break;
			}
			SetCheck(check_style_);
		}*/
		
	}

	void CLkDuiCheckBox::SetCheck(enum_check_style check_style)
	{
		check_style_ = check_style;
		is_erase_bg_ = TRUE;
		OnPaint();
	}
}
