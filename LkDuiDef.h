#pragma once
#ifndef LK_DUI_DEF_HANDLE
#define LK_DUI_DEF_HANDLE

#include <windows.h>
#include <list>
#include <vector>
#include "CGdiPlusBitmap.h"
namespace lkDui
{
#define RC_LEFT(A) ((A).left)
#define RC_RIGHT(A) ((A).right)
#define RC_TOP(A)   ((A).top)
#define RC_BOTTOM(A) ((A).bottom)
#define RC_WIDTH(A) ((A).right - (A).left)
#define RC_HIGH(A) ((A).bottom - (A).top)

#define RC_OFFSET(A,B) A.top += B.y;A.bottom += B.y;A.left += B.x;A.right += B.x;

#define PT_OFFSET(A,B) A.x -= B.x;A.y -= B.y;


#define FULL_ALPHA   0xfe

#define HORIZONTAL_LEFT		(0x01)
#define HORIZONTAL_CENTER   (0x02)
#define HORIZONTAL_RIGHT    (0x04)
#define VERTICAL_LEFT		(0x08)
#define VERTICAL_CENTER		(0x10)
#define VERTICAL_RIGHT		(0x20)

enum enum_check_style
{
	
	CHECK_STYLE_ALL = 0,
	CHECK_STYLE_MID,
	CHECK_STYLE_NONE,
};


typedef UINT (CALLBACK * FuncUserMsg)(UINT msg,WPARAM wParam,LPARAM lParam);

void lkDuiTrace(TCHAR* lpszFormat, ...);

#ifdef _DEBUG
#define LKDUI_TRACE(a,...) lkDuiTrace(a,__VA_ARGS__)
#else
#define LKDUI_TRACE(a,...)
#endif



typedef struct _tag_imageInfo 
{
	float fSpeed;
	float fAngle;
	Gdiplus::Bitmap* pImage;
}lkDuiImage,*LPlkDuiImage;

typedef std::list<lkDuiImage> ImageList;
typedef std::vector<lkDuiImage> ImageArray;


typedef unsigned char TUInt8; // [0..255]
struct TARGB32      //32 bit color
{
	TUInt8  b,g,r,a;          //a is alpha
};

void RC_XMID(const RECT* rc1,RECT* rc2);

void RC_YMID(const RECT* rc1,RECT* rc2);


}


#endif