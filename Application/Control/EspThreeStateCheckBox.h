#pragma once

#include"../Basic/EspWindow.h"
#include"EspCheckBox.h"

class EspThreeStateCheckBox :public EspCheckBox
{
public:
	virtual bool Create(
		LPCTSTR window_name,
		int x, int y, int width, int height,
		HWND parent_window,
		UINT control_id,
		DWORD style = 0
	)
	{
		handle = ::CreateWindowEx(
			WS_EX_LEFT,
			WC_BUTTON,
			window_name,
			WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_CHECKBOX | BS_3STATE | style,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window,
			(HMENU)control_id,
			EspGetApp()->GetInstanceHandle(),
			nullptr
		);

		return handle != nullptr;
	}

};