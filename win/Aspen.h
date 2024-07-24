#pragma once

#define S8      int8_t
#define S16     int16_t
#define S32     int32_t
#define S64     int64_t

#define U8      uint8_t
#define U16     uint16_t
#define U32     uint32_t
#define U64     uint64_t

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

typedef struct WinMsg
{
	HWND hWnd;
	U16	 msg;
	U64	 wp;
	U64  lp;
	U32  count;
} WinMsg;

#define WM_DISPLAY_MSG		(WM_APP + 1)
#define WM_IGNORE_CLIP		(WM_APP + 2)
#define WM_FULLSCR_ON_MAX	(WM_APP + 3)
#define WM_GOT_CLIPDATA		(WM_APP + 4)

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A           /* not defined in earlier SDKs */
#endif 
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E          /* not defined in earlier SDKs */
#endif 
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif 
#define WM_DPICHANGED_BEFOREPARENT	0x02E2
#define WM_DPICHANGED_AFTERPARENT	0x02E3
#define WM_GETDPISCALEDSIZE			0x02E4

extern WinMsg* g_pMSG;
extern U16	msg_count;

LRESULT message_record(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
char* get_message_name(U16 idx);

#define MESSAGE_RECORD(func) \
	{ \
		lResult = func(m_hWnd, uMsg, wParam, lParam); \
	}

// These are defined in <windowsx.h> but we don't want to pull in whole header
// And we need them because coordinates are signed when you have multi-monitor setup.
#ifndef GET_X_PARAM
#define GET_X_PARAM(lp) ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_PARAM
#define GET_Y_PARAM(lp) ((int)(short)HIWORD(lp))
#endif
