#include "LkDuiPublicFuncs.h"
#include <tchar.h>

namespace lkDui
{

	void MakeDirA(const char* sPath)
	{
		if (!sPath)
			return;
		if (sPath[1] != ':')
			return;
		char  sBuffer[MAX_PATH];
		strcpy(sBuffer,sPath);
		char* p = strchr(sBuffer,'\\');
		while (p != NULL && p[1] != '\0')
		{
			p = strchr(++p,'\\');
			if (p)
			{
				*p = '\0';
				CreateDirectoryA(sBuffer,0);
				*p = '\\';
			}
		}
		return;
	}

	void MakeDirW(const	WCHAR* sPath)
	{
		if (!sPath)
			return;
		if (sPath[1] != ':')
			return;
		WCHAR  sBuffer[MAX_PATH];
		wcscpy(sBuffer,sPath);
		WCHAR* p = wcschr(sBuffer,'\\');
		while (p != NULL && p[1] != '\0')
		{
			p = wcschr(++p,'\\');
			if (p)
			{
				*p = '\0';
				CreateDirectoryW(sBuffer,0);
				*p = '\\';
			}
		}
		return;
	}

	BOOL CreateLink(
		TCHAR* szPath,//快捷方式的目标应用程序名
		TCHAR* szLink,//快捷方式的数据文件名(*.lnk)
		TCHAR* cmdLine,
		TCHAR* szIconPath)
	{

		HRESULT hres;
		IShellLink* psl;
		IPersistFile* ppf;
#ifdef _UNICODE
		MakeDirW(szLink);
#else
		MakeDirA(szLink);
#endif
		////初始化COM
		CoInitialize(NULL);
		//创建一个IShellLink实例
		hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl);
		if(FAILED(hres))
		{
			CoUninitialize();
			return FALSE;
		}
		//设置目标应用程序
		psl->SetPath(szPath);
		//设置快捷键(此处设为Shift+Ctrl+'R')
		// psl -> SetHotkey( MAKEWORD( 'R',
		// HOTKEYF_SHIFT |HOTKEYF_CONTROL)) ;
		//CString csWorkDir(szPath);
		TCHAR tsWorkDir[MAX_PATH];
		_tcscpy(tsWorkDir,szPath);
		TCHAR* p = _tcsrchr(tsWorkDir,'\\');
		if (p)
		{
			*p = '\0';
			psl->SetWorkingDirectory(tsWorkDir);
		}

		if (cmdLine != NULL)
		{
			psl->SetArguments(cmdLine);
		}

		if (szIconPath)
		{
			psl->SetIconLocation(szIconPath,NULL);
		}
		//从IShellLink获取其IPersistFile接口
		//用于保存快捷方式的数据文件 (*.lnk)
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if(FAILED(hres))
		{
			CoUninitialize();
			return FALSE;
		}
		//确保数据文件名为ANSI格式
		//MultiByteToWideChar(CP_ACP, 0, szLink, -1, wsz, MAX_PATH);
		//调用IPersistFile::Save
		//保存快捷方式的数据文件 (*.lnk)
		hres = ppf ->Save(szLink, STGM_READWRITE);
		//释放IPersistFile和IShellLink接口
		ppf -> Release( ) ;
		psl -> Release( ) ;
		CoUninitialize();
		return TRUE;
	}


	//2、创建桌面快捷方式
	void CreateDestopShortcut(TCHAR* szPath, TCHAR* szLinkName,TCHAR* cmdLine,BOOL bCreate)
	{
		//快捷方式的数据文件名

		TCHAR szLink[MAX_PATH] = {0};
		//SHGetSpecialFolderPath(NULL, szLink, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
		SHGetSpecialFolderPath(NULL, szLink, CSIDL_DESKTOPDIRECTORY,FALSE);
		if(szLink[lstrlen(szLink)-1] == '\\')
		{
			lstrcat(szLink, szLinkName);
		}
		else
		{
			lstrcat(szLink, L"\\");
			lstrcat(szLink, szLinkName);
		}
		if (!bCreate)
		{	
			DeleteFile(szLink);
			return;
		}
		//创建快捷方式
		if(!CreateLink(szPath, szLink,cmdLine,NULL))
			return;
	}
	// 开始启动栏
	void CreateProgramsShortCut(TCHAR* szPath, TCHAR* szDir, TCHAR* szLinkName,TCHAR* cmdLine,BOOL bCreate)
	{
		//快捷方式的数据文件名
		TCHAR szLink[MAX_PATH] = {0};

		SHGetSpecialFolderPath(NULL, szLink, CSIDL_PROGRAMS, FALSE);
		if(szLink[lstrlen(szLink)-1] == '\\')
		{
			lstrcat(szLink, szDir);
			lstrcat(szLink, _T("\\"));
			lstrcat(szLink, szLinkName);
		}
		else
		{
			lstrcat(szLink, _T("\\"));
			lstrcat(szLink, szDir);
			lstrcat(szLink, _T("\\"));
			lstrcat(szLink, szLinkName);
		}
		if (!bCreate)
		{	
			DeleteFile(szLink);
			return;
		}
		if(!CreateLink(szPath, szLink,cmdLine,NULL))
			return;
	}

	void CreateStartupShortCut(TCHAR* szPath, TCHAR* szLinkName,TCHAR* cmdLine,BOOL bCreate)
	{
		//快捷方式的数据文件名s

		TCHAR szLink[MAX_PATH] = {0};

		SHGetSpecialFolderPath(NULL, szLink, CSIDL_COMMON_STARTUP, FALSE);
		if(szLink[lstrlen(szLink)-1] == '\\')
		{
			lstrcat(szLink, szLinkName);
		}
		else
		{
			lstrcat(szLink, L"\\");
			lstrcat(szLink, szLinkName);
		}
		if (!bCreate)
		{	
			DeleteFile(szLink);
			return;
		}
		if(!CreateLink(szPath, szLink,cmdLine,NULL))
			return;
	}

}