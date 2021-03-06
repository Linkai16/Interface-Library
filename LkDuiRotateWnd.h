#pragma once
#include "LkDuiBaseWnd.h"
#include "LkDuiTimer.h"

namespace lkDui
{


	class CLkDuiRotateWnd :
		public CLkDuiBaseWnd,
		public CLkDuiTimer::Delegate
	{
	public:
		CLkDuiRotateWnd(Color col);
		~CLkDuiRotateWnd(void);
	public:
		BOOL AddImage(DWORD dwID,LPCTSTR sZY,BOOL bPaint,float fSpeed);
		void SetBackground(DWORD dwimgid,BOOL bErase,BOOL bRefalsh = TRUE) override;
		void Stop();
		void Play();
		void ShowWnd(BOOL bShow) override;
	protected:
		void DrawItem() override;
		void MakeBeforeground();
		void OnTime(UINT uTimerID) override;

	protected:
		ImageList image_list_;
		CLkDuiTimer* timer_;
	};
}

