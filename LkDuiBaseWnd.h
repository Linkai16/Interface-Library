#pragma once

#include "LkDuiDef.h"
#include "LkDuiBase.h"
#include "LkDuiDC.h"
#include "LkDuiTimer.h"

namespace lkDui	
{

	typedef UINT (CALLBACK *ButtonClick)(CLkDuiBase* trage,POINT pt);
	class CLkDuiTooltip;
	class CLkDuiBaseWnd :
		public CLkDuiBase
	{
	public:
		CLkDuiBaseWnd(Color col);
		virtual ~CLkDuiBaseWnd(void);
	public:
		virtual void Paint(CLkDuiDC* parentDC,INT xPos = 0,INT yPos = 0) override;
		virtual void ShowWnd(BOOL bShow) override;	

		// 话背景
		virtual void DrawItem() override;

		// 设置字体
		void SetFont(LPCTSTR sFont,float fBom);
		void SetTxtColor(Color col);

		// 设置窗口字体
		virtual void SetWndText(const TCHAR*,DWORD dwalign = HORIZONTAL_LEFT| VERTICAL_CENTER);
		virtual DWORD GetWndText(TCHAR*,DWORD dwLen);
		// 父窗口相对位置
		virtual BOOL SetWndRect(const RECT&) override;
		virtual UINT Create(const TCHAR* wnd_name, CLkDuiBase* pBefore, const RECT& rc) override;

		// 设置背景颜色或图片
		virtual void SetBackground(Color,BOOL bErase,BOOL bRefalsh = TRUE);
		virtual void SetBackground(DWORD dwimgid,BOOL bErase,BOOL bRefalsh = TRUE);

		// 设置背景颜色或图片
		virtual void SetBeforeground(Color,BOOL bErase,BOOL bRefalsh = TRUE);
		virtual void SetBeforeground(DWORD dwimgid,BOOL bErase,BOOL bRefalsh = TRUE);

		virtual void Destroy(BOOL bRemoveFromParent = TRUE);

		virtual void EnableToolTip(BOOL bEnable = TRUE);
		
	public:
		virtual void NotifyMouseIn(BOOL bInWnd) override;
		virtual void NotifyMouseLDown(BOOL bDwon,POINT pt) override;
		virtual void NotifyMouseRDown(BOOL bDwon,POINT pt) override;
		virtual void NotifyMouseHover(POINT pt) override;

		void AddCallback(ButtonClick func)
		{
			func_btClick_ = func;
		}
	protected:
		TCHAR wnd_text_[80];
		CLkDuiDC*    dc_main_mem_;
		CLkDuiDC*    dc_background_;
		CLkDuiDC*    dc_beforeground_;
		CLkDuiDC*    dc_show_;
		BLENDFUNCTION  blen_;
		Color  bg_color_;
		Color  bef_color_;
		Color  txt_color_;
		DWORD  txt_align_;
		ButtonClick  func_btClick_;
		CRITICAL_SECTION callback_lock_;
		CLkDuiTooltip*   tool_tip_;
	};

}
