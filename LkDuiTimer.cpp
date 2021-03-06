#include "LkDuiTimer.h"


namespace lkDui
{
	CLkDuiTimer::Timer_Map   CLkDuiTimer::times_;
	INT  CLkDuiTimer::count_ = 0;

	CLkDuiTimer::CLkDuiTimer(Delegate* delegate):
		delegate_(delegate)
	{
		count_++;
	}

	CLkDuiTimer::~CLkDuiTimer(void)
	{
		Timer_Map::iterator it = times_.begin();
		while (it != times_.end())
		{
			if (it->second == delegate_)
				it = times_.erase(it);
			else
				it++;
		}
		if (--count_ == 0)
			times_.clear();

	}


	VOID CLkDuiTimer::OnTimer(HWND a, UINT b, UINT_PTR c, DWORD d)
	{
		Delegate* gt = times_[c];
		if (gt)
			gt->OnTime(c);
	}


	BOOL  CLkDuiTimer::AddTimer(HWND hWnd,UINT uTimer,UINT uElapse)
	{
		Delegate* ts = times_[uTimer];
		if (ts != NULL)
			return FALSE;
		times_[uTimer] = delegate_;
		SetTimer(hWnd,uTimer,uElapse,OnTimer);
		return TRUE;
	}
	void  CLkDuiTimer::RemoveTimer(HWND hWnd,UINT uTimer)
	{
		times_.erase(uTimer);
		KillTimer(hWnd,uTimer);
	}
}

