// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
	UINT m_nDPI = 0;
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
#if 0
	CView m_view;
#endif 

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return FALSE; // m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
#if 0
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
#endif 
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		m_nDPI = GetDpiForWindow(m_hWnd);
#if 0
		m_hWndClient = CreateWindowExW(0, L"Scintilla", NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
			0, 0, 16, 16, m_hWnd, NULL, HINST_THISCOMPONENT, NULL);

		if (::IsWindow(m_hWndClient))
		{
			::SendMessage(m_hWndClient, SCI_SETTECHNOLOGY, SC_TECHNOLOGY_DIRECTWRITE, 0);
			::SendMessage(m_hWndClient, SCI_SETCODEPAGE, SC_CP_UTF8, 0);
			::SendMessage(m_hWndClient, SCI_SETEOLMODE, SC_EOL_LF, 0);
			::SendMessage(m_hWndClient, SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
			::SendMessage(m_hWndClient, SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Courier New");
			::SendMessage(m_hWndClient, SCI_STYLESETSIZEFRACTIONAL, STYLE_DEFAULT, 1300);

			//::SendMessage(m_hWnd, SCI_STYLESETBACK, STYLE_LINENUMBER, RGB(255, 255, 255));
			//::SendMessage(m_hWnd, SCI_SETMARGINWIDTHN, 1, 2);
			::SendMessage(m_hWndClient, SCI_SETPHASESDRAW, SC_PHASES_MULTIPLE, 0);

			::SendMessage(m_hWndClient, SCI_SETMARGINWIDTHN, 0, 0);
			::SendMessage(m_hWndClient, SCI_SETMARGINS, 0, 0);
			::SendMessage(m_hWndClient, SCI_SETMARGINLEFT, 0, 0);
#if 0
			::SendMessage(m_hWndClient, SCI_STYLESETBACK, STYLE_LINENUMBER, RGB(0, 0, 0));
			::SendMessage(m_hWndClient, SCI_STYLESETBACK, STYLE_DEFAULT, RGB(13, 13, 13));
			::SendMessage(m_hWndClient, SCI_STYLESETFORE, STYLE_DEFAULT, RGB(250, 250, 250));
			::SendMessage(m_hWndClient, SCI_SETCARETFORE, RGB(250, 250, 250), 0);
			::SendMessage(m_hWndClient, SCI_STYLECLEARALL, 0, 0);

			::SetWindowTheme(m_hWndClient, L"DarkMode_Explorer", nullptr);
#endif 
#if 0
			::SendMessage(m_hWnd, SCI_SETVSCROLLBAR, 0, 0);
#endif 
		}
#endif 
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

	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		auto dpiWindow = GetDpiForWindow(m_hWnd);

		bHandled = FALSE;
		return 0;
	}

#if 0
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
#endif 
};
