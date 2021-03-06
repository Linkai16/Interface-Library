#include "LkDuiRotateWnd.h"


namespace lkDui
{
#define TIMER_ROTATE 0x100
	CLkDuiRotateWnd::CLkDuiRotateWnd(Color col): 
			CLkDuiBaseWnd(col)
	{
		bShow_ = FALSE;
		timer_ = new CLkDuiTimer(this);
	}

	CLkDuiRotateWnd::~CLkDuiRotateWnd(void)
	{
		ImageList::iterator it = image_list_.begin();
		while (it != image_list_.end())
		{
			delete (*it).pImage;
			it = image_list_.erase(it);
		}
		delete timer_;
	}
	
	void CLkDuiRotateWnd::OnTime(UINT uTimerID)
	{
		if (uTimerID == TIMER_ROTATE)
		{
			ImageList::iterator it = image_list_.begin();	
			while (it != image_list_.end())
			{
				(*it).fAngle += (*it).fSpeed;
				if ((*it).fAngle > 360.0f)
					(*it).fAngle -= 360.0f;
				else if ((*it).fAngle < .0f)
					(*it).fAngle += 360.0f;
				it++;
			}
			is_erase_bg_ = TRUE;
			OnPaint();
		}
	}

	void CLkDuiRotateWnd::DrawItem()
	{
		dc_show_->EraseDC();
		dc_show_->Copy(dc_background_,0,0);
		MakeBeforeground();
		dc_show_->Add(dc_beforeground_);
		is_erase_bg_ = FALSE;
	}

	void CLkDuiRotateWnd::MakeBeforeground()
	{
		if (!dc_beforeground_)
			dc_beforeground_ = new CLkDuiDC(hWnd_,rect_);
		else
			dc_beforeground_->EraseDC();
		ImageList::iterator it = image_list_.begin();	
		while (it != image_list_.end())
		{
			dc_beforeground_->SetImage((*it).pImage,(*it).fAngle,FALSE);
			it++;
		}
	}
	
	void CLkDuiRotateWnd::SetBackground(DWORD dwimgid,BOOL bErase,BOOL bRefalsh) 
	{
		if (!dc_background_)
			dc_background_ = new CLkDuiDC(hWnd_,rect_);
		Bitmap* p = NULL;
		if (CLkDuiDC::ImageFromSource(p,NULL,dwimgid,_T("PNG")))
		{
			dc_background_->SetImage(p,bErase,0.5f*(RC_WIDTH(rect_) - p->GetWidth()),0.5f*(RC_HIGH(rect_) - p->GetHeight()));
			delete p;
		}
		
		
	}
	BOOL CLkDuiRotateWnd::AddImage(DWORD dwID,LPCTSTR sZY,BOOL bPaint,float fSpeed)
	{
		Bitmap* p = NULL;
		if (CLkDuiDC::ImageFromSource(p,NULL,dwID,sZY))
		{
			lkDuiImage img;
			img.fAngle = 0;
			img.fSpeed = fSpeed;
			img.pImage = p;
			image_list_.push_back(img);
		}
		else
			delete p;
		if (bPaint)
			OnPaint();
		return TRUE;
	}
	void CLkDuiRotateWnd::ShowWnd(BOOL bShow)
	{
		bShow?Play():Stop();
	}
	void CLkDuiRotateWnd::Stop()
	{
		CLkDuiBaseWnd::ShowWnd(FALSE);
		timer_->RemoveTimer(GetMainHWnd(),TIMER_ROTATE+0);
	}
	void CLkDuiRotateWnd::Play()
	{
		CLkDuiBaseWnd::ShowWnd(TRUE);
		timer_->AddTimer(GetMainHWnd(),TIMER_ROTATE+0,20);
	}
}

