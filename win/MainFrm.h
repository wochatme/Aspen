// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#if 0
// Adopted from:
// https://github.com/grassator/win32-window-custom-titlebar
static int win32_dpi_scale(int value, UINT dpi) 
{
	return (int)((float)value * dpi / 96);
}

// Adopted from:
// https://github.com/oberth/custom-chrome/blob/master/source/gui/window_helper.hpp#L52-L64
static RECT win32_titlebar_rect(HWND hWnd) 
{
	SIZE title_bar_size = { 0 };
	const int top_and_bottom_borders = 2;

	HTHEME theme = OpenThemeData(hWnd, L"WINDOW");
	
	UINT dpi = GetDpiForWindow(hWnd);
	
	GetThemePartSize(theme, NULL, WP_CAPTION, CS_ACTIVE, NULL, TS_TRUE, &title_bar_size);
	CloseThemeData(theme);

	int height = win32_dpi_scale(title_bar_size.cy, dpi) + top_and_bottom_borders;

	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom = rect.top + height;
	return rect;
}

// Set this to 0 to remove the fake shadow painting
#define WIN32_FAKE_SHADOW_HEIGHT			1
// The offset of the 2 rectangles of the maximized window button
#define WIN32_MAXIMIZED_RECTANGLE_OFFSET	2

static RECT win32_fake_shadow_rect(HWND hWnd) 
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom = rect.top + WIN32_FAKE_SHADOW_HEIGHT;
	return rect;
}

typedef struct 
{
	RECT close;
	RECT maximize;
	RECT minimize;
	RECT setting;
} CustomTitleBarButtonRects;

typedef enum 
{
	CustomTitleBarHoveredButton_None,
	CustomTitleBarHoveredButton_Setting,
	CustomTitleBarHoveredButton_Minimize,
	CustomTitleBarHoveredButton_Maximize,
	CustomTitleBarHoveredButton_Close,
} CustomTitleBarHoveredButton;

static CustomTitleBarButtonRects win32_get_title_bar_button_rects(HWND hWnd,const RECT* title_bar_rect) 
{
	UINT dpi = GetDpiForWindow(hWnd);
	CustomTitleBarButtonRects button_rects;
	// Sadly SM_CXSIZE does not result in the right size buttons for Win10
	int button_width = win32_dpi_scale(47, dpi);
	button_rects.close = *title_bar_rect;
	button_rects.close.top += WIN32_FAKE_SHADOW_HEIGHT;

	button_rects.close.left = button_rects.close.right - button_width;
	button_rects.maximize = button_rects.close;
	button_rects.maximize.left -= button_width;
	button_rects.maximize.right -= button_width;
	button_rects.minimize = button_rects.maximize;
	button_rects.minimize.left -= button_width;
	button_rects.minimize.right -= button_width;
	button_rects.setting = button_rects.minimize;
	button_rects.setting.left -= button_width;
	button_rects.setting.right -= button_width;
	return button_rects;
}

static bool win32_window_is_maximized(HWND hWnd) 
{
	WINDOWPLACEMENT placement = { 0 };
	placement.length = sizeof(WINDOWPLACEMENT);
	if (GetWindowPlacement(hWnd, &placement)) 
	{
		return (placement.showCmd == SW_SHOWMAXIMIZED);
	}
	return false;
}

static void win32_center_rect_in_rect(RECT* to_center, const RECT* outer_rect) 
{
	int to_width = to_center->right - to_center->left;
	int to_height = to_center->bottom - to_center->top;
	int outer_width = outer_rect->right - outer_rect->left;
	int outer_height = outer_rect->bottom - outer_rect->top;

	int padding_x = (outer_width - to_width) / 2;
	int padding_y = (outer_height - to_height) / 2;

	to_center->left = outer_rect->left + padding_x;
	to_center->top = outer_rect->top + padding_y;
	to_center->right = to_center->left + to_width;
	to_center->bottom = to_center->top + to_height;
}
#endif

#define IDM_REFRESH_MESSAGE		0x200
// Splitter panes constants
#define SPLIT_PANE_LEFT			 0
#define SPLIT_PANE_RIGHT		 1
#define SPLIT_PANE_TOP			 SPLIT_PANE_LEFT
#define SPLIT_PANE_BOTTOM		 SPLIT_PANE_RIGHT
#define SPLIT_PANE_NONE			-1

// Splitter extended styles
#define SPLIT_PROPORTIONAL		0x00000001
#define SPLIT_NONINTERACTIVE	0x00000002
#define SPLIT_RIGHTALIGNED		0x00000004
#define SPLIT_BOTTOMALIGNED		SPLIT_RIGHTALIGNED
#define SPLIT_GRADIENTBAR		0x00000008
#define SPLIT_FLATBAR			0x00000020
#define SPLIT_FIXEDBARSIZE		0x00000010

// Note: SPLIT_PROPORTIONAL and SPLIT_RIGHTALIGNED/SPLIT_BOTTOMALIGNED are 
// mutually exclusive. If both are set, splitter defaults to SPLIT_PROPORTIONAL.
// Also, SPLIT_FLATBAR overrides SPLIT_GRADIENTBAR if both are set.

class CMainFrame :
	public CFrameWindowImpl<CMainFrame>,
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	enum { m_nPanesCount = 2, m_nPropMax = INT_MAX, m_cxyStep = 10 };

	bool m_bVertical;
	HWND m_hWndPane[m_nPanesCount];
	RECT m_rcSplitter;
	int m_xySplitterPos;            // splitter bar position
	int m_xySplitterPosNew;         // internal - new position while moving
	HWND m_hWndFocusSave;
	int m_nDefActivePane;
	int m_cxySplitBar;              // splitter bar width/height
	HCURSOR m_hCursor;
	int m_cxyMin;                   // minimum pane size
	int m_cxyBarEdge;              	// splitter bar edge
	bool m_bFullDrag;
	int m_cxyDragOffset;		// internal
	int m_nProportionalPos;
	bool m_bUpdateProportionalPos;
	DWORD m_dwExtendedStyle;        // splitter specific extended styles
	int m_nSinglePane;              // single pane mode
	int m_xySplitterDefPos;         // default position
	bool m_bProportionalDefPos;     // porportinal def pos

	// Constructor
	CMainFrame(bool bVertical = true) :
		m_bVertical(bVertical), m_xySplitterPos(-1), m_xySplitterPosNew(-1), m_hWndFocusSave(NULL),
		m_nDefActivePane(SPLIT_PANE_NONE), m_cxySplitBar(4), m_hCursor(NULL), m_cxyMin(0), m_cxyBarEdge(0),
		m_bFullDrag(true), m_cxyDragOffset(0), m_nProportionalPos(0), m_bUpdateProportionalPos(true),
		m_dwExtendedStyle(SPLIT_PROPORTIONAL), m_nSinglePane(SPLIT_PANE_NONE),
		m_xySplitterDefPos(-1), m_bProportionalDefPos(false)
	{
		m_hWndPane[SPLIT_PANE_LEFT] = NULL;
		m_hWndPane[SPLIT_PANE_RIGHT] = NULL;

		::SetRectEmpty(&m_rcSplitter);
	}

public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CMDView   m_viewMD;
	CEditView m_viewED;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return FALSE; // m_viewMD.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_RECORD(message_record)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
		if (IsInteractive())
		{
			MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
			MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
			MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
			MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
			MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDoubleClick)
			MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
			MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		}
		MESSAGE_HANDLER(WM_SIZE, OnSize)
#if 0
		MESSAGE_HANDLER(WM_NCCALCSIZE, OnNCCalcSize)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)
		MESSAGE_HANDLER(WM_NCMOUSEMOVE, OnNCMouseMove)
		MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNCLButtonDown)
		MESSAGE_HANDLER(WM_NCLBUTTONUP, OnNCLButtonUp)
#endif 
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
#if 0
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
#endif 
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		WPARAM wp = wParam & ~0xF;
		switch (wp)
		{
		case IDM_REFRESH_MESSAGE:
			if (msg_count >= 0)
			{
#if 10
				RECT wr = {};
				RECT cr = {};

				m_viewMD.GetWindowRect(&wr);
				m_viewMD.GetClientRect(&cr);
				m_viewED.ShowRect(&wr, &cr);
#endif 
				m_viewED.AppendText(g_pMSG, msg_count);
				msg_count = 0;
			}
			break;
		default:
			bHandled = FALSE;
			break;
		}
		return 0;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
#if 0
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();

		m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);
		// replace with appropriate values for the app
		m_view.SetScrollSize(2000, 1000);

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);
#endif 
		Init();
#if 10
		m_viewMD.Create(m_hWnd, rcDefault, NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL);
		m_viewED.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		ATLASSERT(m_viewMD.IsWindow());
		ATLASSERT(m_viewED.IsWindow());

		SetSplitterPanes(m_viewED, m_viewMD);
		SetSplitterPosPct(50);
#endif
		HMENU m = GetSystemMenu(FALSE);
		if (m)
		{
			AppendMenu(m, MF_SEPARATOR, 0, 0);
			AppendMenu(m, MF_ENABLED, IDM_REFRESH_MESSAGE, L"Refresh");
		}

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		// Inform the application of the frame change to force redrawing with the new
		// client area that is extended into the title bar
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

#if 0
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
#endif 
	// Attributes
	void SetSplitterRect(LPRECT lpRect = NULL, bool bUpdate = true)
	{
		if (lpRect == NULL)
		{
			GetClientRect(&m_rcSplitter);
		}
		else
		{
			m_rcSplitter = *lpRect;
		}

		if (IsProportional())
			UpdateProportionalPos();
		else if (IsRightAligned())
			UpdateRightAlignPos();

		if (bUpdate)
			UpdateSplitterLayout();
	}

	void GetSplitterRect(LPRECT lpRect) const
	{
		ATLASSERT(lpRect != NULL);
		*lpRect = m_rcSplitter;
	}

	bool SetSplitterPos(int xyPos = -1, bool bUpdate = true)
	{
		if (xyPos == -1)   // -1 == default position
		{
			if (m_bProportionalDefPos)
			{
				ATLASSERT((m_xySplitterDefPos >= 0) && (m_xySplitterDefPos <= m_nPropMax));

				if (m_bVertical)
					xyPos = ::MulDiv(m_xySplitterDefPos, m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge, m_nPropMax);
				else
					xyPos = ::MulDiv(m_xySplitterDefPos, m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge, m_nPropMax);
			}
			else if (m_xySplitterDefPos != -1)
			{
				xyPos = m_xySplitterDefPos;
			}
			else   // not set, use middle position
			{
				if (m_bVertical)
					xyPos = (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) / 2;
				else
					xyPos = (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge) / 2;
			}
		}

		// Adjust if out of valid range
		int cxyMax = 0;
		if (m_bVertical)
			cxyMax = m_rcSplitter.right - m_rcSplitter.left;
		else
			cxyMax = m_rcSplitter.bottom - m_rcSplitter.top;

		if (xyPos < m_cxyMin + m_cxyBarEdge)
			xyPos = m_cxyMin;
		else if (xyPos > (cxyMax - m_cxySplitBar - m_cxyBarEdge - m_cxyMin))
			xyPos = cxyMax - m_cxySplitBar - m_cxyBarEdge - m_cxyMin;

		// Set new position and update if requested
		bool bRet = (m_xySplitterPos != xyPos);
		m_xySplitterPos = xyPos;

		if (m_bUpdateProportionalPos)
		{
			if (IsProportional())
				StoreProportionalPos();
			else if (IsRightAligned())
				StoreRightAlignPos();
		}
		else
		{
			m_bUpdateProportionalPos = true;
		}

		if (bUpdate && bRet)
			UpdateSplitterLayout();

		return bRet;
	}

	int GetSplitterPos() const
	{
		return m_xySplitterPos;
	}

	void SetSplitterPosPct(int nPct, bool bUpdate = true)
	{
		ATLASSERT((nPct >= 0) && (nPct <= 100));

		m_nProportionalPos = ::MulDiv(nPct, m_nPropMax, 100);
		UpdateProportionalPos();

		if (bUpdate)
			UpdateSplitterLayout();
	}

	int GetSplitterPosPct() const
	{
		int cxyTotal = m_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		return ((cxyTotal > 0) && (m_xySplitterPos >= 0)) ? ::MulDiv(m_xySplitterPos, 100, cxyTotal) : -1;
	}

	bool SetSinglePaneMode(int nPane = SPLIT_PANE_NONE)
	{
		ATLASSERT((nPane == SPLIT_PANE_LEFT) || (nPane == SPLIT_PANE_RIGHT) || (nPane == SPLIT_PANE_NONE));
		if (!((nPane == SPLIT_PANE_LEFT) || (nPane == SPLIT_PANE_RIGHT) || (nPane == SPLIT_PANE_NONE)))
			return false;

		if (nPane != SPLIT_PANE_NONE)
		{
			if (::IsWindowVisible(m_hWndPane[nPane]) == FALSE)
				::ShowWindow(m_hWndPane[nPane], SW_SHOW);
			int nOtherPane = (nPane == SPLIT_PANE_LEFT) ? SPLIT_PANE_RIGHT : SPLIT_PANE_LEFT;
			::ShowWindow(m_hWndPane[nOtherPane], SW_HIDE);
			if (m_nDefActivePane != nPane)
				m_nDefActivePane = nPane;
		}
		else if (m_nSinglePane != SPLIT_PANE_NONE)
		{
			int nOtherPane = (m_nSinglePane == SPLIT_PANE_LEFT) ? SPLIT_PANE_RIGHT : SPLIT_PANE_LEFT;
			::ShowWindow(m_hWndPane[nOtherPane], SW_SHOW);
		}

		m_nSinglePane = nPane;
		UpdateSplitterLayout();

		return true;
	}

	int GetSinglePaneMode() const
	{
		return m_nSinglePane;
	}

	DWORD GetSplitterExtendedStyle() const
	{
		return m_dwExtendedStyle;
	}

	DWORD SetSplitterExtendedStyle(DWORD dwExtendedStyle, DWORD dwMask = 0)
	{
		DWORD dwPrevStyle = m_dwExtendedStyle;
		if (dwMask == 0)
			m_dwExtendedStyle = dwExtendedStyle;
		else
			m_dwExtendedStyle = (m_dwExtendedStyle & ~dwMask) | (dwExtendedStyle & dwMask);

#ifdef _DEBUG
		if (IsProportional() && IsRightAligned())
			ATLTRACE2(atlTraceUI, 0, _T("CSplitterImpl::SetSplitterExtendedStyle - SPLIT_PROPORTIONAL and SPLIT_RIGHTALIGNED are mutually exclusive, defaulting to SPLIT_PROPORTIONAL.\n"));
#endif // _DEBUG

		return dwPrevStyle;
	}

	void SetSplitterDefaultPos(int xyPos = -1)
	{
		m_xySplitterDefPos = xyPos;
		m_bProportionalDefPos = false;
	}

	void SetSplitterDefaultPosPct(int nPct)
	{
		ATLASSERT((nPct >= 0) && (nPct <= 100));

		m_xySplitterDefPos = ::MulDiv(nPct, m_nPropMax, 100);
		m_bProportionalDefPos = true;
	}

	// Splitter operations
	void SetSplitterPanes(HWND hWndLeftTop, HWND hWndRightBottom, bool bUpdate = true)
	{
		m_hWndPane[SPLIT_PANE_LEFT] = hWndLeftTop;
		m_hWndPane[SPLIT_PANE_RIGHT] = hWndRightBottom;
		ATLASSERT((m_hWndPane[SPLIT_PANE_LEFT] == NULL) || (m_hWndPane[SPLIT_PANE_RIGHT] == NULL) || (m_hWndPane[SPLIT_PANE_LEFT] != m_hWndPane[SPLIT_PANE_RIGHT]));
		if (bUpdate)
			UpdateSplitterLayout();
	}

	bool SetSplitterPane(int nPane, HWND hWnd, bool bUpdate = true)
	{
		ATLASSERT((nPane == SPLIT_PANE_LEFT) || (nPane == SPLIT_PANE_RIGHT));
		if ((nPane != SPLIT_PANE_LEFT) && (nPane != SPLIT_PANE_RIGHT))
			return false;

		m_hWndPane[nPane] = hWnd;
		ATLASSERT((m_hWndPane[SPLIT_PANE_LEFT] == NULL) || (m_hWndPane[SPLIT_PANE_RIGHT] == NULL) || (m_hWndPane[SPLIT_PANE_LEFT] != m_hWndPane[SPLIT_PANE_RIGHT]));
		if (bUpdate)
			UpdateSplitterLayout();

		return true;
	}

	HWND GetSplitterPane(int nPane) const
	{
		ATLASSERT((nPane == SPLIT_PANE_LEFT) || (nPane == SPLIT_PANE_RIGHT));
		if ((nPane != SPLIT_PANE_LEFT) && (nPane != SPLIT_PANE_RIGHT))
			return NULL;

		return m_hWndPane[nPane];
	}

	bool SetActivePane(int nPane)
	{
		ATLASSERT((nPane == SPLIT_PANE_LEFT) || (nPane == SPLIT_PANE_RIGHT));
		if ((nPane != SPLIT_PANE_LEFT) && (nPane != SPLIT_PANE_RIGHT))
			return false;
		if ((m_nSinglePane != SPLIT_PANE_NONE) && (nPane != m_nSinglePane))
			return false;

		::SetFocus(m_hWndPane[nPane]);
		m_nDefActivePane = nPane;

		return true;
	}

	int GetActivePane() const
	{
		int nRet = SPLIT_PANE_NONE;
		HWND hWndFocus = ::GetFocus();
		if (hWndFocus != NULL)
		{
			for (int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if ((hWndFocus == m_hWndPane[nPane]) || (::IsChild(m_hWndPane[nPane], hWndFocus) != FALSE))
				{
					nRet = nPane;
					break;
				}
			}
		}

		return nRet;
	}

	bool ActivateNextPane(bool bNext = true)
	{
		int nPane = m_nSinglePane;
		if (nPane == SPLIT_PANE_NONE)
		{
			switch (GetActivePane())
			{
			case SPLIT_PANE_LEFT:
				nPane = SPLIT_PANE_RIGHT;
				break;
			case SPLIT_PANE_RIGHT:
				nPane = SPLIT_PANE_LEFT;
				break;
			default:
				nPane = bNext ? SPLIT_PANE_LEFT : SPLIT_PANE_RIGHT;
				break;
			}
		}

		return SetActivePane(nPane);
	}

	bool SetDefaultActivePane(int nPane)
	{
		ATLASSERT((nPane == SPLIT_PANE_LEFT) || (nPane == SPLIT_PANE_RIGHT));
		if ((nPane != SPLIT_PANE_LEFT) && (nPane != SPLIT_PANE_RIGHT))
			return false;

		m_nDefActivePane = nPane;

		return true;
	}

	bool SetDefaultActivePane(HWND hWnd)
	{
		for (int nPane = 0; nPane < m_nPanesCount; nPane++)
		{
			if (hWnd == m_hWndPane[nPane])
			{
				m_nDefActivePane = nPane;
				return true;
			}
		}

		return false;   // not found
	}

	int GetDefaultActivePane() const
	{
		return m_nDefActivePane;
	}

	void DrawSplitter(CDCHandle dc)
	{
		ATLASSERT(dc.m_hDC != NULL);
		if ((m_nSinglePane == SPLIT_PANE_NONE) && (m_xySplitterPos == -1))
			return;

		
		if (m_nSinglePane == SPLIT_PANE_NONE)
		{
			DrawSplitterBar(dc);

			for (int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if (m_hWndPane[nPane] == NULL)
					DrawSplitterPane(dc, nPane);
			}
		}
		else
		{
			if (m_hWndPane[m_nSinglePane] == NULL)
				DrawSplitterPane(dc, m_nSinglePane);
		}
	}

	// call to initiate moving splitter bar with keyboard
	void MoveSplitterBar()
	{
		

		int x = 0;
		int y = 0;
		if (m_bVertical)
		{
			x = m_xySplitterPos + (m_cxySplitBar / 2) + m_cxyBarEdge;
			y = (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge) / 2;
		}
		else
		{
			x = (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) / 2;
			y = m_xySplitterPos + (m_cxySplitBar / 2) + m_cxyBarEdge;
		}

		POINT pt = { x, y };
		ClientToScreen(&pt);
		::SetCursorPos(pt.x, pt.y);

		m_xySplitterPosNew = m_xySplitterPos;
		SetCapture();
		m_hWndFocusSave = SetFocus();
		::SetCursor(m_hCursor);
		if (!m_bFullDrag)
			DrawGhostBar();
		if (m_bVertical)
			m_cxyDragOffset = x - m_rcSplitter.left - m_xySplitterPos;
		else
			m_cxyDragOffset = y - m_rcSplitter.top - m_xySplitterPos;
	}

	void SetOrientation(bool bVertical, bool bUpdate = true)
	{
		if (m_bVertical != bVertical)
		{
			m_bVertical = bVertical;

			m_hCursor = ::LoadCursor(NULL, m_bVertical ? IDC_SIZEWE : IDC_SIZENS);

			
			GetSystemSettings(false);

			if (m_bVertical)
				m_xySplitterPos = ::MulDiv(m_xySplitterPos, m_rcSplitter.right - m_rcSplitter.left, m_rcSplitter.bottom - m_rcSplitter.top);
			else
				m_xySplitterPos = ::MulDiv(m_xySplitterPos, m_rcSplitter.bottom - m_rcSplitter.top, m_rcSplitter.right - m_rcSplitter.left);
		}

		if (bUpdate)
			UpdateSplitterLayout();
	}

	// Overrideables
	void DrawSplitterBar(CDCHandle dc)
	{
		RECT rect = {};
		if (GetSplitterBarRect(&rect))
		{
			dc.FillRect(&rect, COLOR_3DFACE);

			if ((m_dwExtendedStyle & SPLIT_FLATBAR) != 0)
			{
				RECT rect1 = rect;
				if (m_bVertical)
					rect1.right = rect1.left + 1;
				else
					rect1.bottom = rect1.top + 1;
				dc.FillRect(&rect1, COLOR_WINDOW);

				rect1 = rect;
				if (m_bVertical)
					rect1.left = rect1.right - 1;
				else
					rect1.top = rect1.bottom - 1;
				dc.FillRect(&rect1, COLOR_3DSHADOW);
			}
			else if ((m_dwExtendedStyle & SPLIT_GRADIENTBAR) != 0)
			{
				RECT rect2 = rect;
				if (m_bVertical)
					rect2.left = (rect.left + rect.right) / 2 - 1;
				else
					rect2.top = (rect.top + rect.bottom) / 2 - 1;

				dc.GradientFillRect(rect2, ::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_3DSHADOW), m_bVertical);
			}

			// draw 3D edge if needed
			
			if ((GetExStyle() & WS_EX_CLIENTEDGE) != 0)
				dc.DrawEdge(&rect, EDGE_RAISED, m_bVertical ? (BF_LEFT | BF_RIGHT) : (BF_TOP | BF_BOTTOM));
		}
	}

	// called only if pane is empty
	void DrawSplitterPane(CDCHandle dc, int nPane)
	{
		RECT rect = {};
		if (GetSplitterPaneRect(nPane, &rect))
		{
			
			if ((GetExStyle() & WS_EX_CLIENTEDGE) == 0)
				dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT | BF_ADJUST);
			dc.FillRect(&rect, COLOR_APPWORKSPACE);
		}
	}
#if 0
	void DrawCustomTitle(HDC hdc, RECT* area)
	{
		bool has_focus = !!GetFocus();

		CustomTitleBarHoveredButton title_bar_hovered_button
			 = (CustomTitleBarHoveredButton)GetWindowLongPtrW(GWLP_USERDATA);
		// Paint Background
		COLORREF bg_color = RGB(200, 250, 230);
		HBRUSH bg_brush = CreateSolidBrush(bg_color);
		FillRect(hdc, area, bg_brush);
		DeleteObject(bg_brush);

		// Paint Title Bar
		HTHEME theme = OpenThemeData(m_hWnd, L"WINDOW");

		COLORREF title_bar_color = RGB(150, 200, 180);
		HBRUSH title_bar_brush = CreateSolidBrush(title_bar_color);
		COLORREF title_bar_hover_color = RGB(130, 180, 160);
		HBRUSH title_bar_hover_brush = CreateSolidBrush(title_bar_hover_color);

		RECT title_bar_rect = win32_titlebar_rect(m_hWnd);

		// Title Bar Background
		FillRect(hdc, &title_bar_rect, title_bar_brush);

		COLORREF title_bar_item_color = has_focus ? RGB(33, 33, 33) : RGB(127, 127, 127);

		HBRUSH button_icon_brush = CreateSolidBrush(title_bar_item_color);
		HPEN button_icon_pen = CreatePen(PS_SOLID, 1, title_bar_item_color);

		CustomTitleBarButtonRects button_rects =
			win32_get_title_bar_button_rects(m_hWnd, &title_bar_rect);

		UINT dpi = GetDpiForWindow(m_hWnd);
		int icon_dimension = win32_dpi_scale(10, dpi);

		{ // Minimize button
			if (title_bar_hovered_button == CustomTitleBarHoveredButton_Minimize) 
			{
				FillRect(hdc, &button_rects.minimize, title_bar_hover_brush);
			}
			RECT icon_rect = { 0 };
			icon_rect.right = icon_dimension;
			icon_rect.bottom = 1;
			win32_center_rect_in_rect(&icon_rect, &button_rects.minimize);
			FillRect(hdc, &icon_rect, button_icon_brush);
		}

		{ // Maximize button
			bool const is_hovered = title_bar_hovered_button == CustomTitleBarHoveredButton_Maximize;
			if (is_hovered) {
				FillRect(hdc, &button_rects.maximize, title_bar_hover_brush);
			}
			RECT icon_rect = { 0 };
			icon_rect.right = icon_dimension;
			icon_rect.bottom = icon_dimension;
			win32_center_rect_in_rect(&icon_rect, &button_rects.maximize);
			SelectObject(hdc, button_icon_pen);
			SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
			if (win32_window_is_maximized(m_hWnd)) 
			{
				// Draw the maximized icon
				Rectangle(hdc,
					icon_rect.left + WIN32_MAXIMIZED_RECTANGLE_OFFSET,
					icon_rect.top - WIN32_MAXIMIZED_RECTANGLE_OFFSET,
					icon_rect.right + WIN32_MAXIMIZED_RECTANGLE_OFFSET,
					icon_rect.bottom - WIN32_MAXIMIZED_RECTANGLE_OFFSET
				);
				FillRect(hdc, &icon_rect, is_hovered ? title_bar_hover_brush : title_bar_brush);
			}
			Rectangle(hdc, icon_rect.left, icon_rect.top, icon_rect.right, icon_rect.bottom);
		}

		{ // Close button
			HPEN custom_pen = 0;
			if (title_bar_hovered_button == CustomTitleBarHoveredButton_Close) 
			{
				HBRUSH fill_brush = CreateSolidBrush(RGB(0xCC, 0, 0));
				FillRect(hdc, &button_rects.close, fill_brush);
				DeleteObject(fill_brush);
				custom_pen = CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
				SelectObject(hdc, custom_pen);
			}
			RECT icon_rect = { 0 };
			icon_rect.right = icon_dimension;
			icon_rect.bottom = icon_dimension;
			win32_center_rect_in_rect(&icon_rect, &button_rects.close);
			MoveToEx(hdc, icon_rect.left, icon_rect.top, NULL);
			LineTo(hdc, icon_rect.right + 1, icon_rect.bottom + 1);
			MoveToEx(hdc, icon_rect.left, icon_rect.bottom, NULL);
			LineTo(hdc, icon_rect.right + 1, icon_rect.top - 1);
			if (custom_pen) DeleteObject(custom_pen);
		}
		DeleteObject(title_bar_hover_brush);
		DeleteObject(button_icon_brush);
		DeleteObject(button_icon_pen);
		DeleteObject(title_bar_brush);

		// Draw window title
		LOGFONT logical_font;
		HFONT old_font = NULL;
		if (SUCCEEDED(SystemParametersInfoForDpi(SPI_GETICONTITLELOGFONT, sizeof(logical_font), &logical_font, false, dpi))) {
			HFONT theme_font = CreateFontIndirect(&logical_font);
			old_font = (HFONT)SelectObject(hdc, theme_font);
		}

		wchar_t title_text_buffer[255] = { 0 };
		int buffer_length = sizeof(title_text_buffer) / sizeof(title_text_buffer[0]);
		GetWindowText(title_text_buffer, buffer_length);
		RECT title_bar_text_rect = title_bar_rect;
		// Add padding on the left
		int text_padding = 10; // There seems to be no good way to get this offset
		title_bar_text_rect.left += text_padding;
		// Add padding on the right for the buttons
		title_bar_text_rect.right = button_rects.minimize.left - text_padding;
		DTTOPTS draw_theme_options = { sizeof(draw_theme_options) };
		draw_theme_options.dwFlags = DTT_TEXTCOLOR;
		draw_theme_options.crText = title_bar_item_color;
		DrawThemeTextEx(
			theme,
			hdc,
			0, 0,
			title_text_buffer,
			-1,
			DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS,
			&title_bar_text_rect,
			&draw_theme_options
		);
		if (old_font) SelectObject(hdc, old_font);
		CloseThemeData(theme);

		// Paint fake top shadow. Original is missing because of the client rect extension.
		// You might need to tweak the colors here based on the color scheme of your app
		// or just remove it if you decide it is not worth it.
		static const COLORREF shadow_color = RGB(100, 100, 100);
		COLORREF fake_top_shadow_color = has_focus ? shadow_color : RGB(
			(GetRValue(title_bar_color) + GetRValue(shadow_color)) / 2,
			(GetGValue(title_bar_color) + GetGValue(shadow_color)) / 2,
			(GetBValue(title_bar_color) + GetBValue(shadow_color)) / 2
		);
		HBRUSH fake_top_shadow_brush = CreateSolidBrush(fake_top_shadow_color);
		RECT fake_top_shadow_rect = win32_fake_shadow_rect(m_hWnd);
		FillRect(hdc, &fake_top_shadow_rect, fake_top_shadow_brush);
		DeleteObject(fake_top_shadow_brush);

	}
#endif
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// try setting position if not set
		if ((m_nSinglePane == SPLIT_PANE_NONE) && (m_xySplitterPos == -1))
			SetSplitterPos();

		// do painting
		if (wParam != NULL)
		{
			DrawSplitter((HDC)wParam);
		}
		else
		{
			CPaintDC dc(m_hWnd);
			DrawSplitter(dc.m_hDC);
#if 0
			RECT rc = {};
			GetClientRect(&rc);
			DrawCustomTitle(dc.m_hDC, &rc);// &(dc.m_ps.rcPaint));
#endif
		}

		return 0;
	}

	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		
		if (((HWND)wParam == m_hWnd) && (LOWORD(lParam) == HTCLIENT))
		{
			DWORD dwPos = ::GetMessagePos();
			POINT ptPos = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
			ScreenToClient(&ptPos);
			if (IsOverSplitterBar(ptPos.x, ptPos.y))
				return 1;
		}

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (::GetCapture() == m_hWnd)
		{
			int xyNewSplitPos = 0;
			if (m_bVertical)
				xyNewSplitPos = xPos - m_rcSplitter.left - m_cxyDragOffset;
			else
				xyNewSplitPos = yPos - m_rcSplitter.top - m_cxyDragOffset;

			if (xyNewSplitPos == -1)   // avoid -1, that means default position
				xyNewSplitPos = -2;

			if (m_xySplitterPos != xyNewSplitPos)
			{
				if (m_bFullDrag)
				{
					if (SetSplitterPos(xyNewSplitPos, true))
						UpdateWindow();
				}
				else
				{
					DrawGhostBar();
					SetSplitterPos(xyNewSplitPos, false);
					DrawGhostBar();
				}
			}
		}
		else		// not dragging, just set cursor
		{
			if (IsOverSplitterBar(xPos, yPos))
			{
				::SetCursor(m_hCursor);
				bHandled = FALSE;
			}
#if 0
			else
			{
				CustomTitleBarHoveredButton title_bar_hovered_button;
				title_bar_hovered_button = (CustomTitleBarHoveredButton)GetWindowLongPtrW(GWLP_USERDATA);
				if (title_bar_hovered_button)
				{
					RECT title_bar_rect = win32_titlebar_rect(m_hWnd);
					// You could do tighter invalidation here but probably doesn't matter
					InvalidateRect(&title_bar_rect, FALSE);
					SetWindowLongPtrW(GWLP_USERDATA, (LONG_PTR)CustomTitleBarHoveredButton_None);
				}
			}
			bHandled = FALSE;
#endif 
		}

		return 0;
	}

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if ((::GetCapture() != m_hWnd) && IsOverSplitterBar(xPos, yPos))
		{
			m_xySplitterPosNew = m_xySplitterPos;
			SetCapture();
			m_hWndFocusSave = SetFocus();
			::SetCursor(m_hCursor);
			if (!m_bFullDrag)
				DrawGhostBar();
			if (m_bVertical)
				m_cxyDragOffset = xPos - m_rcSplitter.left - m_xySplitterPos;
			else
				m_cxyDragOffset = yPos - m_rcSplitter.top - m_xySplitterPos;
		}
		else if ((::GetCapture() == m_hWnd) && !IsOverSplitterBar(xPos, yPos))
		{
			::ReleaseCapture();
		}

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		
		if (::GetCapture() == m_hWnd)
		{
			m_xySplitterPosNew = m_xySplitterPos;
			::ReleaseCapture();
		}

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnLButtonDoubleClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		
		SetSplitterPos();   // default

		return 0;
	}

	LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if (!m_bFullDrag)
			DrawGhostBar();

		if ((m_xySplitterPosNew != -1) && (!m_bFullDrag || (m_xySplitterPos != m_xySplitterPosNew)))
		{
			m_xySplitterPos = m_xySplitterPosNew;
			m_xySplitterPosNew = -1;
			UpdateSplitterLayout();
			
			UpdateWindow();
		}

		if (m_hWndFocusSave != NULL)
			::SetFocus(m_hWndFocusSave);

		return 0;
	}

	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		
		if (::GetCapture() == m_hWnd)
		{
			switch (wParam)
			{
			case VK_RETURN:
				m_xySplitterPosNew = m_xySplitterPos;
				// FALLTHROUGH
			case VK_ESCAPE:
				::ReleaseCapture();
				break;
			case VK_LEFT:
			case VK_RIGHT:
				if (m_bVertical)
				{
					POINT pt = {};
					::GetCursorPos(&pt);
					int xyPos = m_xySplitterPos + ((wParam == VK_LEFT) ? -m_cxyStep : m_cxyStep);
					int cxyMax = m_rcSplitter.right - m_rcSplitter.left;
					if (xyPos < (m_cxyMin + m_cxyBarEdge))
						xyPos = m_cxyMin;
					else if (xyPos > (cxyMax - m_cxySplitBar - m_cxyBarEdge - m_cxyMin))
						xyPos = cxyMax - m_cxySplitBar - m_cxyBarEdge - m_cxyMin;
					pt.x += xyPos - m_xySplitterPos;
					::SetCursorPos(pt.x, pt.y);
				}
				break;
			case VK_UP:
			case VK_DOWN:
				if (!m_bVertical)
				{
					POINT pt = {};
					::GetCursorPos(&pt);
					int xyPos = m_xySplitterPos + ((wParam == VK_UP) ? -m_cxyStep : m_cxyStep);
					int cxyMax = m_rcSplitter.bottom - m_rcSplitter.top;
					if (xyPos < (m_cxyMin + m_cxyBarEdge))
						xyPos = m_cxyMin;
					else if (xyPos > (cxyMax - m_cxySplitBar - m_cxyBarEdge - m_cxyMin))
						xyPos = cxyMax - m_cxySplitBar - m_cxyBarEdge - m_cxyMin;
					pt.y += xyPos - m_xySplitterPos;
					::SetCursorPos(pt.x, pt.y);
				}
				break;
			default:
				break;
			}
		}
		else
		{
			bHandled = FALSE;
		}

		return 0;
	}

	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM, BOOL& bHandled)
	{
		
		if (::GetCapture() != m_hWnd)
		{
			if (m_nSinglePane == SPLIT_PANE_NONE)
			{
				if ((m_nDefActivePane == SPLIT_PANE_LEFT) || (m_nDefActivePane == SPLIT_PANE_RIGHT))
					::SetFocus(m_hWndPane[m_nDefActivePane]);
			}
			else
			{
				::SetFocus(m_hWndPane[m_nSinglePane]);
			}
		}

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		if ((lRet == MA_ACTIVATE) || (lRet == MA_ACTIVATEANDEAT))
		{
			DWORD dwPos = ::GetMessagePos();
			POINT pt = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
			ScreenToClient(&pt);
			RECT rcPane = {};
			for (int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if (GetSplitterPaneRect(nPane, &rcPane) && (::PtInRect(&rcPane, pt) != FALSE))
				{
					m_nDefActivePane = nPane;
					break;
				}
			}
		}

		return lRet;
	}

	LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		
		GetSystemSettings(true);

		return 0;
	}

	// Implementation - internal helpers
	void Init()
	{
		m_hCursor = ::LoadCursor(NULL, m_bVertical ? IDC_SIZEWE : IDC_SIZENS);

		
		GetSystemSettings(false);
	}

	void UpdateSplitterLayout()
	{
		if ((m_nSinglePane == SPLIT_PANE_NONE) && (m_xySplitterPos == -1))
			return;

		
		RECT rect = {};
		if (m_nSinglePane == SPLIT_PANE_NONE)
		{
			if (GetSplitterBarRect(&rect))
				InvalidateRect(&rect);

			for (int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if (GetSplitterPaneRect(nPane, &rect))
				{
					if (m_hWndPane[nPane] != NULL)
						::SetWindowPos(m_hWndPane[nPane], NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
					else
						InvalidateRect(&rect);
				}
			}
		}
		else
		{
			if (GetSplitterPaneRect(m_nSinglePane, &rect))
			{
				if (m_hWndPane[m_nSinglePane] != NULL)
					::SetWindowPos(m_hWndPane[m_nSinglePane], NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
				else
					InvalidateRect(&rect);
			}
		}
	}

	bool GetSplitterBarRect(LPRECT lpRect) const
	{
		ATLASSERT(lpRect != NULL);
		if ((m_nSinglePane != SPLIT_PANE_NONE) || (m_xySplitterPos == -1))
			return false;

		if (m_bVertical)
		{
			lpRect->left = m_rcSplitter.left + m_xySplitterPos;
			lpRect->top = m_rcSplitter.top;
			lpRect->right = m_rcSplitter.left + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge;
			lpRect->bottom = m_rcSplitter.bottom;
		}
		else
		{
			lpRect->left = m_rcSplitter.left;
			lpRect->top = m_rcSplitter.top + m_xySplitterPos;
			lpRect->right = m_rcSplitter.right;
			lpRect->bottom = m_rcSplitter.top + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge;
		}

		return true;
	}

	bool GetSplitterPaneRect(int nPane, LPRECT lpRect) const
	{
		ATLASSERT((nPane == SPLIT_PANE_LEFT) || (nPane == SPLIT_PANE_RIGHT));
		ATLASSERT(lpRect != NULL);
		bool bRet = true;
		if (m_nSinglePane != SPLIT_PANE_NONE)
		{
			if (nPane == m_nSinglePane)
				*lpRect = m_rcSplitter;
			else
				bRet = false;
		}
		else if (nPane == SPLIT_PANE_LEFT)
		{
			if (m_bVertical)
			{
				lpRect->left = m_rcSplitter.left;
				lpRect->top = m_rcSplitter.top;
				lpRect->right = m_rcSplitter.left + m_xySplitterPos;
				lpRect->bottom = m_rcSplitter.bottom;
			}
			else
			{
				lpRect->left = m_rcSplitter.left;
				lpRect->top = m_rcSplitter.top;
				lpRect->right = m_rcSplitter.right;
				lpRect->bottom = m_rcSplitter.top + m_xySplitterPos;
			}
		}
		else if (nPane == SPLIT_PANE_RIGHT)
		{
			if (m_bVertical)
			{
				lpRect->left = m_rcSplitter.left + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge;
				lpRect->top = m_rcSplitter.top;
				lpRect->right = m_rcSplitter.right;
				lpRect->bottom = m_rcSplitter.bottom;
			}
			else
			{
				lpRect->left = m_rcSplitter.left;
				lpRect->top = m_rcSplitter.top + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge;
				lpRect->right = m_rcSplitter.right;
				lpRect->bottom = m_rcSplitter.bottom;
			}
		}
		else
		{
			bRet = false;
		}

		return bRet;
	}

	bool IsOverSplitterRect(int x, int y) const
	{
		// -1 == don't check
		return (((x == -1) || ((x >= m_rcSplitter.left) && (x <= m_rcSplitter.right))) &&
			((y == -1) || ((y >= m_rcSplitter.top) && (y <= m_rcSplitter.bottom))));
	}

	bool IsOverSplitterBar(int x, int y) const
	{
		if (m_nSinglePane != SPLIT_PANE_NONE)
			return false;
		if ((m_xySplitterPos == -1) || !IsOverSplitterRect(x, y))
			return false;
		int xy = m_bVertical ? x : y;
		int xyOff = m_bVertical ? m_rcSplitter.left : m_rcSplitter.top;

		return ((xy >= (xyOff + m_xySplitterPos)) && (xy < (xyOff + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge)));
	}

	void DrawGhostBar()
	{
		RECT rect = {};
		if (GetSplitterBarRect(&rect))
		{
			// convert client to window coordinates
			
			RECT rcWnd = {};
			GetWindowRect(&rcWnd);
			::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcWnd, 2);
			::OffsetRect(&rect, -rcWnd.left, -rcWnd.top);

			// invert the brush pattern (looks just like frame window sizing)
			CBrush brush(CDCHandle::GetHalftoneBrush());
			if (brush.m_hBrush != NULL)
			{
				CWindowDC dc(m_hWnd);
				CBrushHandle brushOld = dc.SelectBrush(brush);
				dc.PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
				dc.SelectBrush(brushOld);
			}
		}
	}

	void GetSystemSettings(bool bUpdate)
	{
		if ((m_dwExtendedStyle & SPLIT_FIXEDBARSIZE) == 0)
		{
			m_cxySplitBar = ::GetSystemMetrics(m_bVertical ? SM_CXSIZEFRAME : SM_CYSIZEFRAME);
		}

		
		if ((GetExStyle() & WS_EX_CLIENTEDGE) != 0)
		{
			m_cxyBarEdge = 2 * ::GetSystemMetrics(m_bVertical ? SM_CXEDGE : SM_CYEDGE);
			m_cxyMin = 0;
		}
		else
		{
			m_cxyBarEdge = 0;
			m_cxyMin = 2 * ::GetSystemMetrics(m_bVertical ? SM_CXEDGE : SM_CYEDGE);
		}

		::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &m_bFullDrag, 0);

		if (bUpdate)
			UpdateSplitterLayout();
	}

	bool IsProportional() const
	{
		return ((m_dwExtendedStyle & SPLIT_PROPORTIONAL) != 0);
	}

	void StoreProportionalPos()
	{
		int cxyTotal = m_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		if (cxyTotal > 0)
			m_nProportionalPos = ::MulDiv(m_xySplitterPos, m_nPropMax, cxyTotal);
		else
			m_nProportionalPos = 0;
		ATLTRACE2(atlTraceUI, 0, _T("CSplitterImpl::StoreProportionalPos - %i\n"), m_nProportionalPos);
	}

	void UpdateProportionalPos()
	{
		int cxyTotal = m_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		if (cxyTotal > 0)
		{
			int xyNewPos = ::MulDiv(m_nProportionalPos, cxyTotal, m_nPropMax);
			m_bUpdateProportionalPos = false;
			
			SetSplitterPos(xyNewPos, false);
		}
	}

	bool IsRightAligned() const
	{
		return ((m_dwExtendedStyle & SPLIT_RIGHTALIGNED) != 0);
	}

	void StoreRightAlignPos()
	{
		int cxyTotal = m_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		if (cxyTotal > 0)
			m_nProportionalPos = cxyTotal - m_xySplitterPos;
		else
			m_nProportionalPos = 0;
		ATLTRACE2(atlTraceUI, 0, _T("CSplitterImpl::StoreRightAlignPos - %i\n"), m_nProportionalPos);
	}

	void UpdateRightAlignPos()
	{
		int cxyTotal = m_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		if (cxyTotal > 0)
		{
			m_bUpdateProportionalPos = false;
			
			SetSplitterPos(cxyTotal - m_nProportionalPos, false);
		}
	}

	bool IsInteractive() const
	{
		return ((m_dwExtendedStyle & SPLIT_NONINTERACTIVE) == 0);
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// handled, no background painting needed
		return 1;
	}

	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (wParam != SIZE_MINIMIZED)
		{
			SetSplitterRect();
		}

		//bHandled = FALSE;
		return 1;
	}
#if 0
	LRESULT OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CustomTitleBarHoveredButton title_bar_hovered_button;
		// Let the default procedure m_hWnd resizing areas
		LRESULT hit = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		switch (hit)
		{
		case HTNOWHERE:
		case HTRIGHT:
		case HTLEFT:
		case HTTOPLEFT:
		case HTTOP:
		case HTTOPRIGHT:
		case HTBOTTOMRIGHT:
		case HTBOTTOM:
		case HTBOTTOMLEFT:
			{
				return hit;
			}
		default:
			break;
		}
		
		title_bar_hovered_button =(CustomTitleBarHoveredButton)GetWindowLongPtrW(GWLP_USERDATA);
		// Check if hover button is on maximize to support SnapLayout on Windows 11
		if (title_bar_hovered_button == CustomTitleBarHoveredButton_Maximize) 
		{
			return HTMAXBUTTON;
		}

		// Looks like adjustment happening in NCCALCSIZE is messing with the detection
		// of the top hit area so manually fixing that.
		UINT dpi = GetDpiForWindow(m_hWnd);
		int frame_y = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
		int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
		POINT cursor_point = { 0 };
		cursor_point.x = GET_X_PARAM(lParam);
		cursor_point.y = GET_Y_PARAM(lParam);
		ScreenToClient(&cursor_point);
		if (cursor_point.y > 0 && cursor_point.y < frame_y + padding) 
		{
			return HTTOP;
		}

		// Since we are drawing our own caption, this needs to be a custom test
		if (cursor_point.y < win32_titlebar_rect(m_hWnd).bottom) 
		{
			return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT OnNCMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CustomTitleBarButtonRects button_rects;
		POINT cursor_point;
		GetCursorPos(&cursor_point);
		ScreenToClient(&cursor_point);

		CustomTitleBarHoveredButton title_bar_hovered_button =
			(CustomTitleBarHoveredButton)GetWindowLongPtrW(GWLP_USERDATA);

		RECT title_bar_rect = win32_titlebar_rect(m_hWnd);
		button_rects = win32_get_title_bar_button_rects(m_hWnd, &title_bar_rect);

		CustomTitleBarHoveredButton new_hovered_button = CustomTitleBarHoveredButton_None;
		
		if (PtInRect(&button_rects.close, cursor_point)) 
		{
			new_hovered_button = CustomTitleBarHoveredButton_Close;
		}
		else if (PtInRect(&button_rects.minimize, cursor_point)) 
		{
			new_hovered_button = CustomTitleBarHoveredButton_Minimize;
		}
		else if (PtInRect(&button_rects.maximize, cursor_point)) 
		{
			new_hovered_button = CustomTitleBarHoveredButton_Maximize;
		}

		if (new_hovered_button != title_bar_hovered_button) 
		{
			// You could do tighter invalidation here but probably doesn't matter
			InvalidateRect(&button_rects.close, FALSE);
			InvalidateRect(&button_rects.minimize, FALSE);
			InvalidateRect(&button_rects.maximize, FALSE);

			SetWindowLongPtrW(GWLP_USERDATA, (LONG_PTR)new_hovered_button);
		}

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNCLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Clicks on buttons will be handled in WM_NCLBUTTONUP, but we still need
		// to remove default handling of the click to avoid it counting as drag.
		//
		// Ideally you also want to check that the mouse hasn't moved out or too much
		// between DOWN and UP messages.
		CustomTitleBarHoveredButton title_bar_hovered_button;
		title_bar_hovered_button = (CustomTitleBarHoveredButton)GetWindowLongPtrW(GWLP_USERDATA);
		if (title_bar_hovered_button) 
		{
			return 0;
		}
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNCLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CustomTitleBarHoveredButton title_bar_hovered_button;
		title_bar_hovered_button = (CustomTitleBarHoveredButton)GetWindowLongPtrW(GWLP_USERDATA);

		if (title_bar_hovered_button == CustomTitleBarHoveredButton_Close) 
		{
			PostMessageW(WM_CLOSE, 0, 0);
			return 0;
		}
		else if (title_bar_hovered_button == CustomTitleBarHoveredButton_Minimize) {
			ShowWindow(SW_MINIMIZE);
			return 0;
		}
		else if (title_bar_hovered_button == CustomTitleBarHoveredButton_Maximize) 
		{
			int mode = win32_window_is_maximized(m_hWnd) ? SW_NORMAL : SW_MAXIMIZE;
			ShowWindow(mode);
			return 0;
		}
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT title_bar_rect = win32_titlebar_rect(m_hWnd);
		InvalidateRect(&title_bar_rect, FALSE);
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNCCalcSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Handling this event allows us to extend client (paintable) area into the title bar region
		// The information is partially coming from:
		// https://docs.microsoft.com/en-us/windows/win32/dwm/customframe#extending-the-client-frame
		// Most important paragraph is:
		//   To remove the standard window frame, you must m_hWnd the WM_NCCALCSIZE message,
		//   specifically when its wParam value is TRUE and the return value is 0.
		//   By doing so, your application uses the entire window region as the client area,
		//   removing the standard frame.

		if (wParam) 
		{
			UINT dpi = GetDpiForWindow(m_hWnd);
			int frame_x = GetSystemMetricsForDpi(SM_CXFRAME, dpi);
			int frame_y = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
			int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);

			NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lParam;
			RECT* requested_client_rect = params->rgrc;

			requested_client_rect->right -= frame_x + padding;
			requested_client_rect->left += frame_x + padding;
			requested_client_rect->bottom -= frame_y + padding;

			if (win32_window_is_maximized(m_hWnd)) 
			{
				requested_client_rect->top += padding;
			}
		}
		else bHandled = FALSE; // return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);

		return 0;
	}
#endif 
};
