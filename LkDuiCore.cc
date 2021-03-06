#include "LkDuiCore.h"



namespace lkDui
{

	const TCHAR class_name_[] = _T("LkDuiWnd");
	ULONG_PTR g_gdiplusToken;
	MSG_LOOP_SET g_msgloops;
	BOOL         g_bRunning = FALSE;
	HANDLE	     g_hthExitWnd = NULL;
	int			 g_colBit = 32;
	//typedef std::list<HWND>  FOCUS_LIST;
	//FOCUS_LIST  g_foucs_list;
	typedef  std::list<LPCoreInfo> KILLING_LIST;
	KILLING_LIST  g_killingthList;
	CRITICAL_SECTION g_lock_klist;

	void LkDuiStartup()
	{
		GdiplusStartupInput gdiplusStartupInput;
		Status s = GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);
		InitializeCriticalSection(&g_lock_klist);
		if (s == Ok)
		{
			HDC hTemp = GetDC(GetDesktopWindow());
			g_colBit = ::GetDeviceCaps(hTemp, BITSPIXEL);
			//int a = g_colBit;
			g_hthExitWnd = CreateThread(NULL,0,thExitWnd,NULL,NULL,NULL);
			if (!g_hthExitWnd)
				g_bRunning = FALSE;
			else
				g_bRunning = TRUE;
		}
/*
		else
		{
			TCHAR tCode[100];
			wsprintf(tCode,L"GdiplusStartup error[%d]",s);
			MessageBox(NULL,tCode,TEXT("error"),MB_OK);
			g_bRunning = FALSE;
		}*/
	}
	void LkDuiShutdown()
	{
		GdiplusShutdown(g_gdiplusToken);
		g_msgloops.clear();
		
		g_bRunning = FALSE;
		if (WaitForSingleObject(g_hthExitWnd,5000) == WAIT_TIMEOUT)
		{
			TerminateThread(g_hthExitWnd,1);
			g_hthExitWnd = INVALID_HANDLE_VALUE;
		}

		g_killingthList.clear();
		DeleteCriticalSection(&g_lock_klist);
		
	}



	void RegeditMsgloop(CLkDuiBase* lkdui)
	{
		LPCoreInfo pCoreInfo = new CoreInfo;
		pCoreInfo->main_wnd_ = lkdui;
		InitializeCriticalSection(&pCoreInfo->msg_lock_);
		pCoreInfo->msg_thread_ = TRUE;
		pCoreInfo->msg_handle_ = CreateThread(NULL,0,WndProcDeal,(LPCoreInfo)pCoreInfo,0,NULL);
		if (pCoreInfo->msg_handle_ == INVALID_HANDLE_VALUE)
		{
			DeleteCriticalSection(&pCoreInfo->msg_lock_);
			delete pCoreInfo;
			return;
		}
		g_msgloops[lkdui->GetMainHWnd()] = pCoreInfo;
	}

	ATOM RegisterMainClass(HINSTANCE hInstance,DWORD dwIcon) {
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style         = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc   = WndProc;
		wcex.cbClsExtra    = 0;
		wcex.cbWndExtra    = 0;
		wcex.hInstance     = hInstance;
		wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(dwIcon));
		wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName  = NULL;
		wcex.lpszClassName = class_name_;
		wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(dwIcon));

		return RegisterClassEx(&wcex);
	}



	bool WndIsExisting(HWND hWnd)
	{
		KILLING_LIST::const_iterator cit = g_killingthList.begin();
		while (cit != g_killingthList.end())
		{
			if ((*cit)->main_wnd_->GetMainHWnd() == hWnd)
				return true;
		}
		return false;
	}

	int  RunMessageLoop(HWND hWnd)
	{
		MSG msg;
		// Run the application message loop.
		while (::IsWindow(hWnd) && GetMessage(&msg, hWnd, 0, 0)) {
			// Allow processing of find dialog messages.
			/*
			if (hFindDlg && IsDialogMessage(hFindDlg, &msg))
			continue;*/

			if (!TranslateMessage(&msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_DESTROY)
				break;
		}
		while (WndIsExisting(hWnd))
		{
			Sleep(50);
		}
		return static_cast<int>(msg.wParam);
	}

	DWORD   WINAPI   thExitWnd(LPVOID lpMsg)
	{
		while (g_bRunning)
		{
			KILLING_LIST::iterator it = g_killingthList.begin();
			while (g_killingthList.end() != it)
			{		
				(*it)->msg_thread_ = FALSE;
				DWORD dwRet = WaitForSingleObject((*it)->msg_handle_,2000);
				if (dwRet == WAIT_TIMEOUT)
				{
					TerminateThread((*it)->msg_handle_,1);
					(*it)->msg_handle_ = INVALID_HANDLE_VALUE;
				}
				(*it)->msg_list_.clear();
				DeleteCriticalSection(&(*it)->msg_lock_);	
				delete (*it);
				*it = NULL;
				EnterCriticalSection(&g_lock_klist);
				it = g_killingthList.erase(it);
				LeaveCriticalSection(&g_lock_klist);
			}
			Sleep(1);
		}
		return 0;
	}


	DWORD WINAPI WndProcDeal(LPVOID lpMsg)
	{
		LPCoreInfo pCoreInfo = (LPCoreInfo)lpMsg;
		while (pCoreInfo->msg_thread_)
		{
			MSG_LIST::iterator it = pCoreInfo->msg_list_.begin();
			while (it != pCoreInfo->msg_list_.end())
			{
				STLkDuiMsg msg = (*it);
				EnterCriticalSection(&pCoreInfo->msg_lock_);
				it = pCoreInfo->msg_list_.erase(it);
				LeaveCriticalSection(&pCoreInfo->msg_lock_);
				if (msg.pWnd)
				{
					switch(msg.msgid)
					{
					case WM_MOVE:
						{
							POINT pt;
							pt.x = LOWORD(msg.lParam);
							pt.y = HIWORD(msg.lParam);
							msg.pWnd->OnMove(pt);
							break;
						}
					case WM_MOUSEMOVE:
						{
							TRACKMOUSEEVENT tme;
							tme.cbSize = sizeof(tme); //结构体缓冲区大小
							tme.dwFlags = TME_HOVER | TME_LEAVE; //注册WM_MOUSEHOVER消息
							tme.dwHoverTime = 1000; //WM_MOUSEHOVER消息触发间隔时间
							tme.hwndTrack = msg.pWnd->GetMainHWnd(); //当前窗口句柄
							::TrackMouseEvent(&tme); //注册发送消息
						}
					case WM_LBUTTONUP:
					case WM_LBUTTONDOWN:
					case WM_RBUTTONDOWN:
					case WM_RBUTTONUP:
					case WM_LBUTTONDBLCLK:
					case WM_RBUTTONDBLCLK:
					case WM_MOUSEHOVER:
					case WM_MOUSELEAVE:
						{
							POINT pt;
							pt.x = LOWORD(msg.lParam);
							pt.y = HIWORD(msg.lParam);
							ClientToScreen(msg.pWnd->GetMainHWnd(),&pt);
							msg.pWnd->OnMouseEvent(msg.msgid,pt);
							break;
						}
					default:
						break;				
					}

					if (msg.msgid > WM_USER)
						msg.pWnd->OnUserMsg(msg.msgid, msg.wParam, msg.lParam);
				}


			}
			Sleep(1);
		}
		pCoreInfo->msg_list_.clear();
		return 0;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam) 
	{
		LPCoreInfo pCoreInfo = g_msgloops[hWnd];
		if (!pCoreInfo)
			return DefWindowProc(hWnd, message, wParam, lParam);
		switch (message) 
		{
		case WM_NCHITTEST: 
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				if (::GetAsyncKeyState(MK_LBUTTON) < 0)
				{	
					RECT rc;
					pCoreInfo->main_wnd_->GetClientRect(&rc);
					rc.bottom = rc.top + 40;
					rc.right -= 55;
					if (PtInRect(&rc,pt))
					{
						if (GetFocus() == hWnd)
						{
							return HTCAPTION;  
						}	
					}
				}
				break;
			}
		case WM_KILLFOCUS:
				break;
			/* 添加到处理线程中*/
		case WM_MOVE:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEHOVER:
			{
				STLkDuiMsg lkduimsg;
				lkduimsg.msgid = message;
				lkduimsg.pWnd = pCoreInfo->main_wnd_;
				lkduimsg.lParam = lParam;
				lkduimsg.wParam = wParam;
				EnterCriticalSection(&pCoreInfo->msg_lock_);
				pCoreInfo->msg_list_.push_back(lkduimsg);
				LeaveCriticalSection(&pCoreInfo->msg_lock_);
				return 0;
			}
			/* 添加到处理线程中---end*/
		case WM_CREATE: 
			break;
		case WM_COMMAND: 
			break;
		case WM_SETFOCUS:
			break;
		case WM_SIZE:
			break;
		case WM_MOVING:
			break;
		case WM_ERASEBKGND:
			break;
		case WM_ENTERMENULOOP:
			break;
		case WM_EXITMENULOOP:
			break;
		case WM_CLOSE:
			{
				if (pCoreInfo->main_wnd_->OnSysClose())
					PostMessage(hWnd, WM_DESTROY, 0, 0);
				else
					return 0;
				break;
			}
		case WM_DESTROY:
			{
				//pBaseWnd->Destroy();
				EnterCriticalSection(&g_lock_klist);
				g_killingthList.push_back(pCoreInfo);
				LeaveCriticalSection(&g_lock_klist);
				g_msgloops.erase(hWnd);	
				if (g_msgloops.empty())
				   SendMessage(hWnd,WM_QUIT,0,0);
			}
			break;
		}
		if (message > WM_USER)
		{
			STLkDuiMsg lkduimsg;
			lkduimsg.msgid = message;
			lkduimsg.pWnd = pCoreInfo->main_wnd_;
			lkduimsg.lParam = lParam;
			lkduimsg.wParam = wParam;
			EnterCriticalSection(&pCoreInfo->msg_lock_);
			pCoreInfo->msg_list_.push_back(lkduimsg);
			LeaveCriticalSection(&pCoreInfo->msg_lock_);
			return 0;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}