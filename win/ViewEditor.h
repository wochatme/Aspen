// View.h : interface of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
class CEditView 
{
public:
	HWND m_hWnd = NULL;

	HWND Create(
		_In_opt_ HWND hWndParent,
		_In_ _U_RECT rect = NULL,
		_In_opt_z_ LPCTSTR szWindowName = NULL,
		_In_ DWORD dwStyle = 0,
		_In_ DWORD dwExStyle = 0,
		_In_ _U_MENUorID MenuOrID = 0U,
		_In_opt_ LPVOID lpCreateParam = NULL)
	{
		m_hWnd = CreateWindowExW(0, L"Scintilla", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
			0, 0, 16, 16, hWndParent, NULL, HINST_THISCOMPONENT, NULL);

		if (::IsWindow(m_hWnd))
		{
			::SendMessage(m_hWnd, SCI_SETTECHNOLOGY, SC_TECHNOLOGY_DIRECTWRITE, 0);
			::SendMessage(m_hWnd, SCI_SETCODEPAGE, SC_CP_UTF8, 0);
			::SendMessage(m_hWnd, SCI_SETEOLMODE, SC_EOL_LF, 0);
			::SendMessage(m_hWnd, SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
			::SendMessage(m_hWnd, SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Courier New");
			::SendMessage(m_hWnd, SCI_STYLESETSIZEFRACTIONAL, STYLE_DEFAULT, 1100);
#if 0
			::SendMessage(m_hWnd, SCI_STYLESETBACK, STYLE_LINENUMBER, RGB(255, 255, 255));
			::SendMessage(m_hWnd, SCI_SETMARGINWIDTHN, 1, 2);
			::SendMessage(m_hWnd, SCI_SETPHASESDRAW, SC_PHASES_MULTIPLE, 0);
			::SendMessage(m_hWnd, SCI_SETREADONLY, 1, 0);
			::SendMessage(m_hWnd, SCI_SETVSCROLLBAR, 0, 0);
#endif 
		}
		return m_hWnd;
	}

	operator HWND() const throw()
	{
		return m_hWnd;
	}

	BOOL IsWindow() const throw()
	{
		return ::IsWindow(m_hWnd);
	}

	BOOL GetFocus()
	{
		return (m_hWnd == ::GetFocus());
	}

	BOOL ShowWindow(int nCmdShow)
	{
		BOOL bRet = FALSE;

		if (IsWindow())
		{
			bRet = ::ShowWindow(m_hWnd, nCmdShow);
		}

		return bRet;
	}

	BOOL SetWindowPos(HWND hWndInsertAfter, int  X, int  Y, int  cx, int  cy, UINT uFlags)
	{
		BOOL bRet = FALSE;
		if (IsWindow())
		{
			bRet = ::SetWindowPos(m_hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
		}
		return bRet;
	}

	int ShowRect(RECT* rw, RECT* rc)
	{
		if (rw && rc)
		{
			int w1, h1, w2, h2;
			char string[256] = { 0 };
			w1 = rw->right - rw->left;
			h1 = rw->bottom - rw->top;
			w2 = rc->right - rc->left;
			h2 = rc->bottom - rc->top;

			sprintf(string, "WR:l=%d,t=%d,r=%d,b=%d(%d:%d)\nCR:l=%d,t=%d,r=%d,b=%d(%d:%d)\n\n\n", 
				rw->left, rw->top, rw->right, rw->bottom, w1, h1,
				rc->left, rc->top, rc->right, rc->bottom, w2, h2
				);
			::SendMessage(m_hWnd, SCI_SETTEXT, 0, (LPARAM)string);
		}
		return 0;
	}

	int AppendText(WinMsg* msgarray, U16 count)
	{
		if (IsWindow() && count > 0)
		{
			SIZE_T size = (1 << 16);
			char* text = (char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
			if (text)
			{
				const char* hex = "0123456789ABCDEF";
				char string[32];
				U32 len = 0;
				U64 raw;
				U16 i, k;
				U8 idx;
				WinMsg* pm;
				char* s;
				char* p = text;

				for (i = 0; i < count; i++)
				{
					pm = msgarray + i;
					raw = (U64)(pm->hWnd);
					s = (char*)&raw;
					idx = sizeof(U64);
					for (k = 0; k < sizeof(U64); k++)
					{
						idx = (*s) & 0xF0;
						idx >>= 4;
						*p++ = hex[idx]; len++;
						idx = (*s) & 0x0F;
						*p++ = hex[idx]; len++;
						s++;
					}
					*p++ = ' '; len++;
					*p++ = '-'; len++;
					*p++ = ' '; len++;
					s = get_message_name(pm->msg);
					while (*s)
					{
						*p++ = *s++;
						len++;
					}
					if (pm->count > 1)
					{
						*p++ = ':'; len++;
						sprintf(string, "%d", pm->count);
						s = string;
						while (*s)
						{
							*p++ = *s++;
							len++;
						}
					}

					*p++ = '\n';
					len++;
				}
				*p++ = '-'; len++;
				*p++ = '-'; len++;
				*p++ = '-'; len++;
				*p++ = '-'; len++;
				*p++ = '-'; len++;
				*p++ = '-'; len++;
				*p++ = '-'; len++;
				*p++ = '-'; len++;
				*p++ = '\n'; len++;
				*p = '\0';
#if 0
				::SendMessage(m_hWnd, SCI_SETREADONLY, 0, 0);
#endif 
				::SendMessage(m_hWnd, SCI_APPENDTEXT, len, (LPARAM)text);
#if 0
				::SendMessage(m_hWnd, SCI_SETREADONLY, 1, 0);
#endif
				VirtualFree(text, 0, MEM_RELEASE);
			}
		}
		return 0;
	}

};

