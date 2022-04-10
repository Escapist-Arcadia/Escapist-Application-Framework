#pragma once

#include"EspApplication.h"

#define WindowProc_Start(cls) LRESULT __stdcall cls::WindowProc(UINT message, WPARAM w_param, LPARAM l_param){switch(message){
#define WindowProc_Message(msg,func)case msg:return func(w_param,l_param);
#define WindowProc_End()case WM_NCDESTROY:delete this;}return ::DefWindowProc(GetHandle(),message,w_param,l_param);}

#define WindowProc_OnCommand_Start(cls)LRESULT __stdcall cls::OnCommand(WORD notif, WORD identifier, HWND ctrl_handle){switch(notif){
#define WindowProc_OnCommand_End()}return 0;}

//WM_CREATE
#define WindowProc_OnCreate(func)case WM_CREATE:return func((CREATESTRUCT*)l_param);
//WM_COMMAND
#define WindowProc_OnCommand(func)case WM_COMMAND:return func(HIWORD(w_param),LOWORD(w_param),(HWND)l_param);
//WM_DESTROY
#define WindowProc_OnDestroy(func)case WM_DESTROY:return func();
//WM_LBUTTONDOWN
#define WindowProc_OnLeftButtonDown(func)case WM_LBUTTONDOWN:return func(w_param,{GET_X_LPARAM(l_param),GET_Y_LPARAM(l_param)});
//WM_LBUTTONUP
#define WindowProc_OnLeftButtonUp(func)case WM_LBUTTONUP:return func(w_param,{GET_X_LPARAM(l_param),GET_Y_LPARAM(l_param)});
//WM_RBUTTONDOWN
#define WindowProc_OnRightButtonDown(func)case WM_RBUTTONDOWN:return func(w_param,{GET_X_LPARAM(l_param),GET_Y_LPARAM(l_param)});
//WM_RBUTTONUP
#define WindowProc_OnRightButtonUp(func)case WM_RBUTTONUP:return func(w_param,{GET_X_LPARAM(l_param),GET_Y_LPARAM(l_param)});
//WM_MOUSEMOVE
#define WindowProc_OnMouseMove(func)case WM_MOUSEMOVE:return func(w_param,{GET_X_LPARAM(l_param),GET_Y_LPARAM(l_param)});
//WM_ENTERSIZEMOVE
#define WindowProc_OnEnterSizeMove(func)case WM_ENTERSIZEMOVE:return func();
//WM_EXITSIZEMOVE
#define WindowProc_OnExitSizeMove(func)case WM_EXITSIZEMOVE:return func();

//WM_DPICHANGED
#define WindowProc_OnDPIChanged(func)case WM_DPICHANGED:return func((RECT*)l_param);



class EspWindow
{
protected:
	HWND handle;
	LONG_PTR data;

	struct EspCreateParams
	{
		EspWindow* window;
		LPVOID data;
	};

	static LRESULT __stdcall EspWindowProc(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
	{
		if (message == WM_NCCREATE)
		{
			CREATESTRUCT* cs_ptr = (CREATESTRUCT*)l_param;
			if (cs_ptr)
			{
				EspWindow* window_ptr = ((EspCreateParams*)cs_ptr->lpCreateParams)->window;
				if (window_ptr)
				{
					window_ptr->SetHandle(handle);
					//SetWindowLongPtr supports 64-bit
					::SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)window_ptr);
					return window_ptr->WindowProc(message, w_param, l_param);
				}
			}
		}
		else
		{
			EspWindow* window_ptr = (EspWindow*)::GetWindowLongPtr(handle, GWLP_USERDATA);
			if (window_ptr)
				return window_ptr->WindowProc(message, w_param, l_param);
		}
		return ::DefWindowProc(handle, message, w_param, l_param);
	}

#define WindowProc_OnCreate_HaveParam(func)case WM_CREATE:return func((CREATESTRUCT*)l_param,(LPVOID)(((EspCreateParams*)(((CREATESTRUCT*)l_param)->lpCreateParams))->data));


public:
	inline const HWND& GetHandle()const { return handle; }
	inline void SetHandle(const HWND& new_handle)
	{
		handle = new_handle;
	}

	virtual bool Create(
		LPCTSTR window_name,
		int x,
		int y,
		int width,
		int height,
		DWORD style,
		DWORD extended_style,
		HICON icon_handle,
		HICON small_icon_handle,
		HCURSOR cursor_handle,
		HBRUSH brush_handle,
		LPCTSTR class_name,
		HMENU menu_handle,
		HWND parent_window,
		LPVOID param
	)
	{
		WNDCLASSEX wndcls;
		wndcls.cbSize = sizeof(WNDCLASSEX);
		if (!::GetClassInfoEx(EspGetApp()->GetInstanceHandle(), class_name, &wndcls))
		{
			wndcls.style = CS_HREDRAW | CS_VREDRAW;
			wndcls.lpfnWndProc = EspWindow::EspWindowProc;
			wndcls.cbClsExtra = 0;
			wndcls.cbWndExtra = 0;
			wndcls.hInstance = EspGetApp()->GetInstanceHandle();
			wndcls.hIcon = icon_handle;
			wndcls.hCursor = cursor_handle;
			wndcls.hbrBackground = brush_handle;
			wndcls.lpszMenuName = nullptr;
			wndcls.lpszClassName = class_name;
			wndcls.hIconSm = small_icon_handle;

			if (!::RegisterClassEx(&wndcls))
				return false;
		}

		EspCreateParams param_ptr = { this,param };

		handle = ::CreateWindowEx(
			extended_style,
			class_name,
			window_name,
			style,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window,
			menu_handle,
			EspGetApp()->GetInstanceHandle(),
			(LPVOID)&param_ptr
		);
		if (handle == nullptr)
			return false;

		return true;
	}

	inline void SetShowMode(int command_show) { ::ShowWindow(handle, command_show); }
	inline void Update() { ::UpdateWindow(handle); }

	inline void Show() { ::ShowWindow(handle, SW_SHOW); }
	inline void Hide() { ::ShowWindow(handle, SW_HIDE); }
	inline void Minimize() { ::ShowWindow(handle, SW_MINIMIZE); }
	inline void Maximize() { ::ShowWindow(handle, SW_MAXIMIZE); }
	inline void Restore() { ::ShowWindow(handle, SW_RESTORE); }

	inline int GetTextLength()const { return ::GetWindowTextLength(handle); }
	inline int GetText(LPTSTR text)const { return ::GetWindowText(handle, text, GetTextLength() + 1); }
	inline int GetText(LPTSTR text, int max_len)const { return ::GetWindowText(handle, text, max_len); }
	inline void SetText(LPCTSTR text) { ::SetWindowText(handle, text); }

	inline ULONGLONG GetStyle() { return ::GetWindowLongPtr(handle, GWL_STYLE); }
	inline ULONGLONG GetExtendedStyle() { return ::GetWindowLongPtr(handle, GWL_EXSTYLE); }
	inline WNDPROC GetWindowProc() { return (WNDPROC)::GetWindowLongPtr(handle, GWLP_WNDPROC); }
	inline LONG_PTR GetUserData() { return data; }
	inline void SetStyle(ULONGLONG new_style) { ::SetWindowLongPtr(handle, GWL_STYLE, new_style); }
	inline void SetExtendedStyle(ULONGLONG new_style) { ::SetWindowLongPtr(handle, GWL_EXSTYLE, new_style); }
	inline WNDPROC SetWindowProc(WNDPROC proc) { return (WNDPROC)::SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)proc); }
	inline void SetUserData(LONG_PTR new_data) { data = new_data; }

	inline HCURSOR GetCursor() { return (HCURSOR)::GetClassLongPtr(handle, GCLP_HCURSOR); }
	inline HICON GetIcon() { return (HCURSOR)::GetClassLongPtr(handle, GCLP_HICON); }
	inline HICON GetSmallIcon() { return (HCURSOR)::GetClassLongPtr(handle, GCLP_HICONSM); }
	inline void SetCursor(HCURSOR cursor) { ::SetClassLongPtr(handle, GCLP_HCURSOR, (LONG_PTR)cursor); }
	inline void SetIcon(HICON icon) { ::SetClassLongPtr(handle, GCLP_HICON, (LONG_PTR)icon); }
	inline void SetSmallIcon(HICON icon) { ::SetClassLongPtr(handle, GCLP_HICONSM, (LONG_PTR)icon); }

	inline HWND GetParent()const { return ::GetParent(handle); }
	inline HWND GetAncestor()const { return ::GetAncestor(handle, GA_ROOT); }

	inline void Enable() { ::EnableWindow(handle, 1); }
	inline void Disenable() { ::EnableWindow(handle, 0); }
	inline bool IsEnabled() { return ::IsWindowEnabled(handle); }
	inline bool IsVisible() { return ::IsWindowVisible(handle);}

	inline HMENU GetMenu()const { return (HMENU)::SendMessage(handle, MN_GETHMENU, 0, 0); }
	inline HFONT GetFont()const { return (HFONT)::SendMessage(handle, WM_GETFONT, 0, 0); }
	inline void SetFont(HFONT font, bool redraw = false) { ::SendMessage(handle, WM_SETFONT, (WPARAM)font, redraw); }

	inline HWND SetFocus() { return ::SetFocus(handle); }

	inline void Destroy() { ::DestroyWindow(handle); }

	inline void CenterWindow()
	{
		RECT rect = { 0 };
		::GetWindowRect(handle, &rect);
		::SetWindowPos(handle, HWND_TOP,
			((GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2),
			((GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2),
			rect.right, rect.bottom, SWP_SHOWWINDOW);
	}

	virtual LRESULT __stdcall WindowProc(UINT message, WPARAM w_param, LPARAM l_param)
	{
		if (message == WM_DESTROY)
			::PostQuitMessage(0);

		return ::DefWindowProc(handle, message, w_param, l_param);
	}

	virtual LRESULT __stdcall OnCreate(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_CREATE, w_param, l_param); }
	virtual LRESULT __stdcall OnCreate(CREATESTRUCT* cs_ptr) { return 0; }
	virtual LRESULT __stdcall OnCreate(CREATESTRUCT* cs_ptr, LPVOID param) { return 0; }
	virtual LRESULT __stdcall OnCommand(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_COMMAND, w_param, l_param); }
	virtual LRESULT __stdcall OnCommand(WORD notif, WORD identifier, HWND ctrl_handle) { return 0; }
	virtual LRESULT __stdcall OnDestroy(WPARAM w_param, LPARAM l_param) { ::PostQuitMessage(0); return 0; }
	virtual LRESULT __stdcall OnDestroy() { ::PostQuitMessage(0); return 0; }
	virtual LRESULT __stdcall OnLeftButtonDown(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_LBUTTONDOWN, w_param, l_param); }
	virtual LRESULT __stdcall OnLeftButtonDown(ULONGLONG virtual_key, POINT point) { return 0; }
	virtual LRESULT __stdcall OnLeftButtonUp(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_LBUTTONUP, w_param, l_param); }
	virtual LRESULT __stdcall OnLeftButtonUp(ULONGLONG virtual_key, POINT point) { return 0; }
	virtual LRESULT __stdcall OnRightButtonDown(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_RBUTTONDOWN, w_param, l_param); }
	virtual LRESULT __stdcall OnRightButtonDown(ULONGLONG virtual_key, POINT point) { return 0; }
	virtual LRESULT __stdcall OnRightButtonUp(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_RBUTTONUP, w_param, l_param); }
	virtual LRESULT __stdcall OnRightButtonUp(ULONGLONG virtual_key, POINT point) { return 0; }
	virtual LRESULT __stdcall OnMouseMove(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_RBUTTONUP, w_param, l_param); }
	virtual LRESULT __stdcall OnMouseMove(ULONGLONG virtual_key, POINT point) { return 0; }
	virtual LRESULT __stdcall OnEnterSizeMove(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_RBUTTONUP, w_param, l_param); }
	virtual LRESULT __stdcall OnEnterSizeMove() { return 0; }
	virtual LRESULT __stdcall OnExitSizeMove(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_RBUTTONUP, w_param, l_param); }
	virtual LRESULT __stdcall OnExitSizeMove() { return 0; }
	virtual LRESULT __stdcall OnDPIChanged(WPARAM w_param, LPARAM l_param) { return ::DefWindowProc(handle, WM_DPICHANGED, w_param, l_param); }
	virtual LRESULT __stdcall OnDPIChanged(RECT* rect) { return 0; }

};