#include "LkDuiProgress.h"


namespace lkDui
{
	CLkDuiProgress::CLkDuiProgress(Color col)
		:CLkDuiBaseWnd(col)
		,total_step_(100.0f)
		,step_(.0f)
		,fxBeg_(.0f)
		,fyBeg_(.0f)
	{
	}

	CLkDuiProgress::~CLkDuiProgress(void)
	{
	}

	UINT CLkDuiProgress::Create(const TCHAR* wnd_name, CLkDuiBase* pBefore, INT x,INT y,UINT uIDBg,UINT uIDBef)
	{
		UINT uRet = 0;
		Bitmap* pBg = NULL;
		Bitmap* pBef = NULL;
		if (CLkDuiDC::ImageFromSource(pBg,NULL,uIDBg,_T("PNG")) 
			&& CLkDuiDC::ImageFromSource(pBef,NULL,uIDBef,_T("PNG")))
		{
			bg_img_.Width = pBg->GetWidth();
			bg_img_.Height = pBg->GetHeight();
			bef_img_.Width = pBef->GetWidth();
			bef_img_.Height = pBef->GetHeight();
			RECT rc = {x,y,x+bg_img_.Width,y+bg_img_.Height};
			uRet = CLkDuiBaseWnd::Create(wnd_name,pBefore,rc);
			fxBeg_ = 0.5f * (bg_img_.Width - bef_img_.Width);
			fyBeg_ = 0.5f * (bg_img_.Height - bef_img_.Height);
			if (!dc_background_)
				dc_background_ = new CLkDuiDC(hWnd_,rc);
			dc_background_->SetImage(pBg,TRUE);
			if (!dc_beforeground_)
				dc_beforeground_ = new CLkDuiDC(hWnd_,rc);
			dc_beforeground_->SetImage(pBef,TRUE,.0f,fyBeg_);
		}
		delete pBef;
		delete pBg;
		return uRet;
	}

	void CLkDuiProgress::DrawItem()
	{
		dc_show_->EraseDC();
		dc_show_->Copy(dc_background_,0,0);
		INT drawCx = step_ / total_step_ * bef_img_.Width;
		if (drawCx == 0)
			drawCx = 1;
		dc_show_->SizeAdd(dc_beforeground_,0,0,0,0,drawCx,0,drawCx,0);	
		is_erase_bg_ = FALSE;
	}

	void CLkDuiProgress::SetTotalStep(float fTotal,BOOL bReDraw)
	{
		total_step_ = fTotal;
		if (bReDraw)
		{
			is_erase_bg_ = TRUE;
			OnPaint();
		}
	}

	void CLkDuiProgress::SetStep(float fStep,BOOL bReDraw)
	{
		step_ = fStep;
		if (bReDraw)
		{
			is_erase_bg_ = TRUE;
			OnPaint();
		}
	}
}

