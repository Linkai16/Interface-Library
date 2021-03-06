#pragma once

#include "LkDuiDef.h"

using namespace Gdiplus;  
#pragma   comment(lib,"Msimg32.lib")
#pragma comment(lib, "gdiplus.lib")

namespace lkDui
{

	class CLkDuiDC
	{
	public:
		CLkDuiDC(HWND ,const RECT&);
		CLkDuiDC(HWND ,const Size&);
		~CLkDuiDC(void);

		void PaintDC(HDC hDC
			,LONG x,LONG y,LONG cx,LONG cy
			,LONG px,LONG py,BOOL bAlpha = TRUE);

		void Copy(CLkDuiDC* ,INT x = 0,INT y = 0,INT x1 = 0,INT y1 = 0);
		void Add(CLkDuiDC* ,INT x = 0,INT y = 0,INT x1 = 0,INT y1 = 0,INT cx1 = 0,INT cy1 = 0,INT alpha = FULL_ALPHA);
		void SizeAdd(CLkDuiDC* ,INT x = 0,INT y = 0,INT x1 = 0,INT y1 = 0,INT cx1 = 0,INT cy1 = 0,INT cx2 = 0,INT cy2 = 0,INT alpha = FULL_ALPHA);
		void TurnAdd(CLkDuiDC* sou, float radians);	// 有问题
		void SetFont(LPCTSTR sFont,float emSize);
		void EraseDC();
		void SetColor(Color col,BOOL bErase);
		void SetTextColor(Color col);
		void SetImage(const CGdiPlusBitmapResource* image,BOOL bErase,float xBeg = .0f,float yBeg = .0f);
		void SetImage(const CGdiPlusBitmapResource* image,float fAngle,BOOL bErase);
		void SetImage(Image* img,BOOL bErase,float xBeg = .0f,float yBeg = .0f);
		void SetImage(Image* image,float fAngle,BOOL bErase);
		void DrawText(const TCHAR* pWndText,float xPoint,float yPoint,DWORD dwAlign,BOOL bMultiLine = TRUE);
		void TurnDC(Point pt,float fAngle);
		void InitByImage(const CGdiPlusBitmapResource*);		// 根据图片创建DC
		void Init(const SizeF*);			// 根据当前窗口创建DC
		void Release();			// 释放DC
		INT GetCX()
		{
			return size_.Width;
		}

		INT GetCY()
		{
			return size_.Height;
		}

		HDC GetHDC()
		{
			return hDC_;
		}
		BOOL IsEmpty()
		{
			return is_empty_ /*|| !is_inited_*/;
		}
		
		COLORREF GetPixel(int x,int y)
		{
			return ::GetPixel(hDC_,x,y);
		}

		void SetPixel(int x,int y,COLORREF col)
		{
			::SetPixel(hDC_,x,y,col);
		}

		static void ColorAdd(TARGB32& col1,const TARGB32& col2)
		{
			col1.a = (col2.a + col1.a) % FULL_ALPHA;
			col1.r = col2.r;
			col1.g = col2.g;
			col1.b = col2.b;
		}
/*
		BOOL isInited()
		{
			return is_inited_;
		}*/
		void GetTextBounds(const Font& font,const StringFormat& strFormat,const TCHAR* szText,SizeF& sizef);
		

		static bool ImageFromSourceFile(Bitmap * & pImg,HMODULE hInst, UINT nResourceID, LPCTSTR lpType);
		static bool ImageFromSource(Bitmap * & pImg,HMODULE hInst, UINT nResourceID, LPCTSTR lpType);
		static void GetTextOutRect(TCHAR* szChar,HDC hDC,const RECT& rcWnd,RECT& rcOut,TCHAR* fontname,float fSize,DWORD dwAlign = HORIZONTAL_LEFT | VERTICAL_LEFT);
	protected:
		//BOOL			is_inited_;		// DC是否初始化
		BOOL			is_empty_;		// DC是否为空
		Color            txt_color_;
		BLENDFUNCTION	blen_;
		Size			size_;
		HDC				hDC_;
		HBITMAP			hBmp_;
		HBITMAP			hOld_;
		Font*			font_;
	};

}

