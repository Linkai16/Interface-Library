#pragma once

#include "LkDuiBase.h"
#include <map>
#include <list>

#define TRACE

namespace lkDui
{
	typedef  struct _tag_LkDuiMsg 
	{
		CLkDuiBase* pWnd;
		UINT		msgid;
		LPARAM		lParam;
		WPARAM      wParam;
	}STLkDuiMsg,*LPlkDuiMsg;
	typedef std::list<STLkDuiMsg> MSG_LIST;

	typedef struct _tag_CoreInfo 
	{
		CLkDuiBase*			main_wnd_;
		CRITICAL_SECTION	msg_lock_;
		BOOL				msg_thread_;			// 消息处理线程是否运行
		HANDLE				msg_handle_;			// 消息处理线程句柄
		MSG_LIST			msg_list_;				// 消息列表
	}CoreInfo,*LPCoreInfo;
	typedef std::map<HWND,LPCoreInfo> MSG_LOOP_SET;

	extern const TCHAR class_name_[];
	extern int			 g_colBit;


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,	LPARAM lParam);
	DWORD   WINAPI   WndProcDeal(LPVOID lpMsg);
	DWORD   WINAPI   thExitWnd(LPVOID lpMsg);
	int  RunMessageLoop(HWND);
	ATOM RegisterMainClass(HINSTANCE hInstance,DWORD dwIcon);
	void LkDuiStartup();
	void LkDuiShutdown();
	void RegeditMsgloop(CLkDuiBase*);
}