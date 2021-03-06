#pragma once
#include "LkDuiBaseWnd.h"
#include "LkDuiTimer.h"

namespace lkDui
{
	class CLkDuiButton :
		public CLkDuiBaseWnd,
		public CLkDuiTimer::Delegate
	{
	public:
		CLkDuiButton(Color col);
		~CLkDuiButton(void);
	public:
		UINT Create(
			const TCHAR* wnd_name,		// 窗口名称
			CLkDuiBase* pBefore,		// 上一级子窗口
			INT   x,
			INT   y,
			UINT uImageID,				// 设置前景图片
			UINT nAvgelen);				// 前景图片长度
		void SetBeforeground(DWORD dwimgid,UINT nAvgelen,BOOL bErase,BOOL bRefalsh);
		void DrawItem() override;
	public:
		void NotifyMouseIn(BOOL bIn) override;
		void NotifyMouseLDown(BOOL bDwon,POINT pt) override;
		void NotifyMouseRDown(BOOL bDwon,POINT pt) override;
	protected:
		virtual void OnTime(UINT uTimerID) override;
	protected:
		CLkDuiTimer* timer_;
		INT			 mouse_in_;
		Size         bef_size_;
		INT          bef_Avgelen_;
		INT          bef_show_index_;
	};
}

