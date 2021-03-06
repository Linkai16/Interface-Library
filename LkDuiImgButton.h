#pragma once
#include "LkDuiBaseWnd.h"


namespace lkDui
{
	class CLkDuiImgButton :
		public CLkDuiBaseWnd
	{
	public:
		CLkDuiImgButton(Color col);
		~CLkDuiImgButton(void);
	public:
		UINT Create(
			const TCHAR* wnd_name,		// 窗口名称
			CLkDuiBase* pBefore,		// 上一级子窗口
			INT   x,
			INT   y,
			UINT uImageID,
			UINT nAvgelen);
		void SetBackground(DWORD dwimgid,UINT nAvgelen,BOOL bErase,BOOL bRefalsh);
		void DrawItem() override;
		void SetTxtColor(Color col1,Color col2,Color col3);
		void EnableWnd(BOOL bEnable,DWORD dwID);
	public:
		void NotifyMouseIn(BOOL bIn) override;
		void NotifyMouseLDown(BOOL bDwon,POINT pt) override;
		void NotifyMouseRDown(BOOL bDwon,POINT pt) override;
	protected:
		INT			 mouse_in_;
		Size         bg_size_;
		INT          bef_Avgelen_;
		INT          bef_show_index_;
		Color		 txt_color_[3];
	};
}

