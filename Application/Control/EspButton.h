#pragma once

#include"../Basic/EspWindow.h"

#define WindowProc_Button_OnClicked_Start()case BN_CLICKED:{switch(identifier){
#define WindowProc_Button_OnDoubleClicked_Start()case BN_DBLCLK:{switch(identifier){
#define WindowProc_Button_Control(ident,func)case ident:return func();
#define WindowProc_Button_End()}}


class EspButton :public EspWindow
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
			WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_PUSHBUTTON | style,
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

	virtual void Click() { ::SendMessage(GetHandle(), BM_CLICK, 0, 0); }
	virtual UINT GetButtonState()const { return ::SendMessage(GetHandle(), BM_GETSTATE, 0, 0); }


};