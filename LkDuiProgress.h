#pragma once
#include "LkDuiBaseWnd.h"

namespace lkDui
{
	class CLkDuiProgress :
		public CLkDuiBaseWnd
	{
	public:
		CLkDuiProgress(Color col);
		~CLkDuiProgress(void);
	public:
		UINT Create(const TCHAR* wnd_name, CLkDuiBase* pBefore, INT x,INT y,UINT uIDBg,UINT uIDBef);
		void SetTotalStep(float fTotal,BOOL bReDraw);
		void SetStep(float fStep,BOOL bReDraw = TRUE);
	protected:
		void DrawItem() override;
		float total_step_;
		float step_;
		float fxBeg_;
		float fyBeg_;
		Size bg_img_;
		Size bef_img_;
	};
}

