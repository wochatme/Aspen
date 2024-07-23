// XEdit.cpp : main source file for XEdit.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlscrl.h>

#include "resource.h"
#include "Aspen.h"
#include "ViewMarkDown.h"
#include "ViewEditor.h"
#include "WinDlg.h"
#include "MainFrm.h"

CAppModule _Module;

static int AppRun(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

static int AppInit(HINSTANCE hInstance)
{
	int ret = 0;

	if (Scintilla_RegisterClasses(hInstance) == 0)
		ret = 1;
	return ret;
}

static int AppTerm(HINSTANCE hInstance = NULL)
{
	Scintilla_ReleaseResources();
	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpstrCmdLine, _In_ int nCmdShow)
{
	int nRet;
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// add flags to support other controls
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	nRet = AppInit(hInstance);
	if (nRet == 0)
	{
		AppRun(lpstrCmdLine, nCmdShow);
	}

	AppTerm();
	_Module.Term();
	::CoUninitialize();

	return nRet;
}

