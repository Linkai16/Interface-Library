#pragma once
#include "LkDuiBaseWnd.h"


namespace lkDui
{

	class CLkDuiCheckBox :
		public CLkDuiBaseWnd
	{
	public:
		CLkDuiCheckBox(Color col);
		~CLkDuiCheckBox(void);
	public:
		UINT Create(
			const TCHAR* wnd_name,		// 窗口名称
			CLkDuiBase* pBefore,		// 上一级子窗口
			RECT rc,					// 窗口大小
			UINT ubkImageID,			// 背景图片
			UINT nbkAvgelen,			// 背景图片长度
			UINT ubefImageID,			// 前景图片
			UINT nbefAvgelen);			// 前景图片长度	

		void DrawItem() override;
		enum_check_style GetCheck()
		{
			return check_style_;
		}

		void SetCheck(enum_check_style);
		void SetIndexLimit(INT nLimit)
		{
			bg_index_limit_ = nLimit;
		}
	public:
		void NotifyMouseIn(BOOL bIn) override;
		void NotifyMouseLDown(BOOL bDwon,POINT pt) override;
		void NotifyMouseRDown(BOOL bDwon,POINT pt) override;
	protected:
		SizeF				bef_size_;
		SizeF				bg_size_;
		INT					bg_show_index_;
		INT					bg_index_limit_;
		enum_check_style    check_style_;
	};

	
}

