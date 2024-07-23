// View.h : interface of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#if 0
class CView : public CScrollWindowImpl<CView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	void DoPaint(CDCHandle dc)
	{
		//TODO: Add your drawing code here
	}

	BEGIN_MSG_MAP(CView)
		CHAIN_MSG_MAP(CScrollWindowImpl<CView>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};
#endif 

#define DECLARE_XWND_CLASS(WndClassName) \
static ATL::CWndClassInfo& GetWndClassInfo() \
{ \
	static ATL::CWndClassInfo wc = \
	{ \
		{ sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW /*| CS_DBLCLKS*/, StartWindowProc, \
		  0, 0, NULL, NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1), NULL, WndClassName, NULL }, \
		NULL, NULL, IDC_IBEAM, TRUE, 0, _T("") \
	}; \
	return wc; \
}

class CMDView : public CWindowImpl<CMDView>
{
public:
	DECLARE_XWND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return FALSE;
	}

	BEGIN_MSG_MAP(CMDView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		PAINTSTRUCT ps;
		RECT rc = { 0 };

		HDC hdc = BeginPaint(&ps);
		GetClientRect(&rc);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(0, 0, 0)); 
		::FillRect(hdc, &rc, hBrush);
		::DeleteObject(hBrush);

		EndPaint(&ps);
		return 0;
	}

};

