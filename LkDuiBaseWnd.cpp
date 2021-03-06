#include "LkDuiBaseWnd.h"
#include "LkDuiDef.h"
#include "LkDuiTooltip.h"
#include "LkDuiCore.h"
namespace lkDui
{
	CLkDuiBaseWnd::CLkDuiBaseWnd(Color col):
		dc_main_mem_(NULL),
		dc_background_(NULL),
		dc_beforeground_(NULL),
		dc_show_(NULL),
		func_btClick_(NULL),
		txt_align_(HORIZONTAL_LEFT | VERTICAL_CENTER),
		tool_tip_(NULL)
	{
		InitializeCriticalSection(&callback_lock_);
		bg_color_ = col;
		bef_color_ = col;
		txt_color_.SetValue(ARGB(0xfe566c7c));

		//LPCOLORADJUSTMENT
		
		// Initialize GDI+.
		blen_.AlphaFormat = AC_SRC_ALPHA;
		blen_.BlendFlags = 0;
		blen_.BlendOp = AC_SRC_OVER;
		blen_.SourceConstantAlpha = 255;
		ZeroMemory(wnd_text_,sizeof(wnd_text_));
	}

	CLkDuiBaseWnd::~CLkDuiBaseWnd(void)
	{
		Destroy();
		DeleteCriticalSection(&callback_lock_);
	}



	void CLkDuiBaseWnd::Paint(CLkDuiDC* parentDC,INT xPos,INT yPos)
	{
		if (!is_main_wnd_ && !bShow_)
			return;
		EnterCriticalSection(&dc_lock_);
		if (is_erase_bg_)
			DrawItem();
		if (dc_main_mem_)
		{
			parentDC = dc_main_mem_;
			parentDC->Copy(dc_show_,xPos,yPos);
		}
		else
		{
			parentDC->Add(dc_show_,xPos += RC_LEFT(rect_),yPos += RC_TOP(rect_));
		}
		ChildList::const_iterator cIt = child_list_.begin();	
		while (cIt != child_list_.end())
		{
			(*cIt)->Paint(parentDC,xPos,yPos);
			cIt++;
		}
		
		if (dc_main_mem_)
		{

			
			POINT ptWinPos={RC_LEFT(rect_),RC_TOP(rect_)};
			SIZE sizeWindow={RC_WIDTH(rect_),RC_HIGH(rect_)};
			POINT ptSrc={0,0};
			HDC hMainDC = GetDC(NULL);
			BOOL bRet= UpdateLayeredWindow( GetMainHWnd(),hMainDC,&ptWinPos,
				&sizeWindow, dc_main_mem_->GetHDC(), &ptSrc, RGB(0, 0, 0), &blen_, ULW_ALPHA/*g_colBit == 32?ULW_ALPHA:ULW_COLORKEY*/);
			if (!bRet)
			{
				TCHAR s[100];
				wsprintf(s,TEXT("UpdateLayeredWindow error[%08X]"),GetLastError());
				//MessageBox(NULL,s,L"error",MB_OK);
			}
			ReleaseDC(hWnd_,hMainDC);
		}
		LeaveCriticalSection(&dc_lock_);


	}
	void CLkDuiBaseWnd::ShowWnd(BOOL bShow)
	{
		bShow_ = bShow;
		if (is_main_wnd_)
			ShowWindow(hWnd_,bShow?SW_SHOW:SW_HIDE);
	}

	void CLkDuiBaseWnd::DrawItem()
	{
		dc_show_->EraseDC();

		dc_show_->Copy(dc_background_,0,0);
		//dc_background_->TurnDC(Point(10,10),30);
		dc_show_->Add(dc_beforeground_,0,0);	
		//dc_show_->TurnAdd(dc_beforeground_,60);
		if (wnd_text_[0] != '\0')
			dc_show_->DrawText(wnd_text_,0,0,txt_align_);
		is_erase_bg_ = FALSE;
	}

	// 设置窗口字体
	void CLkDuiBaseWnd::SetWndText(const TCHAR* wndText,DWORD dwalign)
	{
		_tcscpy(wnd_text_,wndText);
		txt_align_ = dwalign;
		is_erase_bg_ = TRUE;
		OnPaint();
	}
	DWORD CLkDuiBaseWnd::GetWndText(TCHAR* wndText,DWORD dwLen)
	{
		_tcscpy_s(wndText,dwLen,wnd_text_);
		return _tcslen(wndText);
	}
	// 父窗口相对位置
	BOOL CLkDuiBaseWnd::SetWndRect(const RECT& rc)
	{
		BOOL bRedraw = ((RC_WIDTH(rc) != RC_WIDTH(rect_)) || (RC_HIGH(rc) != RC_HIGH(rect_)));
		memcpy(&rect_,&rc,sizeof(RECT));
		if (bRedraw)
		{
			EnterCriticalSection(&dc_lock_);
			if (is_main_wnd_)
			{
				if (dc_main_mem_)
					delete dc_main_mem_;
				dc_main_mem_ = new CLkDuiDC(hWnd_,rect_);
			}
			if (dc_show_)
				delete dc_show_;
			dc_show_ = new CLkDuiDC(hWnd_,rect_);
			LeaveCriticalSection(&dc_lock_);
			is_erase_bg_ = TRUE;
		}
		return bRedraw;
	}

	// 设置背景颜色或图片
	void CLkDuiBaseWnd::SetBackground(Color col,BOOL bErase,BOOL bRefalsh)
	{
		if (!dc_background_)
			dc_background_ = new CLkDuiDC(hWnd_,rect_);
		bg_color_ = col;
		dc_background_->SetColor(bg_color_,bErase);
		is_erase_bg_ = TRUE;
		if (bRefalsh)
			OnPaint();
	}

	void CLkDuiBaseWnd::EnableToolTip(BOOL bEnable)
	{
		if (bEnable)
		{
			if (!tool_tip_)
				tool_tip_ = new CLkDuiTooltip();
		}
		else
		{
			if (!tool_tip_)
			{
				delete tool_tip_;
				tool_tip_ = NULL;
			}
		}
	}

	void CLkDuiBaseWnd::SetBackground(DWORD dwimgid,BOOL bErase,BOOL bRefalsh)
	{
		//CGdiPlusBitmapResource* bg_image = new CGdiPlusBitmapResource;
		Bitmap* bmp = NULL;
		if (CLkDuiDC::ImageFromSource(bmp,NULL,dwimgid,L"PNG"))
		{
			Size sImg(bmp->GetWidth(),bmp->GetHeight());
			EnterCriticalSection(&dc_lock_);
			if (dc_background_ && 
				(dc_background_->GetCX() != sImg.Width 
				|| dc_background_->GetCY() != sImg.Height ))
			{
				delete dc_background_;
				dc_background_ = NULL;
			}
			if (!dc_background_)
				dc_background_ = new CLkDuiDC(hWnd_,sImg);
			LeaveCriticalSection(&dc_lock_);
			dc_background_->SetImage(bmp,bErase);
			is_erase_bg_ = TRUE;
			delete bmp;
			if (bRefalsh)
				OnPaint();
		}
		
	}



	// 设置背景颜色或图片
	void CLkDuiBaseWnd::SetBeforeground(Color col,BOOL bErase,BOOL bRefalsh)
	{
		if (!dc_beforeground_)
			dc_beforeground_ = new CLkDuiDC(hWnd_,rect_);
		bef_color_ = col;
		dc_beforeground_->SetColor(bef_color_,bErase);

		is_erase_bg_ = TRUE;
		if (bRefalsh)
			OnPaint();
	}
	void CLkDuiBaseWnd::SetBeforeground(DWORD dwimgid,BOOL bErase,BOOL bRefalsh)
	{
		Bitmap* bmp = NULL;
		if (CLkDuiDC::ImageFromSource(bmp,NULL,dwimgid,L"PNG"))
		{
			Size sImg(bmp->GetWidth(),bmp->GetHeight());
			if (dc_beforeground_ && 
				(dc_beforeground_->GetCX() != sImg.Width 
				|| dc_beforeground_->GetCY() != sImg.Height ))
			{
				delete dc_beforeground_;
				dc_beforeground_ = NULL;
			}
			if (!dc_beforeground_)
				dc_beforeground_ = new CLkDuiDC(hWnd_,sImg);

			dc_beforeground_->SetImage(bmp,bErase);
			delete bmp;
			is_erase_bg_ = TRUE;
			if (bRefalsh)
				OnPaint();
		}
		
	}

	UINT CLkDuiBaseWnd::Create(const TCHAR* wnd_name, CLkDuiBase* pBefore, const RECT& rc)
	{
		UINT uRetID =  CLkDuiBase::Create(wnd_name,pBefore,rc);
		if (is_main_wnd_)
		{
			if (dc_main_mem_)
				delete dc_main_mem_;
			dc_main_mem_ = new CLkDuiDC(hWnd_,rect_);
			//blen_.AlphaFormat = g_colBit == 32?AC_SRC_ALPHA:0;
			blen_.AlphaFormat = AC_SRC_ALPHA;
		}
		if (dc_show_)
			delete dc_show_;
		dc_show_ = new CLkDuiDC(hWnd_,rect_);
		SetBackground(bg_color_,TRUE,FALSE);
		return uRetID;
	}
	void CLkDuiBaseWnd::Destroy(BOOL bRemoveFromParent)
	{
		bShow_ = FALSE;
		if (dc_main_mem_)
		{	
			delete dc_main_mem_;
			dc_main_mem_ = NULL;
		}
		if (dc_background_)
		{
			delete dc_background_;
			dc_background_ = NULL;
		}
		if (dc_beforeground_)
		{
			delete dc_beforeground_;
			dc_beforeground_ = NULL;
		}
		if (dc_show_)
		{
			delete dc_show_;
			dc_show_ = NULL;
		}
		if (tool_tip_)
		{
			delete tool_tip_;
			tool_tip_ = NULL;
		}
		CLkDuiBase::Destroy(bRemoveFromParent);
	}
	void CLkDuiBaseWnd::SetFont(LPCTSTR sFont,float fBom)
	{
		if (dc_show_)
			dc_show_->SetFont(sFont,fBom);
	}
	void CLkDuiBaseWnd::SetTxtColor(Color col)
	{
		txt_color_ = col;
		if (dc_show_)
			dc_show_->SetTextColor(col);
		is_erase_bg_ = TRUE;
	}

	void CLkDuiBaseWnd::NotifyMouseIn(BOOL bInWnd)
	{
		if (!is_enable_)
			return;
		if (!bInWnd
			&& tool_tip_)
		{
			tool_tip_->Hide();
		}
		return;
	}

	void CLkDuiBaseWnd::NotifyMouseHover(POINT pt)
	{
		if (!is_enable_)
			return;
		if (tool_tip_)
		{
			tool_tip_->SetShowText(wnd_text_);
			tool_tip_->Show(pt);
		}
	}

	void CLkDuiBaseWnd::NotifyMouseLDown(BOOL bDwon,POINT pt)
	{
		if (!is_enable_)
			return;
		if (!bDwon && func_btClick_)
		{
			// 这里需要阻塞
			if (TryEnterCriticalSection(&callback_lock_))
			{
				func_btClick_(this,pt);
				LeaveCriticalSection(&callback_lock_);
			}
		}
	}
	void CLkDuiBaseWnd::NotifyMouseRDown(BOOL bDwon,POINT pt)
	{
		if (!is_enable_)
			return;
		return;
	}


/*
	BOOL CLkDuiBaseWnd::ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * & pImg)  
	{  
		HINSTANCE hInst = GetModuleHandle(NULL);  
		HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(nID),sTR); // type  
		if (!hRsrc)  
			return FALSE;  
		// load resource into memory  
		DWORD len = SizeofResource(hInst, hRsrc);  
		BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
		if (!lpRsrc)  
			return FALSE;  
		// Allocate global memory on which to create stream  
		HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);  
		BYTE* pmem = (BYTE*)GlobalLock(m_hMem);  
		memcpy(pmem,lpRsrc,len);  
		IStream* pstm;  
		CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
		// load from stream  
		pImg=Gdiplus::Image::FromStream(pstm);  
		// free/release stuff  
		GlobalUnlock(m_hMem);  
		pstm->Release();  
		FreeResource(lpRsrc);  
		return TRUE;  
	} */

/*
	bool add1 = true;
	bool add2 = true;
	bool add3 = true;
	void CLkDuiBaseWnd::OnTime(UINT uTimerID)
	{
		BYTE a = bg_color_.GetAlpha();
		BYTE r = bg_color_.GetRed();
		BYTE g = bg_color_.GetGreen();
		BYTE b = bg_color_.GetBlue();
		
		if (add1)
		{
			r += 1;
			if (r > 253)
				add1 = false;
		}
		if (add2)
		{
			g += 2;
			if (g > 252)
				add2 = false;
		}
		if (add3)
		{
			b += 3;
			if (b > 251)
				add3 = false;
		}
		if (!add1)
		{
			r -= 1;
			if (r < 1)
				add1 = true;
		}
		if (!add2)
		{
			g -= 2;
			if (g < 2)
				add2 = true;
		}
		if (!add3)
		{
			b -= 3;
			if (b < 3)
				add3 = true;
		}
		a = r;
		Color cl(a,r,g,b);
		SetBackground(cl);
	}*/



}

