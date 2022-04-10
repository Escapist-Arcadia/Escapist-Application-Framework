#pragma once

#include"../Basic/EspWindow.h"

class EspComboBox :public EspWindow
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
			WC_COMBOBOX,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |CBS_HASSTRINGS| style,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window,
			NULL,
			EspGetApp()->GetInstanceHandle(),
			nullptr);
		return (handle != nullptr);
	}

	inline void AddItem(LPCTSTR str) const { ::SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)str); }
	inline void InsertItem(unsigned int index, LPCTSTR str)const { ::SendMessage(handle, CB_INSERTSTRING, index, (LPARAM)str); }
	inline int FindItem(LPCTSTR str, unsigned int start_index)const { return ::SendMessage(handle, CB_FINDSTRING, start_index, (LPARAM)str); }
	inline void GetItemString(unsigned int index, LPTSTR str)const { ::SendMessage(handle, CB_GETLBTEXT, index, (LPARAM)str); }
	inline void DeleteItem(unsigned int index) { ::SendMessage(handle, CB_DELETESTRING, index, 0); }
	inline unsigned int GetItemStringLength(unsigned int index)const { return ::SendMessage(handle, CB_GETLBTEXTLEN, index, 0); }
	inline unsigned int GetCurrentTopIndex()const { return ::SendMessage(handle, CB_GETTOPINDEX, 0, 0); }
	inline void SetCurrentTopIndex(unsigned int index)const { ::SendMessage(handle, CB_SETTOPINDEX, index, 0); }
	inline unsigned int GetItemCount()const { return ::SendMessage(handle, CB_GETCOUNT, 0, 0); }
	inline unsigned int GetSelectedItem()const { return ::SendMessage(handle, CB_GETCURSEL, 0, 0); }
	inline void SetSelectedItem(unsigned int index)const { ::SendMessage(handle, CB_SETCURSEL, index, 0); }
	inline bool IsDroppedDown()const { return ::SendMessage(handle, CB_GETDROPPEDSTATE, 0, 0); }
	inline bool GetDropDownWidth()const { return ::SendMessage(handle, CB_GETDROPPEDWIDTH, 0, 0); }
	inline void ShowDropDown()const { ::SendMessage(handle, CB_SHOWDROPDOWN, true, 0); }
	inline bool SelectItem(unsigned int index, LPCTSTR str)const { return ::SendMessage(handle, CB_SELECTSTRING, index, (LPARAM)str); }
	inline void LimitLength(unsigned int count) { ::SendMessage(handle, CB_LIMITTEXT, count, 0); }
	inline void EmptyItems() { ::SendMessage(handle, CB_RESETCONTENT, 0, 0); }
	//How to reset the item?
};