#ifndef LKDUI_PUBLIC_FUNCS
#define LKDUI_PUBLIC_FUNCS
#include "LkDuiDef.h"
//include 获取系统目录
#include   <shlobj.h>   
#pragma   comment(lib,   "shell32.lib")  
//inlcude end

// include  文件是否存在
#include "Shlwapi.h"
#pragma comment(lib,"Shlwapi.lib") /*需要加上此行才可以正确link，VC6.0*/
// include end


namespace lkDui
{
	BOOL CreateLink(
		TCHAR* szPath,//快捷方式的目标应用程序名
		TCHAR* szLink,//快捷方式的数据文件名(*.lnk)
		TCHAR* cmdLine,
		TCHAR* szIconPath);
	void CreateDestopShortcut(TCHAR* szPath, TCHAR* szLinkName,TCHAR* cmdLine,BOOL bCreate = TRUE);
	void CreateStartupShortCut(TCHAR* szPath, TCHAR* szLinkName,TCHAR* cmdLine,BOOL bCreate = TRUE);
	void CreateProgramsShortCut(TCHAR* szPath, TCHAR* szDir, TCHAR* szLinkName,TCHAR* cmdLine,BOOL bCreate = TRUE);
}


#endif