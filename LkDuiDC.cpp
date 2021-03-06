#include "LkDuiDC.h"
#include "LkDuiDef.h"
#include "LkDuiCore.h"
#include <math.h>
#include <tchar.h>

namespace lkDui
{
	CLkDuiDC::CLkDuiDC(HWND hWnd,const RECT& rc)
		:is_empty_(TRUE)
		,font_(NULL)
		,txt_color_(ARGB(0xfe566c7c))
	
	{
		size_.Width = RC_WIDTH(rc);
		size_.Height = RC_HIGH(rc);
		HDC hTemp = GetDC(hWnd);
		hDC_ = CreateCompatibleDC(hTemp);
		if (g_colBit == 32)
			hBmp_ = CreateCompatibleBitmap(hTemp,size_.Width,size_.Height);
		else
		{
			BYTE * pBits;
			BITMAPINFOHEADER bmih;
			ZeroMemory( &bmih, sizeof( BITMAPINFOHEADER ) );

			bmih.biSize = sizeof (BITMAPINFOHEADER) ;
			bmih.biWidth = size_.Width;
			bmih.biHeight = size_.Height;
			bmih.biPlanes = 1 ;
			bmih.biBitCount = 32; //这里一定要是32
			bmih.biCompression = BI_RGB ;
			bmih.biSizeImage = 0 ;
			bmih.biXPelsPerMeter = 0 ;
			bmih.biYPelsPerMeter = 0 ;
			bmih.biClrUsed = 0 ;
			bmih.biClrImportant = 0 ;

			hBmp_ = CreateDIBSection (NULL, (BITMAPINFO *) &bmih, 0, (VOID**)&pBits, NULL, 0) ;
		}


		hOld_ = (HBITMAP)SelectObject(hDC_,hBmp_);
		SetBkMode(hDC_,TRANSPARENT);
		ReleaseDC(hWnd,hTemp);

		blen_.AlphaFormat = AC_SRC_ALPHA;
		blen_.BlendFlags = 0;
		blen_.BlendOp = AC_SRC_OVER;
		blen_.SourceConstantAlpha = 255;

		SetFont(TEXT("宋体"),10);
	}

	CLkDuiDC::CLkDuiDC(HWND hWnd,const Size& size)
		:is_empty_(TRUE)
		,font_(NULL)
	{
		size_.Width = size.Width;
		size_.Height = size.Height;
		HDC hTemp = GetDC(hWnd);
		hDC_ = CreateCompatibleDC(hTemp);
		if (g_colBit == 32)
			hBmp_ = CreateCompatibleBitmap(hTemp, size_.Width, size_.Height);
		else
		{
			BYTE * pBits;
			BITMAPINFOHEADER bmih;
			ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

			bmih.biSize = sizeof(BITMAPINFOHEADER);
			bmih.biWidth = size_.Width;
			bmih.biHeight = size_.Height;
			bmih.biPlanes = 1;
			bmih.biBitCount = 32; //这里一定要是32
			bmih.biCompression = BI_RGB;
			bmih.biSizeImage = 0;
			bmih.biXPelsPerMeter = 0;
			bmih.biYPelsPerMeter = 0;
			bmih.biClrUsed = 0;
			bmih.biClrImportant = 0;

			hBmp_ = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID**)&pBits, NULL, 0);
		}
		hOld_ = (HBITMAP)SelectObject(hDC_,hBmp_);
		SetBkMode(hDC_,TRANSPARENT);
		ReleaseDC(hWnd,hTemp);

		blen_.AlphaFormat = AC_SRC_ALPHA;
		blen_.BlendFlags = 0;
		blen_.BlendOp = AC_SRC_OVER;
		blen_.SourceConstantAlpha = 255;

		SetFont(TEXT("宋体"),10);
	}


	CLkDuiDC::~CLkDuiDC(void)
	{
		if (font_)
			delete font_;
		SelectObject(hDC_,hOld_);
		DeleteObject(hBmp_);
		DeleteDC(hDC_);
	}


	void CLkDuiDC::InitByImage(const CGdiPlusBitmapResource* image)
	{

	}
	void CLkDuiDC::Init(const SizeF*)
	{

	}
	void CLkDuiDC::Release()
	{

	}

	void CLkDuiDC::PaintDC(HDC hDC
		,LONG x,LONG y,LONG cx,LONG cy
		,LONG px,LONG py,BOOL bAlpha)
	{
		if (!bAlpha)
			BitBlt(hDC_,x, y, cx, cy, hDC, px, py, SRCCOPY);
		else
		{
			BOOL bRet = AlphaBlend(hDC_,x, y, cx, cy, hDC,0,0, cx, cy,blen_);
			if (!bRet)
			{
				DWORD dw = GetLastError();
				LKDUI_TRACE(L" AlphaBlend ERROR [%d]",dw);
			}
		}

	}

	void CLkDuiDC::Copy(CLkDuiDC* pSourceDC,INT x,INT y,INT x1,INT y1)
	{
		if (pSourceDC && !pSourceDC->IsEmpty())
		{	
			BitBlt(hDC_,x, y, pSourceDC->GetCX(), pSourceDC->GetCY()
			, pSourceDC->GetHDC(), x1, y1, SRCCOPY);
			is_empty_ = FALSE;
		}

	}
	void CLkDuiDC::Add(CLkDuiDC* pSourceDC,INT x,INT y,INT x1,INT y1,INT cx1,INT cy1,INT alpha)
	{
		if (pSourceDC && !pSourceDC->IsEmpty())
		{
			if (cx1 == 0)
				cx1 = pSourceDC->GetCX();
			if (cy1 == 0)
				cy1 = pSourceDC->GetCY();
			blen_.SourceConstantAlpha = alpha;
			AlphaBlend(hDC_,x,y, cx1, cy1
			, pSourceDC->GetHDC(),x1,y1, cx1, cy1,blen_);
			is_empty_ = FALSE;
		}
	}

	void CLkDuiDC::SizeAdd(CLkDuiDC* pSourceDC,INT x,INT y,INT x1,INT y1,INT cx1,INT cy1,INT cx2,INT cy2,INT alpha)
	{
		if (pSourceDC && !pSourceDC->IsEmpty())
		{
			if (cx1 == 0)
				cx1 = GetCX();
			if (cy1 == 0)
				cy1 = GetCY();
			if (cx2 == 0)
				cx2 = pSourceDC->GetCX();
			if (cy2 == 0)
				cy2 = pSourceDC->GetCY();
			blen_.SourceConstantAlpha = alpha;
			AlphaBlend(hDC_,x,y, cx1, cy1
				, pSourceDC->GetHDC(),x1,y1, cx2, cy2,blen_);
			is_empty_ = FALSE;
		}
	}

	void CLkDuiDC::TurnAdd(CLkDuiDC* sou, float radians)
	{
/*
		float cosine = (float)cos(radians);  
		float sine = (float)sin(radians); 
		
		// 设置坐标远点
		int des_X_offset = GetCX()/2;
		int des_Y_offset = GetCY()/2;

		int sou_X_offset = sou->GetCX()/2;
		int sou_Y_offset = sou->GetCY()/2;


		for( int y = 0; y < size_.Height; y++ )  
		{  
			for( int x = 0; x < size_.Width; x++ )  
			{  
				int sourcex = (int)(x*cosine - y*sine);  
				int sourcey = (int)(y*cosine + x*sine);  
				if( sourcex >= 0 && sourcex < sou->GetCX() && sourcey >= 0  
					&& sourcey < sou->GetCY() ) 
				{
					COLORREF color1 = GetPixel(x,y);
					ColorAdd(color1,sou->GetPixel(sourcex,sourcey));
					SetPixel(x,y,color1);  
				}
			}  
		} */
		return;
	}

	void CLkDuiDC::EraseDC()
	{
		PatBlt(hDC_, 0, 0, size_.Width, size_.Height, BLACKNESS);
		is_empty_ = TRUE;
	}

	void CLkDuiDC::SetColor(Color col,BOOL bErase)
	{
		if (bErase)
			EraseDC();
		Gdiplus::Graphics graphics(hDC_);
		SolidBrush bh(col);
		graphics.FillRectangle(&bh,.0f,.0f,1.0f*size_.Width,1.0f*size_.Height);
		is_empty_ = FALSE;
	}

	void CLkDuiDC::SetTextColor(Color col)
	{
		txt_color_ = col;
	}

	void CLkDuiDC::SetImage(const CGdiPlusBitmapResource* image,BOOL bErase,float xBeg ,float yBeg)
	{
		if (bErase)
			EraseDC();
		Gdiplus::Graphics graphics(hDC_);
		graphics.DrawImage(*image,xBeg,yBeg);
		is_empty_ = FALSE;
	}

	void CLkDuiDC::SetImage(Image* img,BOOL bErase,float xBeg ,float yBeg)
	{
		if (bErase)
			EraseDC();
		Gdiplus::Graphics graphics(hDC_);
		graphics.DrawImage(img,xBeg,yBeg);
		is_empty_ = FALSE;
	}

	void CLkDuiDC::SetImage(const CGdiPlusBitmapResource* image,float fAngle,BOOL bErase)
	{

		Gdiplus::Graphics graphics(hDC_);
		graphics.TranslateTransform(0.5f*size_.Width,0.5f*size_.Height); // 将原点移动到(230,10)
		//graphics.TranslateTransform(0,0);
		graphics.RotateTransform(fAngle); // 顺时针旋转30度
		graphics.TranslateTransform(-0.5f*size_.Width,-0.5f*size_.Height); // 将原点移动到(230,10)
		Size sImage = image->GetSize();
		float fxBeg = 0.5f * (size_.Width - sImage.Width);
		float fyBeg = 0.5f * (size_.Height - sImage.Height);
		graphics.DrawImage(*image,fxBeg,fyBeg);
		is_empty_ = FALSE;
	}

	void CLkDuiDC::SetImage(Image* image,float fAngle,BOOL bErase)
	{

		Gdiplus::Graphics graphics(hDC_);
		graphics.TranslateTransform(0.5f*size_.Width,0.5f*size_.Height); // 将原点移动到(230,10)
		//graphics.TranslateTransform(0,0);
		graphics.RotateTransform(fAngle); // 顺时针旋转30度
		graphics.TranslateTransform(-0.5f*size_.Width,-0.5f*size_.Height); // 将原点移动到(230,10)
		Size sImage(image->GetWidth(),image->GetHeight());
		float fxBeg = 0.5f * (size_.Width - sImage.Width);
		float fyBeg = 0.5f * (size_.Height - sImage.Height);
		graphics.DrawImage(image,fxBeg,fyBeg);
		is_empty_ = FALSE;
	}

	void CLkDuiDC::TurnDC(Point pt,float fAngle)
	{
		Gdiplus::Graphics graphics(hDC_);
		graphics.TranslateTransform(pt.X,pt.Y); // 将原点移动到(230,10)
		graphics.RotateTransform(fAngle); // 顺时针旋转30度
	}

	void CLkDuiDC::DrawText(const TCHAR* pWndText,float xPoint,float yPoint,DWORD dwAlign,BOOL bMultiLine)
	{

		Gdiplus::Graphics graphics(hDC_);


		//-----------------------------------------  
		//stringformat主要用以下的函数来控制文本的显示方式  
		// SetAlignment  
		//  StringAlignmentNear(左对齐或右对齐，取决于书写方向是从左到右还是从右到左)、  
		//  StringAlignmentCenter(水平居中)  
		//  StringAlignmentFar(两端对齐)。  
		//  默认都不影响垂直  
		//-----------------------------------------  
		//  SetLineAlignment 
		//  和SetAlignment对应，为竖直的设置  
		//-----------------------------------------  
		StringFormat stringformat;  

		//GetTextBounds(*font_,stringformat,pWndText,txtSize);
		if (dwAlign & HORIZONTAL_LEFT)
		{	
			stringformat.SetAlignment(StringAlignmentNear); 
		}
		else if (dwAlign & HORIZONTAL_CENTER)
			stringformat.SetAlignment(StringAlignmentCenter);  
		else if (dwAlign & HORIZONTAL_RIGHT)
		{
			stringformat.SetAlignment(StringAlignmentFar); 
		}

		if (dwAlign & VERTICAL_LEFT)
			stringformat.SetLineAlignment(StringAlignmentNear); 
		else if (dwAlign & VERTICAL_CENTER)
			stringformat.SetLineAlignment(StringAlignmentCenter);  
		else if (dwAlign & VERTICAL_RIGHT)
			stringformat.SetLineAlignment(StringAlignmentCenter);  

		RectF layoutRc(xPoint,yPoint,1.0f*size_.Width-xPoint,1.0f*size_.Height-yPoint),
			txtRc(.0f,.0f,.0f,.0f);
		if (!bMultiLine)
			layoutRc.Width = 8000.0f;
		Status s = graphics.MeasureString(pWndText,-1, font_,layoutRc,&stringformat,&txtRc);
		SolidBrush brush(txt_color_);
		if (txtRc.Width > 1.0f*size_.Width-xPoint)
			txtRc.Width = 1.0f*size_.Width-xPoint;
		graphics.DrawString(pWndText,-1,font_,txtRc,&stringformat,&brush);
		is_empty_ = FALSE;
		return;
	}

	void CLkDuiDC::GetTextOutRect(TCHAR* szChar,HDC hDC,const RECT& rcWnd,RECT& rcOut,TCHAR* fontname,float fSize,DWORD dwAlign)
	{
		Gdiplus::Graphics graphics(hDC);


		//-----------------------------------------  
		//stringformat主要用以下的函数来控制文本的显示方式  
		// SetAlignment  
		//  StringAlignmentNear(左对齐或右对齐，取决于书写方向是从左到右还是从右到左)、  
		//  StringAlignmentCenter(水平居中)  
		//  StringAlignmentFar(两端对齐)。  
		//  默认都不影响垂直  
		//-----------------------------------------  
		//  SetLineAlignment 
		//  和SetAlignment对应，为竖直的设置  
		//-----------------------------------------  
		StringFormat stringformat;  

		//GetTextBounds(*font_,stringformat,pWndText,txtSize);
		if (dwAlign & HORIZONTAL_LEFT)
		{	
			stringformat.SetAlignment(StringAlignmentNear); 
		}
		else if (dwAlign & HORIZONTAL_CENTER)
			stringformat.SetAlignment(StringAlignmentCenter);  
		else if (dwAlign & HORIZONTAL_RIGHT)
		{
			stringformat.SetAlignment(StringAlignmentFar); 
		}

		if (dwAlign & VERTICAL_LEFT)
			stringformat.SetLineAlignment(StringAlignmentNear); 
		else if (dwAlign & VERTICAL_CENTER)
			stringformat.SetLineAlignment(StringAlignmentCenter);  
		else if (dwAlign & VERTICAL_RIGHT)
			stringformat.SetLineAlignment(StringAlignmentCenter);  

		RectF layoutRc(rcWnd.left,rcWnd.top,RC_WIDTH(rcWnd),RC_HIGH(rcWnd)),
			txtRc(.0f,.0f,.0f,.0f);
		Font font(fontname,fSize);
		Status s = graphics.MeasureString(szChar,-1, &font,layoutRc,&stringformat,&txtRc);
		rcOut.top = txtRc.GetTop();
		rcOut.left = txtRc.GetLeft();
		rcOut.bottom = txtRc.GetBottom();
		rcOut.right = txtRc.GetRight();
		return;
	}


	void CLkDuiDC::SetFont(LPCTSTR sFont,float fBom)
	{
		if (font_)
			delete font_;
		font_ = new Font(sFont,fBom);
	}
	
	void CLkDuiDC::GetTextBounds(const Font& font,const StringFormat& strFormat,const TCHAR* szText,SizeF& sizef)
	{
		GraphicsPath graphicsPathObj;
		FontFamily fontfamily;
		font.GetFamily(&fontfamily);
		graphicsPathObj.AddString(szText,-1,&fontfamily,font.GetStyle(),font.GetSize(),\
			PointF(0,0),&strFormat);
		RectF rcBound;
		/// 获取边界范围
		graphicsPathObj.GetBounds(&rcBound);
		sizef.Width = rcBound.Width;
		sizef.Height = rcBound.Height;
		/// 返回文本的宽高
		return;
	}

	
	bool CLkDuiDC::ImageFromSourceFile(Bitmap * & pImg,HMODULE hInst, UINT nResourceID, LPCTSTR lpType)
	{
		TCHAR tPath[MAX_PATH];
		if (GetTempPath(MAX_PATH, tPath) == 0)
		{
			_tcscpy(tPath, TEXT("c:\\"));
		}
		DWORD DW = rand();
		wsprintf(tPath,_T("%sqyimg%d%d.png"),tPath,GetTickCount(),DW);
		HANDLE hFile = CreateFile(tPath,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			DWORD dw = GetLastError();
			return false;
		}
		HRSRC	hResLoad;
		hResLoad = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(nResourceID), lpType);
		HGLOBAL hResData = LoadResource(NULL, hResLoad);
		LPCSTR	data = (LPCSTR)LockResource(hResData);
		if (hResLoad != NULL && hResData != NULL && data != NULL)
		{
			DWORD dwSize;
			WriteFile(hFile,data,SizeofResource(NULL, hResLoad),&dwSize,NULL);
		}
		CloseHandle(hFile);

		// load from stream这是关键一句，通过FromStream返回以各Image*，然后在Graphic的DrawImage地方调用就行了！
		pImg=Gdiplus::Bitmap::FromFile(tPath,TRUE);
		if (!pImg)
		{

/*
			TCHAR s[100];
			wsprintf(s,TEXT("图像载入失败4[%08X]"),GetLastError());
			MessageBox(NULL,s,L"error",MB_OK);*/
			return false;
		}
		DeleteFile(tPath);
		return true;
	} 


	bool CLkDuiDC::ImageFromSource(Bitmap * & pImg,HMODULE hInst, UINT nResourceID, LPCTSTR lpType)
	{
		//return ImageFromSourceFile(pImg,hInst,nResourceID,lpType);
		pImg = NULL;
		HRSRC hRsrc = ::FindResource (hInst,
			MAKEINTRESOURCE(nResourceID),
			lpType); // type
		if (!hRsrc)
		{
/*
			TCHAR s[100];
			wsprintf(s,TEXT("图像载入失败1[%08X]"),GetLastError());
			MessageBox(NULL,s,L"error",MB_OK);*/
			return false;
		}

		// load resource into memory
		DWORD len = SizeofResource(hInst, hRsrc);
		BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
		if (!lpRsrc)
		{
/*
			TCHAR s[100];
			wsprintf(s,TEXT("图像载入失败2[%08X]"),GetLastError());
			MessageBox(NULL,s,L"error",MB_OK);*/
			return false;
		}

		// Allocate global memory on which to create stream
		HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
		BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
		memcpy(pmem,lpRsrc,len);
		IStream* pstm;
		HRESULT res1 = CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
		if (res1 != S_OK)
		{
/*
			TCHAR s[100];
			wsprintf(s,TEXT("图像载入失败3[%08X]"),GetLastError());
			MessageBox(NULL,s,L"error",MB_OK);*/
			return false;
		}

		// load from stream这是关键一句，通过FromStream返回以各Image*，然后在Graphic的DrawImage地方调用就行了！
		pImg=Gdiplus::Bitmap::FromStream(pstm,TRUE);
		if (!pImg)
			ImageFromSourceFile(pImg,hInst,nResourceID,lpType);

		// free/release stuff
		GlobalUnlock(m_hMem);
		pstm->Release();
		FreeResource(lpRsrc);
		return true;
	} 

}
