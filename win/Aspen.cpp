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

#define MAG_TABLE_SIZE		(1<<18)
char* msg_name[MAG_TABLE_SIZE] = { 0 };

const char* msg_xxx = "WM_XXXXXXXXXXXXXXXX";

WinMsg* g_pMSG = NULL;
U16	msg_count;
static DWORD g_tid;

LRESULT message_record(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD tid = GetCurrentThreadId();
	if (tid == g_tid)
	{
		if (msg_count < MAG_TABLE_SIZE)
		{
			WinMsg* pm = g_pMSG + msg_count;
			ATLASSERT(pm);

			pm->count = 1;
			pm->hWnd = hWnd;
			pm->msg = (U16)uMsg;
			pm->wp = wParam;
			pm->lp = lParam;

			if (msg_count > 0)
			{
				WinMsg* pmPrev = g_pMSG + (msg_count - 1);
				if ((pmPrev->msg == pm->msg) && (pmPrev->hWnd == pm->hWnd))
				{
					pmPrev->count++;
					return 0;
				}
			}
			msg_count++;

			if (msg_count >= MAG_TABLE_SIZE)
			{
				msg_count = 0;
				ATLASSERT(0);
			}
		}
	}
	else 
	{
		ATLASSERT(0);
		tid++;
	}
	return 0;
}

char* get_message_name(U16 idx)
{
	return(msg_name[idx]);
}

static void InitWinMessageName(void)
{
	U16 idx;

	msg_count = 0;

	for (int i = 0; i < MAG_TABLE_SIZE; i++)
	{
		msg_name[i] = (char*)msg_xxx;
	}

	idx = (WM_NULL & 0xFFFF);                         msg_name[idx] = (char*)"WM_NULL";
	idx = (WM_CREATE & 0xFFFF);                       msg_name[idx] = (char*)"WM_CREATE";
	idx = (WM_DESTROY & 0xFFFF);                      msg_name[idx] = (char*)"WM_DESTROY";
	idx = (WM_MOVE & 0xFFFF);                         msg_name[idx] = (char*)"WM_MOVE";
	idx = (WM_SIZE & 0xFFFF);                         msg_name[idx] = (char*)"WM_SIZ-E";
	idx = (WM_ACTIVATE & 0xFFFF);                     msg_name[idx] = (char*)"WM_ACTIVATE";
	idx = (WM_SETFOCUS & 0xFFFF);                     msg_name[idx] = (char*)"WM_SETFOCUS";
	idx = (WM_KILLFOCUS & 0xFFFF);                    msg_name[idx] = (char*)"WM_KILLFOCUS";
	idx = (WM_ENABLE & 0xFFFF);                       msg_name[idx] = (char*)"WM_ENABLE";
	idx = (WM_SETREDRAW & 0xFFFF);                    msg_name[idx] = (char*)"WM_SETREDRAW";
	idx = (WM_SETTEXT & 0xFFFF);                      msg_name[idx] = (char*)"WM_SETTEXT";
	idx = (WM_GETTEXT & 0xFFFF);                      msg_name[idx] = (char*)"WM_GETTEXT";
	idx = (WM_GETTEXTLENGTH & 0xFFFF);                msg_name[idx] = (char*)"WM_GETTEXTLENGTH";
	idx = (WM_PAINT & 0xFFFF);                        msg_name[idx] = (char*)"WM_PAINT";
	idx = (WM_CLOSE & 0xFFFF);                        msg_name[idx] = (char*)"WM_CLOSE";
	idx = (WM_QUERYENDSESSION & 0xFFFF);              msg_name[idx] = (char*)"WM_QUERYENDSESSION";
	idx = (WM_QUERYOPEN & 0xFFFF);                    msg_name[idx] = (char*)"WM_QUERYOPEN";
	idx = (WM_ENDSESSION & 0xFFFF);                   msg_name[idx] = (char*)"WM_ENDSESSION";
	idx = (WM_QUIT & 0xFFFF);                         msg_name[idx] = (char*)"WM_QUIT";
	idx = (WM_ERASEBKGND & 0xFFFF);                   msg_name[idx] = (char*)"WM_ERASEBKGND";
	idx = (WM_SYSCOLORCHANGE & 0xFFFF);               msg_name[idx] = (char*)"WM_SYSCOLORCHANGE";
	idx = (WM_SHOWWINDOW & 0xFFFF);                   msg_name[idx] = (char*)"WM_SHOWWINDOW";
	idx = (WM_WININICHANGE & 0xFFFF);                 msg_name[idx] = (char*)"WM_WININICHANGE";
	idx = (WM_SETTINGCHANGE & 0xFFFF);                msg_name[idx] = (char*)"WM_SETTINGCHANGE";
	idx = (WM_DEVMODECHANGE & 0xFFFF);                msg_name[idx] = (char*)"WM_DEVMODECHANGE";
	idx = (WM_ACTIVATEAPP & 0xFFFF);                  msg_name[idx] = (char*)"WM_ACTIVATEAPP";
	idx = (WM_FONTCHANGE & 0xFFFF);                   msg_name[idx] = (char*)"WM_FONTCHANGE";
	idx = (WM_TIMECHANGE & 0xFFFF);                   msg_name[idx] = (char*)"WM_TIMECHANGE";
	idx = (WM_CANCELMODE & 0xFFFF);                   msg_name[idx] = (char*)"WM_CANCELMODE";
	idx = (WM_SETCURSOR & 0xFFFF);                    msg_name[idx] = (char*)"WM_SETCURSOR";
	idx = (WM_MOUSEACTIVATE & 0xFFFF);                msg_name[idx] = (char*)"WM_MOUSEACTIVATE";
	idx = (WM_CHILDACTIVATE & 0xFFFF);                msg_name[idx] = (char*)"WM_CHILDACTIVATE";
	idx = (WM_QUEUESYNC & 0xFFFF);                    msg_name[idx] = (char*)"WM_QUEUESYNC";
	idx = (WM_GETMINMAXINFO & 0xFFFF);                msg_name[idx] = (char*)"WM_GETMINMAXINFO";
	idx = (WM_PAINTICON & 0xFFFF);                    msg_name[idx] = (char*)"WM_PAINTICON";
	idx = (WM_ICONERASEBKGND & 0xFFFF);               msg_name[idx] = (char*)"WM_ICONERASEBKGND";
	idx = (WM_NEXTDLGCTL & 0xFFFF);                   msg_name[idx] = (char*)"WM_NEXTDLGCTL";
	idx = (WM_SPOOLERSTATUS & 0xFFFF);                msg_name[idx] = (char*)"WM_SPOOLERSTATUS";
	idx = (WM_DRAWITEM & 0xFFFF);                     msg_name[idx] = (char*)"WM_DRAWITEM";
	idx = (WM_MEASUREITEM & 0xFFFF);                  msg_name[idx] = (char*)"WM_MEASUREITEM";
	idx = (WM_DELETEITEM & 0xFFFF);                   msg_name[idx] = (char*)"WM_DELETEITEM";
	idx = (WM_VKEYTOITEM & 0xFFFF);                   msg_name[idx] = (char*)"WM_VKEYTOITEM";
	idx = (WM_CHARTOITEM & 0xFFFF);                   msg_name[idx] = (char*)"WM_CHARTOITEM";
	idx = (WM_SETFONT & 0xFFFF);                      msg_name[idx] = (char*)"WM_SETFONT";
	idx = (WM_GETFONT & 0xFFFF);                      msg_name[idx] = (char*)"WM_GETFONT";
	idx = (WM_SETHOTKEY & 0xFFFF);                    msg_name[idx] = (char*)"WM_SETHOTKEY";
	idx = (WM_GETHOTKEY & 0xFFFF);                    msg_name[idx] = (char*)"WM_GETHOTKEY";
	idx = (WM_QUERYDRAGICON & 0xFFFF);                msg_name[idx] = (char*)"WM_QUERYDRAGICON";
	idx = (WM_COMPAREITEM & 0xFFFF);                  msg_name[idx] = (char*)"WM_COMPAREITEM";
	idx = (WM_GETOBJECT & 0xFFFF);                    msg_name[idx] = (char*)"WM_GETOBJECT";
	idx = (WM_COMPACTING & 0xFFFF);                   msg_name[idx] = (char*)"WM_COMPACTING";
	idx = (WM_COMMNOTIFY & 0xFFFF);                   msg_name[idx] = (char*)"WM_COMMNOTIFY";
	idx = (WM_WINDOWPOSCHANGING & 0xFFFF);            msg_name[idx] = (char*)"WM_WINDOWPOSCHANGING";
	idx = (WM_WINDOWPOSCHANGED & 0xFFFF);             msg_name[idx] = (char*)"WM_WINDOWPOSCHANGED";
	idx = (WM_POWER & 0xFFFF);                        msg_name[idx] = (char*)"WM_POWER";
	idx = (WM_COPYDATA & 0xFFFF);                     msg_name[idx] = (char*)"WM_COPYDATA";
	idx = (WM_CANCELJOURNAL & 0xFFFF);                msg_name[idx] = (char*)"WM_CANCELJOURNAL";
	idx = (WM_NOTIFY & 0xFFFF);                       msg_name[idx] = (char*)"WM_NOTIFY";
	idx = (WM_INPUTLANGCHANGEREQUEST & 0xFFFF);       msg_name[idx] = (char*)"WM_INPUTLANGCHANGEREQUEST";
	idx = (WM_INPUTLANGCHANGE & 0xFFFF);              msg_name[idx] = (char*)"WM_INPUTLANGCHANGE";
	idx = (WM_TCARD & 0xFFFF);                        msg_name[idx] = (char*)"WM_TCARD";
	idx = (WM_HELP & 0xFFFF);                         msg_name[idx] = (char*)"WM_HELP";
	idx = (WM_USERCHANGED & 0xFFFF);                  msg_name[idx] = (char*)"WM_USERCHANGED";
	idx = (WM_NOTIFYFORMAT & 0xFFFF);                 msg_name[idx] = (char*)"WM_NOTIFYFORMAT";
	idx = (WM_CONTEXTMENU & 0xFFFF);                  msg_name[idx] = (char*)"WM_CONTEXTMENU";
	idx = (WM_STYLECHANGING & 0xFFFF);                msg_name[idx] = (char*)"WM_STYLECHANGING";
	idx = (WM_STYLECHANGED & 0xFFFF);                 msg_name[idx] = (char*)"WM_STYLECHANGED";
	idx = (WM_DISPLAYCHANGE & 0xFFFF);                msg_name[idx] = (char*)"WM_DISPLAYCHANGE";
	idx = (WM_GETICON & 0xFFFF);                      msg_name[idx] = (char*)"WM_GETICON";
	idx = (WM_SETICON & 0xFFFF);                      msg_name[idx] = (char*)"WM_SETICON";
	idx = (WM_NCCREATE & 0xFFFF);                     msg_name[idx] = (char*)"WM_NCCREATE";
	idx = (WM_NCDESTROY & 0xFFFF);                    msg_name[idx] = (char*)"WM_NCDESTROY";
	idx = (WM_NCCALCSIZE & 0xFFFF);                   msg_name[idx] = (char*)"WM_NCCALCSIZE";
	idx = (WM_NCHITTEST & 0xFFFF);                    msg_name[idx] = (char*)"WM_NCHITTEST";
	idx = (WM_NCPAINT & 0xFFFF);                      msg_name[idx] = (char*)"WM_NCPAINT";
	idx = (WM_NCACTIVATE & 0xFFFF);                   msg_name[idx] = (char*)"WM_NCACTIVATE";
	idx = (WM_GETDLGCODE & 0xFFFF);                   msg_name[idx] = (char*)"WM_GETDLGCODE";
	idx = (WM_SYNCPAINT & 0xFFFF);                    msg_name[idx] = (char*)"WM_SYNCPAINT";
	idx = (WM_NCMOUSEMOVE & 0xFFFF);                  msg_name[idx] = (char*)"WM_NCMOUSEMOVE";
	idx = (WM_NCLBUTTONDOWN & 0xFFFF);                msg_name[idx] = (char*)"WM_NCLBUTTONDOWN";
	idx = (WM_NCLBUTTONUP & 0xFFFF);                  msg_name[idx] = (char*)"WM_NCLBUTTONUP";
	idx = (WM_NCLBUTTONDBLCLK & 0xFFFF);              msg_name[idx] = (char*)"WM_NCLBUTTONDBLCLK";
	idx = (WM_NCRBUTTONDOWN & 0xFFFF);                msg_name[idx] = (char*)"WM_NCRBUTTONDOWN";
	idx = (WM_NCRBUTTONUP & 0xFFFF);                  msg_name[idx] = (char*)"WM_NCRBUTTONUP";
	idx = (WM_NCRBUTTONDBLCLK & 0xFFFF);              msg_name[idx] = (char*)"WM_NCRBUTTONDBLCLK";
	idx = (WM_NCMBUTTONDOWN & 0xFFFF);                msg_name[idx] = (char*)"WM_NCMBUTTONDOWN";
	idx = (WM_NCMBUTTONUP & 0xFFFF);                  msg_name[idx] = (char*)"WM_NCMBUTTONUP";
	idx = (WM_NCMBUTTONDBLCLK & 0xFFFF);              msg_name[idx] = (char*)"WM_NCMBUTTONDBLCLK";
	idx = (WM_NCXBUTTONDOWN & 0xFFFF);                msg_name[idx] = (char*)"WM_NCXBUTTONDOWN";
	idx = (WM_NCXBUTTONUP & 0xFFFF);                  msg_name[idx] = (char*)"WM_NCXBUTTONUP";
	idx = (WM_NCXBUTTONDBLCLK & 0xFFFF);              msg_name[idx] = (char*)"WM_NCXBUTTONDBLCLK";
	idx = (WM_INPUT_DEVICE_CHANGE & 0xFFFF);          msg_name[idx] = (char*)"WM_INPUT_DEVICE_CHANGE";
	idx = (WM_INPUT & 0xFFFF);                        msg_name[idx] = (char*)"WM_INPUT";
	idx = (WM_KEYFIRST & 0xFFFF);                     msg_name[idx] = (char*)"WM_KEYFIRST";
	idx = (WM_KEYDOWN & 0xFFFF);                      msg_name[idx] = (char*)"WM_KEYDOWN";
	idx = (WM_KEYUP & 0xFFFF);                        msg_name[idx] = (char*)"WM_KEYUP";
	idx = (WM_CHAR & 0xFFFF);                         msg_name[idx] = (char*)"WM_CHAR";
	idx = (WM_DEADCHAR & 0xFFFF);                     msg_name[idx] = (char*)"WM_DEADCHAR";
	idx = (WM_SYSKEYDOWN & 0xFFFF);                   msg_name[idx] = (char*)"WM_SYSKEYDOWN";
	idx = (WM_SYSKEYUP & 0xFFFF);                     msg_name[idx] = (char*)"WM_SYSKEYUP";
	idx = (WM_SYSCHAR & 0xFFFF);                      msg_name[idx] = (char*)"WM_SYSCHAR";
	idx = (WM_SYSDEADCHAR & 0xFFFF);                  msg_name[idx] = (char*)"WM_SYSDEADCHAR";
	idx = (WM_UNICHAR & 0xFFFF);                      msg_name[idx] = (char*)"WM_UNICHAR";
	idx = (WM_KEYLAST & 0xFFFF);                      msg_name[idx] = (char*)"WM_KEYLAST";
	idx = (WM_KEYLAST & 0xFFFF);                      msg_name[idx] = (char*)"WM_KEYLAST";
	idx = (WM_IME_STARTCOMPOSITION & 0xFFFF);         msg_name[idx] = (char*)"WM_IME_STARTCOMPOSITION";
	idx = (WM_IME_ENDCOMPOSITION & 0xFFFF);           msg_name[idx] = (char*)"WM_IME_ENDCOMPOSITION";
	idx = (WM_IME_COMPOSITION & 0xFFFF);              msg_name[idx] = (char*)"WM_IME_COMPOSITION";
	idx = (WM_IME_KEYLAST & 0xFFFF);                  msg_name[idx] = (char*)"WM_IME_KEYLAST";
	idx = (WM_INITDIALOG & 0xFFFF);                   msg_name[idx] = (char*)"WM_INITDIALOG";
	idx = (WM_COMMAND & 0xFFFF);                      msg_name[idx] = (char*)"WM_COMMAND";
	idx = (WM_SYSCOMMAND & 0xFFFF);                   msg_name[idx] = (char*)"WM_SYSCOMMAND";
	idx = (WM_TIMER & 0xFFFF);                        msg_name[idx] = (char*)"WM_TIMER";
	idx = (WM_HSCROLL & 0xFFFF);                      msg_name[idx] = (char*)"WM_HSCROLL";
	idx = (WM_VSCROLL & 0xFFFF);                      msg_name[idx] = (char*)"WM_VSCROLL";
	idx = (WM_INITMENU & 0xFFFF);                     msg_name[idx] = (char*)"WM_INITMENU";
	idx = (WM_INITMENUPOPUP & 0xFFFF);                msg_name[idx] = (char*)"WM_INITMENUPOPUP";
	idx = (WM_GESTURE & 0xFFFF);                      msg_name[idx] = (char*)"WM_GESTURE";
	idx = (WM_GESTURENOTIFY & 0xFFFF);                msg_name[idx] = (char*)"WM_GESTURENOTIFY";
	idx = (WM_MENUSELECT & 0xFFFF);                   msg_name[idx] = (char*)"WM_MENUSELECT";
	idx = (WM_MENUCHAR & 0xFFFF);                     msg_name[idx] = (char*)"WM_MENUCHAR";
	idx = (WM_ENTERIDLE & 0xFFFF);                    msg_name[idx] = (char*)"WM_ENTERIDLE";
	idx = (WM_MENURBUTTONUP & 0xFFFF);                msg_name[idx] = (char*)"WM_MENURBUTTONUP";
	idx = (WM_MENUDRAG & 0xFFFF);                     msg_name[idx] = (char*)"WM_MENUDRAG";
	idx = (WM_MENUGETOBJECT & 0xFFFF);                msg_name[idx] = (char*)"WM_MENUGETOBJECT";
	idx = (WM_UNINITMENUPOPUP & 0xFFFF);              msg_name[idx] = (char*)"WM_UNINITMENUPOPUP";
	idx = (WM_MENUCOMMAND & 0xFFFF);                  msg_name[idx] = (char*)"WM_MENUCOMMAND";
	idx = (WM_CHANGEUISTATE & 0xFFFF);                msg_name[idx] = (char*)"WM_CHANGEUISTATE";
	idx = (WM_UPDATEUISTATE & 0xFFFF);                msg_name[idx] = (char*)"WM_UPDATEUISTATE";
	idx = (WM_QUERYUISTATE & 0xFFFF);                 msg_name[idx] = (char*)"WM_QUERYUISTATE";
	idx = (WM_CTLCOLORMSGBOX & 0xFFFF);               msg_name[idx] = (char*)"WM_CTLCOLORMSGBOX";
	idx = (WM_CTLCOLOREDIT & 0xFFFF);                 msg_name[idx] = (char*)"WM_CTLCOLOREDIT";
	idx = (WM_CTLCOLORLISTBOX & 0xFFFF);              msg_name[idx] = (char*)"WM_CTLCOLORLISTBOX";
	idx = (WM_CTLCOLORBTN & 0xFFFF);                  msg_name[idx] = (char*)"WM_CTLCOLORBTN";
	idx = (WM_CTLCOLORDLG & 0xFFFF);                  msg_name[idx] = (char*)"WM_CTLCOLORDLG";
	idx = (WM_CTLCOLORSCROLLBAR & 0xFFFF);            msg_name[idx] = (char*)"WM_CTLCOLORSCROLLBAR";
	idx = (WM_CTLCOLORSTATIC & 0xFFFF);               msg_name[idx] = (char*)"WM_CTLCOLORSTATIC";
	idx = (WM_MOUSEFIRST & 0xFFFF);                   msg_name[idx] = (char*)"WM_MOUSEFIRST";
	idx = (WM_MOUSEMOVE & 0xFFFF);                    msg_name[idx] = (char*)"WM_MOUSEMOVE";
	idx = (WM_LBUTTONDOWN & 0xFFFF);                  msg_name[idx] = (char*)"WM_LBUTTON-DOWN";
	idx = (WM_LBUTTONUP & 0xFFFF);                    msg_name[idx] = (char*)"WM_LBUTTON-UP";
	idx = (WM_LBUTTONDBLCLK & 0xFFFF);                msg_name[idx] = (char*)"WM_LBUTTON-DBLCLK";
	idx = (WM_RBUTTONDOWN & 0xFFFF);                  msg_name[idx] = (char*)"WM_RBUTTON-DOWN";
	idx = (WM_RBUTTONUP & 0xFFFF);                    msg_name[idx] = (char*)"WM_RBUTTON-UP";
	idx = (WM_RBUTTONDBLCLK & 0xFFFF);                msg_name[idx] = (char*)"WM_RBUTTON-DBLCLK";
	idx = (WM_MBUTTONDOWN & 0xFFFF);                  msg_name[idx] = (char*)"WM_MBUTTON-DOWN";
	idx = (WM_MBUTTONUP & 0xFFFF);                    msg_name[idx] = (char*)"WM_MBUTTON-UP";
	idx = (WM_MBUTTONDBLCLK & 0xFFFF);                msg_name[idx] = (char*)"WM_MBUTTON-DBLCLK";
	idx = (WM_MOUSEWHEEL & 0xFFFF);                   msg_name[idx] = (char*)"WM_MOUSEWHEEL";
	idx = (WM_XBUTTONDOWN & 0xFFFF);                  msg_name[idx] = (char*)"WM_XBUTTONDOWN";
	idx = (WM_XBUTTONUP & 0xFFFF);                    msg_name[idx] = (char*)"WM_XBUTTONUP";
	idx = (WM_XBUTTONDBLCLK & 0xFFFF);                msg_name[idx] = (char*)"WM_XBUTTONDBLCLK";
	idx = (WM_MOUSEHWHEEL & 0xFFFF);                  msg_name[idx] = (char*)"WM_MOUSEHWHEEL";
	idx = (WM_MOUSELAST & 0xFFFF);                    msg_name[idx] = (char*)"WM_MOUSELAST";
	idx = (WM_MOUSELAST & 0xFFFF);                    msg_name[idx] = (char*)"WM_MOUSELAST";
	idx = (WM_MOUSELAST & 0xFFFF);                    msg_name[idx] = (char*)"WM_MOUSELAST";
	idx = (WM_MOUSELAST & 0xFFFF);                    msg_name[idx] = (char*)"WM_MOUSELAST";
	idx = (WM_PARENTNOTIFY & 0xFFFF);                 msg_name[idx] = (char*)"WM_PARENTNOTIFY";
	idx = (WM_ENTERMENULOOP & 0xFFFF);                msg_name[idx] = (char*)"WM_ENTERMENULOOP";
	idx = (WM_EXITMENULOOP & 0xFFFF);                 msg_name[idx] = (char*)"WM_EXITMENULOOP";
	idx = (WM_NEXTMENU & 0xFFFF);                     msg_name[idx] = (char*)"WM_NEXTMENU";
	idx = (WM_SIZING & 0xFFFF);                       msg_name[idx] = (char*)"WM_SIZ-ING";
	idx = (WM_CAPTURECHANGED & 0xFFFF);               msg_name[idx] = (char*)"WM_CAPTURECHANGED";
	idx = (WM_MOVING & 0xFFFF);                       msg_name[idx] = (char*)"WM_MOVING";
	idx = (WM_POWERBROADCAST & 0xFFFF);               msg_name[idx] = (char*)"WM_POWERBROADCAST";
	idx = (WM_DEVICECHANGE & 0xFFFF);                 msg_name[idx] = (char*)"WM_DEVICECHANGE";
	idx = (WM_MDICREATE & 0xFFFF);                    msg_name[idx] = (char*)"WM_MDICREATE";
	idx = (WM_MDIDESTROY & 0xFFFF);                   msg_name[idx] = (char*)"WM_MDIDESTROY";
	idx = (WM_MDIACTIVATE & 0xFFFF);                  msg_name[idx] = (char*)"WM_MDIACTIVATE";
	idx = (WM_MDIRESTORE & 0xFFFF);                   msg_name[idx] = (char*)"WM_MDIRESTORE";
	idx = (WM_MDINEXT & 0xFFFF);                      msg_name[idx] = (char*)"WM_MDINEXT";
	idx = (WM_MDIMAXIMIZE & 0xFFFF);                  msg_name[idx] = (char*)"WM_MDIMAXIMIZE";
	idx = (WM_MDITILE & 0xFFFF);                      msg_name[idx] = (char*)"WM_MDITILE";
	idx = (WM_MDICASCADE & 0xFFFF);                   msg_name[idx] = (char*)"WM_MDICASCADE";
	idx = (WM_MDIICONARRANGE & 0xFFFF);               msg_name[idx] = (char*)"WM_MDIICONARRANGE";
	idx = (WM_MDIGETACTIVE & 0xFFFF);                 msg_name[idx] = (char*)"WM_MDIGETACTIVE";
	idx = (WM_MDISETMENU & 0xFFFF);                   msg_name[idx] = (char*)"WM_MDISETMENU";
	idx = (WM_ENTERSIZEMOVE & 0xFFFF);                msg_name[idx] = (char*)"WM_ENTER-SIZEMOVE";
	idx = (WM_EXITSIZEMOVE & 0xFFFF);                 msg_name[idx] = (char*)"WM_EXIT-SIZEMOVE";
	idx = (WM_DROPFILES & 0xFFFF);                    msg_name[idx] = (char*)"WM_DROPFILES";
	idx = (WM_MDIREFRESHMENU & 0xFFFF);               msg_name[idx] = (char*)"WM_MDIREFRESHMENU";
	idx = (WM_POINTERDEVICECHANGE & 0xFFFF);          msg_name[idx] = (char*)"WM_POINTERDEVICECHANGE";
	idx = (WM_POINTERDEVICEINRANGE & 0xFFFF);         msg_name[idx] = (char*)"WM_POINTERDEVICEINRANGE";
	idx = (WM_POINTERDEVICEOUTOFRANGE & 0xFFFF);      msg_name[idx] = (char*)"WM_POINTERDEVICEOUTOFRANGE";
	idx = (WM_TOUCH & 0xFFFF);                        msg_name[idx] = (char*)"WM_TOUCH";
	idx = (WM_NCPOINTERUPDATE & 0xFFFF);              msg_name[idx] = (char*)"WM_NCPOINTERUPDATE";
	idx = (WM_NCPOINTERDOWN & 0xFFFF);                msg_name[idx] = (char*)"WM_NCPOINTERDOWN";
	idx = (WM_NCPOINTERUP & 0xFFFF);                  msg_name[idx] = (char*)"WM_NCPOINTERUP";
	idx = (WM_POINTERUPDATE & 0xFFFF);                msg_name[idx] = (char*)"WM_POINTERUPDATE";
	idx = (WM_POINTERDOWN & 0xFFFF);                  msg_name[idx] = (char*)"WM_POINTERDOWN";
	idx = (WM_POINTERUP & 0xFFFF);                    msg_name[idx] = (char*)"WM_POINTERUP";
	idx = (WM_POINTERENTER & 0xFFFF);                 msg_name[idx] = (char*)"WM_POINTERENTER";
	idx = (WM_POINTERLEAVE & 0xFFFF);                 msg_name[idx] = (char*)"WM_POINTERLEAVE";
	idx = (WM_POINTERACTIVATE & 0xFFFF);              msg_name[idx] = (char*)"WM_POINTERACTIVATE";
	idx = (WM_POINTERCAPTURECHANGED & 0xFFFF);        msg_name[idx] = (char*)"WM_POINTERCAPTURECHANGED";
	idx = (WM_TOUCHHITTESTING & 0xFFFF);              msg_name[idx] = (char*)"WM_TOUCHHITTESTING";
	idx = (WM_POINTERWHEEL & 0xFFFF);                 msg_name[idx] = (char*)"WM_POINTERWHEEL";
	idx = (WM_POINTERHWHEEL & 0xFFFF);                msg_name[idx] = (char*)"WM_POINTERHWHEEL";
	idx = (WM_POINTERROUTEDTO & 0xFFFF);              msg_name[idx] = (char*)"WM_POINTERROUTEDTO";
	idx = (WM_POINTERROUTEDAWAY & 0xFFFF);            msg_name[idx] = (char*)"WM_POINTERROUTEDAWAY";
	idx = (WM_POINTERROUTEDRELEASED & 0xFFFF);        msg_name[idx] = (char*)"WM_POINTERROUTEDRELEASED";
	idx = (WM_IME_SETCONTEXT & 0xFFFF);               msg_name[idx] = (char*)"WM_IME_SETCONTEXT";
	idx = (WM_IME_NOTIFY & 0xFFFF);                   msg_name[idx] = (char*)"WM_IME_NOTIFY";
	idx = (WM_IME_CONTROL & 0xFFFF);                  msg_name[idx] = (char*)"WM_IME_CONTROL";
	idx = (WM_IME_COMPOSITIONFULL & 0xFFFF);          msg_name[idx] = (char*)"WM_IME_COMPOSITIONFULL";
	idx = (WM_IME_SELECT & 0xFFFF);                   msg_name[idx] = (char*)"WM_IME_SELECT";
	idx = (WM_IME_CHAR & 0xFFFF);                     msg_name[idx] = (char*)"WM_IME_CHAR";
	idx = (WM_IME_REQUEST & 0xFFFF);                  msg_name[idx] = (char*)"WM_IME_REQUEST";
	idx = (WM_IME_KEYDOWN & 0xFFFF);                  msg_name[idx] = (char*)"WM_IME_KEYDOWN";
	idx = (WM_IME_KEYUP & 0xFFFF);                    msg_name[idx] = (char*)"WM_IME_KEYUP";
	idx = (WM_MOUSEHOVER & 0xFFFF);                   msg_name[idx] = (char*)"WM_MOUSEHOVER";
	idx = (WM_MOUSELEAVE & 0xFFFF);                   msg_name[idx] = (char*)"WM_MOUSELEAVE";
	idx = (WM_NCMOUSEHOVER & 0xFFFF);                 msg_name[idx] = (char*)"WM_NCMOUSEHOVER";
	idx = (WM_NCMOUSELEAVE & 0xFFFF);                 msg_name[idx] = (char*)"WM_NCMOUSELEAVE";
	idx = (WM_WTSSESSION_CHANGE & 0xFFFF);            msg_name[idx] = (char*)"WM_WTSSESSION_CHANGE";
	idx = (WM_TABLET_FIRST & 0xFFFF);                 msg_name[idx] = (char*)"WM_TABLET_FIRST";
	idx = (WM_TABLET_LAST & 0xFFFF);                  msg_name[idx] = (char*)"WM_TABLET_LAST";
	idx = (WM_DPICHANGED & 0xFFFF);                   msg_name[idx] = (char*)"WM_DPICHANGED";
	idx = (WM_DPICHANGED_BEFOREPARENT & 0xFFFF);      msg_name[idx] = (char*)"WM_DPICHANGED_BEFOREPARENT";
	idx = (WM_DPICHANGED_AFTERPARENT & 0xFFFF);       msg_name[idx] = (char*)"WM_DPICHANGED_AFTERPARENT";
	idx = (WM_GETDPISCALEDSIZE & 0xFFFF);             msg_name[idx] = (char*)"WM_GETDPISCALEDSIZE";
	idx = (WM_CUT & 0xFFFF);                          msg_name[idx] = (char*)"WM_CUT";
	idx = (WM_COPY & 0xFFFF);                         msg_name[idx] = (char*)"WM_COPY";
	idx = (WM_PASTE & 0xFFFF);                        msg_name[idx] = (char*)"WM_PASTE";
	idx = (WM_CLEAR & 0xFFFF);                        msg_name[idx] = (char*)"WM_CLEAR";
	idx = (WM_UNDO & 0xFFFF);                         msg_name[idx] = (char*)"WM_UNDO";
	idx = (WM_RENDERFORMAT & 0xFFFF);                 msg_name[idx] = (char*)"WM_RENDERFORMAT";
	idx = (WM_RENDERALLFORMATS & 0xFFFF);             msg_name[idx] = (char*)"WM_RENDERALLFORMATS";
	idx = (WM_DESTROYCLIPBOARD & 0xFFFF);             msg_name[idx] = (char*)"WM_DESTROYCLIPBOARD";
	idx = (WM_DRAWCLIPBOARD & 0xFFFF);                msg_name[idx] = (char*)"WM_DRAWCLIPBOARD";
	idx = (WM_PAINTCLIPBOARD & 0xFFFF);               msg_name[idx] = (char*)"WM_PAINTCLIPBOARD";
	idx = (WM_VSCROLLCLIPBOARD & 0xFFFF);             msg_name[idx] = (char*)"WM_VSCROLLCLIPBOARD";
	idx = (WM_SIZECLIPBOARD & 0xFFFF);                msg_name[idx] = (char*)"WM_SIZECLIPBOARD";
	idx = (WM_ASKCBFORMATNAME & 0xFFFF);              msg_name[idx] = (char*)"WM_ASKCBFORMATNAME";
	idx = (WM_CHANGECBCHAIN & 0xFFFF);                msg_name[idx] = (char*)"WM_CHANGECBCHAIN";
	idx = (WM_HSCROLLCLIPBOARD & 0xFFFF);             msg_name[idx] = (char*)"WM_HSCROLLCLIPBOARD";
	idx = (WM_QUERYNEWPALETTE & 0xFFFF);              msg_name[idx] = (char*)"WM_QUERYNEWPALETTE";
	idx = (WM_PALETTEISCHANGING & 0xFFFF);            msg_name[idx] = (char*)"WM_PALETTEISCHANGING";
	idx = (WM_PALETTECHANGED & 0xFFFF);               msg_name[idx] = (char*)"WM_PALETTECHANGED";
	idx = (WM_HOTKEY & 0xFFFF);                       msg_name[idx] = (char*)"WM_HOTKEY";
	idx = (WM_PRINT & 0xFFFF);                        msg_name[idx] = (char*)"WM_PRINT";
	idx = (WM_PRINTCLIENT & 0xFFFF);                  msg_name[idx] = (char*)"WM_PRINTCLIENT";
	idx = (WM_APPCOMMAND & 0xFFFF);                   msg_name[idx] = (char*)"WM_APPCOMMAND";
	idx = (WM_THEMECHANGED & 0xFFFF);                 msg_name[idx] = (char*)"WM_THEMECHANGED";
	idx = (WM_CLIPBOARDUPDATE & 0xFFFF);              msg_name[idx] = (char*)"WM_CLIPBOARDUPDATE";
	idx = (WM_DWMCOMPOSITIONCHANGED & 0xFFFF);        msg_name[idx] = (char*)"WM_DWMCOMPOSITIONCHANGED";
	idx = (WM_DWMNCRENDERINGCHANGED & 0xFFFF);        msg_name[idx] = (char*)"WM_DWMNCRENDERINGCHANGED";
	idx = (WM_DWMCOLORIZATIONCOLORCHANGED & 0xFFFF);  msg_name[idx] = (char*)"WM_DWMCOLORIZATIONCOLORCHANGED";
	idx = (WM_DWMWINDOWMAXIMIZEDCHANGE & 0xFFFF);     msg_name[idx] = (char*)"WM_DWMWINDOWMAXIMIZEDCHANGE";
	idx = (WM_GETTITLEBARINFOEX & 0xFFFF);            msg_name[idx] = (char*)"WM_GETTITLEBARINFOEX";
	idx = (WM_HANDHELDFIRST & 0xFFFF);                msg_name[idx] = (char*)"WM_HANDHELDFIRST";
	idx = (WM_HANDHELDLAST & 0xFFFF);                 msg_name[idx] = (char*)"WM_HANDHELDLAST";
	idx = (WM_AFXFIRST & 0xFFFF);                     msg_name[idx] = (char*)"WM_AFXFIRST";
	idx = (WM_AFXLAST & 0xFFFF);                      msg_name[idx] = (char*)"WM_AFXLAST";
	idx = (WM_PENWINFIRST & 0xFFFF);                  msg_name[idx] = (char*)"WM_PENWINFIRST";
	idx = (WM_PENWINLAST & 0xFFFF);                   msg_name[idx] = (char*)"WM_PENWINLAST";
	idx = (WM_APP & 0xFFFF);                          msg_name[idx] = (char*)"WM_APP";
	idx = (WM_USER & 0xFFFF);                         msg_name[idx] = (char*)"WM_USER";
	idx = (WM_DWMSENDICONICTHUMBNAIL & 0xFFFF);       msg_name[idx] = (char*)"WM_DWMSENDICONICTHUMBNAIL";
	idx = (WM_DWMSENDICONICLIVEPREVIEWBITMAP & 0xFFFF); msg_name[idx] = (char*)"WM_DWMSENDICONICLIVEPREVIEWBITMAP";

}

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

	ATLASSERT(g_pMSG == NULL);
	SIZE_T size = sizeof(WinMsg);
	size *= MAG_TABLE_SIZE;

	g_pMSG = (WinMsg*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
	if (g_pMSG == NULL)
		return 1;

	if (Scintilla_RegisterClasses(hInstance) == 0)
		ret = 2;

	InitWinMessageName();

	return ret;
}

static int AppTerm(HINSTANCE hInstance = NULL)
{
	if (g_pMSG != NULL)
	{
		VirtualFree(g_pMSG, 0, MEM_RELEASE);
		g_pMSG = NULL;
	}

	Scintilla_ReleaseResources();
	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpstrCmdLine, _In_ int nCmdShow)
{
	int nRet;

	g_tid = GetCurrentThreadId();

	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// add flags to support other controls
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

#if 0
	if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) 
	{
		OutputDebugStringW(L"WARNING: could not set DPI awareness");
	}
	else
	{
		OutputDebugStringW(L"INFO: could set DPI awareness");
	}
#endif
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

