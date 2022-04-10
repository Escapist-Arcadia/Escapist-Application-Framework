#pragma once

#include"../Basic/EspWindow.h"

class EspLabel :public EspWindow
{
public:
	virtual bool Create(
		LPCTSTR window_name,
		int x, int y, int width, int height,
		HWND parent_window,
		DWORD style = 0
	)
	{
		handle = ::CreateWindowEx(
			WS_EX_LEFT,
			WC_STATIC,
			window_name,
			WS_CHILD | WS_VISIBLE | style,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window,
			nullptr,
			EspGetApp()->GetInstanceHandle(),
			nullptr
		);

		return handle != nullptr;
	}
};