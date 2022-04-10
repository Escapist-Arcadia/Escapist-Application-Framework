#pragma once

#include"../Basic/EspWindow.h"

class EspCheckBox :public EspWindow
{
public:
	virtual bool Create(LPCTSTR window_name, int x, int y, int width, int height, HWND parent_window, DWORD style = 0)
	{
		handle = ::CreateWindowEx(
			WS_EX_LEFT,
			WC_BUTTON,
			window_name,
			WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_CHECKBOX | style,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window,
			nullptr,
			EspGetApp()->GetInstanceHandle(),
			nullptr
		);
		return GetHandle() == nullptr;
	}

	UINT GetCheckState()const { return ::SendMessage(GetHandle(), BM_GETCHECK, 0, 0); }
	bool IsChecked()const { return GetCheckState() == BST_CHECKED; }
	void SetCheckState(UINT state)const { ::SendMessage(GetHandle(), BM_SETCHECK, state, 0); }
	void SetChecked() { SetCheckState(BST_CHECKED); }
	void SetUnChecked() { SetCheckState(BST_UNCHECKED); }
};