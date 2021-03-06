
#pragma once
#include <windows.h>
#include <list>
#include <tchar.h>
#include "CGdiPlusBitmap.h"
#include "LkDuiDC.h"

using namespace std;


namespace lkDui
{
	class CLkDuiBase
	{
		typedef std::list<CLkDuiBase*>   ChildList;
	public:
		CLkDuiBase(void);
		virtual ~CLkDuiBase(void);

		HWND   CreateMainFrame(HINSTANCE hInstance,DWORD dwIcon);

		// 创建
		virtual UINT Create(
			const TCHAR* wnd_name,		// 窗口名称
			CLkDuiBase* pBefore,		// 上一级子窗口
			const RECT& rc);			// 相对上一级窗口的偏移	

		virtual void Destroy(BOOL bRemoveFromParent = TRUE);

		
		virtual void ShowWnd(BOOL bShow) = 0;	
		virtual void DrawItem() = 0;

		// 设置窗口字体
		virtual void SetWndText(const TCHAR*,DWORD dwalign) = 0;
		virtual DWORD GetWndText(TCHAR*,DWORD dwLen) = 0;
		// 父窗口相对位置
		virtual BOOL SetWndRect(const RECT&) = 0;
				
		// 设置父窗口
		virtual void SetParent(CLkDuiBase* pParent,BOOL bAddChild = TRUE);
		// 添加子窗口
		virtual void AddChild(CLkDuiBase* pChild,BOOL bSetParent = TRUE);
		virtual void RemoveChild(CLkDuiBase* pChild);

		
		// 相对上一级窗口的位置
		virtual void GetLastRect(RECT* rc);
		// 相对于最上级窗口的位置
		virtual void GetClientRect(RECT* rc);
		// 将子窗口的相对坐标转换为当前窗口的相对坐标
		virtual void ClientToClient(RECT* rc);
		// 重绘窗口
		virtual void Invalid(BOOL bPaint = FALSE)
		{
			is_erase_bg_ = TRUE;
			if (bPaint)
				OnPaint();
		}

		void SetCapture();
		void ReleaseCapture();
	public:
		// 处理窗口消息
		LRESULT OnMouseEvent(UINT mouse,POINT p);
		LRESULT OnSize(SIZE s);
		LRESULT OnMove(POINT p);
		LRESULT OnCommand(UINT id);
		LRESULT OnTimer(UINT id);
		LRESULT OnUserMsg(UINT message,WPARAM wParam,LPARAM lParam);
	public:
		virtual void NotifyMouseIn(BOOL bInWnd) = 0;
		virtual void NotifyMouseLDown(BOOL bDwon,POINT pt) = 0;
		virtual void NotifyMouseRDown(BOOL bDwon,POINT pt) = 0;
		virtual void NotifyMouseHover(POINT pt) = 0;

	public:
		CLkDuiBase* FindChildByName(TCHAR* name);
		void OnPaint();
		const TCHAR* GetWndName()
		{
			return wnd_name_;
		}

		void  SetWndName(const TCHAR* name)
		{
			_tcscpy_s(wnd_name_,sizeof(wnd_name_)/sizeof(TCHAR),name);
		}

		// 设置用户数据
		void SetData(LPVOID userdata)
		{
			pUserData_ = userdata;
		}
		void* GetData()
		{
			return pUserData_;
		}
		BOOL IsShow()
		{
			return bShow_;
		}
		BOOL IsCreated()
		{
			return is_created_;
		}	
		CLkDuiBase* GetParent()
		{
			return Parent_Wnd_;
		}
		HWND GetMainHWnd()
		{
			return hWnd_;
		}
		BOOL IsWnd(TCHAR* name)
		{
			return (_tcscmp(wnd_name_,name) == 0);
		}
		BOOL EnableWnd(BOOL bEnable)
		{
			is_enable_ = bEnable;
		}
		BOOL OnSysClose()
		{
			return FALSE;
		}
		void SetFuncUserMsg(FuncUserMsg func)
		{
			func_usermsg_ = func;
		}
		virtual void Paint(CLkDuiDC*,INT xPos = 0,INT yPos = 0) = 0;
	protected:
		CLkDuiBase*  FindDuiWndByPoint(POINT& pt);
	protected:
		BOOL   is_main_wnd_;
		BOOL   is_wnd_painting_;	// 主窗口正在刷新
		BOOL   is_wnd_image_chaged_;	// 窗口是否需要更新(暂时不用)
		BOOL   is_erase_bg_;			// 是否需要擦除背景
		BOOL   is_enable_;
		LPVOID pUserData_;
		RECT   rect_;
		HWND   hWnd_;
		BOOL   bShow_;
		CLkDuiBase* Parent_Wnd_;
		ChildList  child_list_;
		BOOL   is_created_;
		TCHAR  wnd_name_[80];
		FuncUserMsg func_usermsg_;
	protected:
		CRITICAL_SECTION dc_lock_;
		BOOL   is_capture_;
	protected:
		CLkDuiBase* mouse_wnd_;		// 当前鼠标下的窗口
		BOOL        is_ToolWnd_;
		
	};// clkduibase

}// lkdui
