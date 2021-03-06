#pragma once

#include <map>
#include <Windows.h>


namespace lkDui
{
	class CLkDuiTimer
	{
	public:

		class Delegate {
		public:
			// Called when the BrowserWindow has been destroyed.
			virtual void OnTime(UINT uTimerID) = 0;
		protected:
			virtual ~Delegate() {}
		};

		typedef std::map<UINT,Delegate*> Timer_Map;
	public:
		CLkDuiTimer(Delegate* delegate);
		~CLkDuiTimer(void);

	public:
		BOOL  AddTimer(HWND hWnd,UINT uTimer,UINT uElapse);
		void  RemoveTimer(HWND hWnd,UINT uTimer);
		static VOID CALLBACK OnTimer(HWND, UINT, UINT_PTR, DWORD);
		static Timer_Map   times_;
		static INT         count_;
	protected:
		Delegate* delegate_;
	};
}

