#include "LkDuiBase.h"
#include "LkDuiCore.h"
#include "LkDuiDef.h"
#include "../resource.h"

namespace lkDui
{

	CLkDuiBase::CLkDuiBase(void):
is_main_wnd_(FALSE),
pUserData_(NULL),
hWnd_(NULL),
bShow_(TRUE),
Parent_Wnd_(NULL),
is_created_(FALSE),
is_erase_bg_(TRUE),
is_wnd_painting_(FALSE),
is_capture_(FALSE),
mouse_wnd_(NULL),
is_wnd_image_chaged_(FALSE),
is_ToolWnd_(FALSE),
is_enable_(TRUE),
func_usermsg_(NULL)
{
	ZeroMemory(wnd_name_,sizeof(wnd_name_));
	InitializeCriticalSection(&dc_lock_);
}

CLkDuiBase::~CLkDuiBase(void)
{
	if (IsCreated())
		Destroy();
}


// 相对桌面窗口的位置
void CLkDuiBase::GetLastRect(RECT* rc)
{
	GetClientRect(rc);
	if (Parent_Wnd_)
	{
		RECT rcOffset;
		Parent_Wnd_->GetLastRect(&rcOffset);
		rc->top += rcOffset.top;
		rc->left += rcOffset.left;
		rc->bottom += rcOffset.top;
		rc->right += rcOffset.left;
	}

}

// 创建
UINT CLkDuiBase::Create(const TCHAR* wnd_name,		// 窗口名称
						CLkDuiBase* pBefore,		// 上一级子窗口
						const RECT& rc)			// 相对上一级窗口的偏移	
{

	if (!pBefore)
	{
		// 创建一个父窗口
		hWnd_ = CreateMainFrame(GetModuleHandle(NULL),IDI_ICON_QYAZ);
		is_main_wnd_ = TRUE;
	}
	else
	{
		hWnd_ = pBefore->GetMainHWnd();
		pBefore->AddChild(this);
	}
	SetWndRect(rc);
	SetWndName(wnd_name);
	is_created_ = TRUE;
	return 1;
}


void CLkDuiBase::Destroy(BOOL bRemoveFromParent)
{
	is_created_ = FALSE;
	ChildList::iterator it = child_list_.begin();
	while (it != child_list_.end())
	{
		if ((*it)->IsCreated())
			(*it)->Destroy(FALSE);
		it = child_list_.erase(it);
	}
	if (is_main_wnd_)
	{
		DestroyWindow(hWnd_);
	}
	if (bRemoveFromParent && Parent_Wnd_)
	{
		Parent_Wnd_->RemoveChild(this);
	}
	child_list_.clear();
	DeleteCriticalSection(&dc_lock_);
	
}


// 相对于最上级窗口的位置
void CLkDuiBase::GetClientRect(RECT* rc)
{
	memcpy(rc,&rect_,sizeof(RECT));
	return;
}


void  CLkDuiBase::ClientToClient(RECT* rc)
{
	rc->left += rect_.left;
	rc->right += rect_.right;
	rc->top  += rect_.top;
	rc->bottom += rect_.bottom;
}



// 设置父窗口
void  CLkDuiBase::SetParent(CLkDuiBase* pParent,BOOL bAddChild)
{
	Parent_Wnd_ = pParent;
	if (bAddChild)
		Parent_Wnd_->AddChild(this,FALSE);
}
// 添加子窗口
void  CLkDuiBase::AddChild(CLkDuiBase* pChild,BOOL bSetParent)
{
	child_list_.push_back(pChild);
	if (bSetParent)
		pChild->SetParent(this,FALSE);
}

void CLkDuiBase::RemoveChild(CLkDuiBase* pChild)
{
	ChildList::iterator it = child_list_.begin();
	while (it != child_list_.end())
	{
		if (*it == pChild)
		{
			it = child_list_.erase(it);
			break;
		}
		it++;
	}
}


CLkDuiBase* CLkDuiBase::FindChildByName(TCHAR* name)
{
	if (!is_created_)
		return NULL;
	ChildList::iterator it = child_list_.begin();
	while (it != child_list_.end())
	{
		if (_tcscmp(name,(*it)->GetWndName()) == 0)
		{
			return (*it);
		}
		it++;
	}
	return NULL;
}


void CLkDuiBase::OnPaint()
{
	if (!is_wnd_painting_)
	{
		is_wnd_painting_ = TRUE;
		if (!is_main_wnd_)
		{
			if (Parent_Wnd_)
				Parent_Wnd_->OnPaint();
		}
		else
			Paint(NULL);
		is_wnd_painting_ = FALSE;
	}
}

HWND CLkDuiBase::CreateMainFrame(HINSTANCE hInstance,DWORD dwIcon)
{
	RegisterMainClass(hInstance,dwIcon);
	// Create the main window initially hidden.
	DWORD dwStyleEx = WS_EX_LAYERED | WS_EX_APPWINDOW;
	if (is_ToolWnd_)
		dwStyleEx = WS_EX_LAYERED | WS_EX_TOOLWINDOW;
	return CreateWindowEx(dwStyleEx,  
		class_name_,  
		_T(""),  
		WS_POPUP,  
		-1, -1, 400, 300,  
		NULL, NULL, hInstance, NULL); 
}

CLkDuiBase*  CLkDuiBase::FindDuiWndByPoint(POINT& pt)
{
	POINT p1;
	p1.x = rect_.left;
	p1.y = rect_.top;
	PT_OFFSET(pt,p1)

		 
	ChildList::reverse_iterator rIt = child_list_.rbegin();
	while (rIt != child_list_.rend())
	{
		RECT rc;
		(*rIt)->GetClientRect(&rc);
		if ((*rIt)->IsShow() && PtInRect(&rc,pt))
		{
			return (*rIt)->FindDuiWndByPoint(pt);
		}
		rIt++;
	}
	return this;
}

void CLkDuiBase::SetCapture()
{
	is_capture_ = TRUE;
	::SetCapture(hWnd_);
}
void CLkDuiBase::ReleaseCapture()
{
	is_capture_ = FALSE;
	::ReleaseCapture();
}

LRESULT CLkDuiBase::OnMouseEvent(UINT mouse,POINT p)
{
	LRESULT lRet = 0;
	POINT ptClient;
	ptClient = p;
	CLkDuiBase* pLkDuiWnd = FindDuiWndByPoint(ptClient);
	if (mouse == WM_MOUSEMOVE)
	{
		if (!is_capture_ && pLkDuiWnd != mouse_wnd_)
		{
			pLkDuiWnd->NotifyMouseIn(TRUE);
			if ( mouse_wnd_)
				mouse_wnd_->NotifyMouseIn(FALSE);
			mouse_wnd_ = pLkDuiWnd;
		}
	}
	else if (mouse  ==  WM_LBUTTONDOWN)
	{
		SetCapture();
		mouse_wnd_->NotifyMouseLDown(TRUE,ptClient);
	}
	else if (mouse  ==  WM_RBUTTONDOWN)
	{
		SetCapture();
		mouse_wnd_->NotifyMouseRDown(TRUE,ptClient);
	}
	else if (mouse  ==  WM_LBUTTONUP)
	{
		ReleaseCapture();
		mouse_wnd_->NotifyMouseLDown(FALSE,ptClient);
	}
	else if (mouse  ==  WM_RBUTTONDOWN)
	{
		SetCapture();
		mouse_wnd_->NotifyMouseRDown(TRUE,ptClient);
	}
	else if (mouse ==  WM_MOUSEHOVER)
	{
		mouse_wnd_->NotifyMouseHover(p);
	}
	return lRet;
}

LRESULT CLkDuiBase::OnUserMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (func_usermsg_)
		return func_usermsg_(message,wParam,lParam);
	return 0;
}

LRESULT CLkDuiBase::OnSize(SIZE s)
{
	LRESULT lRet = 0;
	return lRet;
}

LRESULT CLkDuiBase::OnMove(POINT p)
{
	rect_.right = RC_WIDTH(rect_) + p.x;
	rect_.bottom = RC_HIGH(rect_) + p.y;
	rect_.left = p.x;
	rect_.top = p.y;
	return 0;
}

LRESULT CLkDuiBase::OnCommand(UINT id)
{
	LRESULT lRet = 0;
	return lRet;
}

LRESULT CLkDuiBase::OnTimer(UINT id)
{
	LRESULT lRet = 0;
	return lRet;
}

}
