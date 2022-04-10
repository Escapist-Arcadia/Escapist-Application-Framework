#pragma once

#include"../Basic/EspWindow.h"

class EspGroupBox :public EspWindow
{
public:
	virtual bool Create(
		LPCTSTR window_name,
		int x, int y, int width, int height,
		HWND parent_window
	)
	{
		handle = ::CreateWindowEx(
			WS_EX_LEFT,
			WC_BUTTON,
			window_name,
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window,
			nullptr,
			EspGetApp()->GetInstanceHandle(),
			(LPVOID)this
		);

		//SetWindowProc(EspGroupBox::EspGroupBoxProc);

		return handle != nullptr;
	}


};