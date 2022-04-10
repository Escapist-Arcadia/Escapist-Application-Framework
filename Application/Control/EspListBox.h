#pragma once

#include"../Basic/EspWindow.h"

class EspListBox :public EspWindow
{
public:
	virtual bool Create(
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
			WC_LISTBOX,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_HASSTRINGS | style,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window,
			NULL,
			EspGetApp()->GetInstanceHandle(),
			nullptr
		);
		return (handle != nullptr);
	}

	inline void AddItem(LPCTSTR str) { ::SendMessage(handle, LB_ADDSTRING, 0, (LPARAM)str); }
	inline void DeleteItem(unsigned int index) { ::SendMessage(handle, LB_DELETESTRING, index, 0); }
	inline void InsertItem(unsigned int index, LPCTSTR str) { ::SendMessage(handle, LB_INSERTSTRING, index, (LPARAM)str); }
	inline unsigned int FindItem(unsigned int index, LPCTSTR str) { ::SendMessage(handle, LB_FINDSTRING, index, (LPARAM)str); }
	inline unsigned int GetItemCount()const { return ::SendMessage(handle, LB_GETCOUNT, 0, 0); }
	inline unsigned int GetSelectedItem()const { return ::SendMessage(handle, LB_GETCURSEL, 0, 0); }
	inline bool GetSelectState(unsigned int index)const { return ::SendMessage(handle, LB_GETSEL, index, 0); }
	inline unsigned int GetSelectedItemCount()const { return ::SendMessage(handle, LB_GETSELCOUNT, 0, 0); }
	inline void GetSelectedItems(const int* ptr, unsigned int capacity)const { ::SendMessage(handle, LB_GETSELITEMS, capacity, (LPARAM)(ptr)); }
	inline int GetItemStringLength(unsigned int index)const { return ::SendMessage(handle, LB_GETTEXTLEN, index, 0); }
	inline void GetItemString(unsigned int index, LPTSTR str)const { ::SendMessage(handle, LB_GETTEXT, index, (LPARAM)str); }
	inline unsigned int GetCurrentTopIndex()const { return ::SendMessage(handle, LB_GETTOPINDEX, 0, 0); }
	inline void EmptyItems() { ::SendMessage(handle, LB_RESETCONTENT, 0, 0); }
	inline void SelectItem(unsigned int index, LPCTSTR str) { ::SendMessage(handle, LB_SELECTSTRING, index, (LPARAM)str); }
	inline void SetSelectedItem(unsigned int index) { ::SendMessage(handle, LB_SETCURSEL, index, 0); }
};