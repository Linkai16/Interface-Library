#include "LkDuiImgDisplay.h"


namespace lkDui
{

	#define TIMER_IMAGEWND 0x200
	#define   TIMER_IMAGE_CHANGE		(TIMER_IMAGEWND + 0)
	#define   TIMER_IMAGE_CHANGEING		(TIMER_IMAGEWND + 1)

	CLkDuiImgDisplay::CLkDuiImgDisplay(Color col) : CLkDuiBaseWnd(col)
		,show_pos_(0)
		,byAlpha_(FULL_ALPHA)
		,display_interval_(0xFFFFFFFF)
	{
		timer_ = new CLkDuiTimer(this);
	}

	CLkDuiImgDisplay::~CLkDuiImgDisplay(void)
	{
		delete timer_;
		for (int i = 0;i < image_array_.size();i++)
		{
			delete image_array_[i].pImage;
		}
		image_array_.clear();
	}

	void CLkDuiImgDisplay::AddImage(DWORD dwID,LPCTSTR sZY,BOOL bFlash)
	{
		Bitmap* p = NULL;
		if (CLkDuiDC::ImageFromSource(p,NULL,dwID,sZY))
		{
			lkDuiImage img;
			img.fAngle = 0;
			img.fSpeed = 0;
			img.pImage = p;
			image_array_.push_back(img);
			if(dc_background_->IsEmpty())
				dc_background_->SetImage(p,TRUE);
		}
		else 
			delete p;
		if (bFlash)
		{
			OnPaint();
		}

	}

	void CLkDuiImgDisplay::OnTime(UINT id)
	{
		if (id == TIMER_IMAGE_CHANGE)
		{
			OnChangeImage();
		}
		else if (id == TIMER_IMAGE_CHANGEING)
		{
			
			if (byAlpha_ < 20)
			{	
				byAlpha_ = FULL_ALPHA;
				if (++show_pos_ == image_array_.size())
					show_pos_ = 0;
				dc_background_->SetImage(GetCurrentImg().pImage,TRUE);
				timer_->RemoveTimer(GetMainHWnd(),TIMER_IMAGE_CHANGEING);
			}
			else
				byAlpha_ -= 20;

			is_erase_bg_ = TRUE;
			OnPaint();
		}
	}

	void CLkDuiImgDisplay::ShowWnd(BOOL bShow)
	{
		bShow?Play(display_interval_):Stop();
	}

	void CLkDuiImgDisplay::Play(UINT nInterval)
	{
		CLkDuiBaseWnd::ShowWnd(TRUE);
		if (image_array_.size() <= 0)
			return;
		dc_background_->SetImage(image_array_[0].pImage,TRUE);
		display_interval_ = nInterval;
		timer_->AddTimer(GetMainHWnd(),TIMER_IMAGE_CHANGE,display_interval_);
		is_erase_bg_ = TRUE;
		OnPaint();
	}

	void CLkDuiImgDisplay::Stop()
	{
		CLkDuiBaseWnd::ShowWnd(FALSE);
		timer_->RemoveTimer(GetMainHWnd(),TIMER_IMAGE_CHANGE);
	}

	void CLkDuiImgDisplay::DrawItem()
	{
		dc_show_->EraseDC();
		if (byAlpha_ != FULL_ALPHA)
		{
			dc_show_->Copy(dc_background_,0,0);
			dc_show_->Add(dc_beforeground_,0,0,0,0,0,0,byAlpha_);	
		}
		else
			dc_show_->Copy(dc_background_,0,0);
		is_erase_bg_ = FALSE;
	}

	void CLkDuiImgDisplay::OnChangeImage()
	{
		dc_background_->SetImage(GetNextImg().pImage,TRUE);
		if (!dc_beforeground_)
			dc_beforeground_ = new CLkDuiDC(hWnd_,rect_);	
		dc_beforeground_->SetImage(GetCurrentImg().pImage,TRUE);
		byAlpha_ -= 20;
		timer_->AddTimer(GetMainHWnd(),TIMER_IMAGE_CHANGEING,100);
	}

	const lkDuiImage& CLkDuiImgDisplay::GetCurrentImg()
	{
		lkDuiImage img;
		if (show_pos_ < image_array_.size())
			return image_array_.at(show_pos_);
		return img;
	}
	const lkDuiImage& CLkDuiImgDisplay::GetNextImg()
	{
		lkDuiImage img;
		ImageArray::size_type sTp = show_pos_+1;
		if (sTp >= image_array_.size())
			sTp -= image_array_.size();
		if (sTp < image_array_.size())
			return image_array_.at(sTp);
		return img;
	}
}

