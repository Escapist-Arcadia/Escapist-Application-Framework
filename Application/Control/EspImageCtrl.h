#pragma once

#include"../Basic/EspWindow.h"

class EspImageCtrl :public EspWindow
{
public:
	virtual bool Create(
		int x, int y, int width, int height,
		HWND parent_window,
		DWORD style
	)
	{
		handle = ::CreateWindowEx(
			WS_EX_LEFT,
			WC_STATIC,
			nullptr,
			WS_CHILD | WS_VISIBLE | SS_BITMAP | style,
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

	void SetImageFromPath(LPCTSTR path)
	{
		Gdiplus::Rect rect;
		::GetClientRect(handle, (RECT*)&rect);
		Gdiplus::Bitmap res(rect.Width, rect.Height, PixelFormat32bppARGB);
		Gdiplus::Graphics g(&res);
		
		Gdiplus::Bitmap init(path);
		g.DrawImage(&init, 0, 0, rect.Width, rect.Height);

		HBITMAP hbmp;
		res.GetHBITMAP(Gdiplus::Color(0).GetValue(), &hbmp);

		::SendMessage(handle, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbmp);
	}
};