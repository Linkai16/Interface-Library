#pragma once
#include "LkDuiBaseWnd.h"


namespace lkDui
{
	class CLkDuiImgDisplay :
		public CLkDuiBaseWnd,
		public CLkDuiTimer::Delegate
	{
	public:
		CLkDuiImgDisplay(Color col);
		~CLkDuiImgDisplay(void);
	public:
		void AddImage(DWORD dwID,LPCTSTR sZY,BOOL bFlash);
		void ShowWnd(BOOL bShow) override;
		void Play(UINT nInterval);
		void Stop();
	protected:
		void OnTime(UINT id) override;
		void DrawItem() override;
		void OnChangeImage();
		const lkDuiImage& GetCurrentImg();
		const lkDuiImage& GetNextImg();
	protected:
		ImageArray::size_type  show_pos_;
		BYTE		byAlpha_;
		UINT        display_interval_;
		ImageArray image_array_;		
		CLkDuiTimer* timer_;
	};
}

