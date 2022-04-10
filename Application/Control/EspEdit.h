#pragma once

#include"../Basic/EspWindow.h"

class EspEdit :public EspWindow
{
public:
	virtual bool Create(
		LPCTSTR window_name,
		int x,
		int y,
		int width,
		int height,
		HWND parent_window,
		DWORD style
	)
	{
		handle = ::CreateWindowEx(
			WS_EX_CLIENTEDGE,
			WC_EDIT,
			window_name,
			WS_CHILD | WS_VISIBLE | style,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window, NULL, EspGetApp()->GetInstanceHandle(), nullptr);
		return (handle != nullptr);
	}

	int GetLineCount()const { return ::SendMessage(handle, EM_GETLINECOUNT, 0, 0); }
	void GetSelectedPos(DWORD& start, DWORD& end) { ::SendMessage(handle, EM_GETSEL, start, end); }
	void SetSelectedPos(DWORD& start, const DWORD& end) { ::SendMessage(handle, EM_SETSEL, start, end); }
	LPTSTR GetTextPtr()const { return (LPTSTR)::SendMessage(handle, EM_GETHANDLE, 0, 0); }
	void Undo()const { ::SendMessage(handle, EM_UNDO, 0, 0); }
};