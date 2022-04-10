#pragma once

#include"../Basic/EspWindow.h"
#include"EspImageList.h"

class EspListView :public EspWindow
{
public:
	virtual bool Create(
		int x,
		int y,
		int width,
		int height,
		HWND parent_window,
		DWORD style,
		DWORD extended_style
	)
	{
		style |= (WS_CHILD | WS_VISIBLE);
		handle = ::CreateWindowEx(WS_EX_CLIENTEDGE,
			WC_LISTVIEW,
			NULL,
			style,
			::MulDiv(x, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(y, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(width, EspGetApp()->GetAppDpi(), 96),
			::MulDiv(height, EspGetApp()->GetAppDpi(), 96),
			parent_window,
			NULL,
			EspGetApp()->GetInstanceHandle(),
			nullptr
		);
		if (handle == nullptr)
			return false;

		::SendMessage(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, extended_style);
		return true;
	}

	inline void InsertColumn(unsigned int index, LVCOLUMN* column)
	{
		column->cx = ::MulDiv(column->cx, EspGetApp()->GetAppDpi(), 96);
		::SendMessage(handle, LVM_INSERTCOLUMN, index, (LPARAM)column);
	}
	inline void InsertColumn(unsigned int index, unsigned int width, LPCTSTR str)
	{
		LVCOLUMN col;
		col.mask = LVCF_TEXT | LVCF_WIDTH;
		col.cx = ::MulDiv(col.cx, EspGetApp()->GetAppDpi(), 96);
		col.pszText = (LPTSTR)str;
		::SendMessage(handle, LVM_INSERTCOLUMN, index, (LPARAM)&col);
	}
	inline void InsertColumn(unsigned int index, unsigned int width, LPCTSTR str, int image_index)
	{
		LVCOLUMN col;
		col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_IMAGE;
		col.cx = width;
		col.pszText = (LPTSTR)str;
		col.iImage = image_index;
		::SendMessage(handle, LVM_INSERTCOLUMN, index, (LPARAM)&col);
	}
	inline void InsertItem(LVITEM item) { ::SendMessage(handle, LVM_INSERTITEM, 0, (LPARAM)&item); }
	void InsertItem(unsigned int index, LPCTSTR str)
	{
		LVITEM item;
		item.iItem = index;
		item.iSubItem = 0;
		item.mask = LVIF_TEXT;
		item.pszText = (LPTSTR)str;
		::SendMessage(handle, LVM_INSERTITEM, 0, (LPARAM)&item);
	}
	void InsertItem(unsigned int index, LPCTSTR str, int image_index)
	{
		LVITEM item;
		item.iItem = index;
		item.iSubItem = 0;
		item.mask = LVIF_TEXT | LVIF_IMAGE;
		item.pszText = (LPTSTR)str;
		item.iImage = image_index;
		::SendMessage(handle, LVM_INSERTITEM, index, (LPARAM)&item);
	}
	inline void SetItem(LVITEM item) { ::SendMessage(handle, LVM_SETITEM, 0, (LPARAM)&item); }
	void SetItem(unsigned int index, unsigned int subindex, LPCTSTR str)
	{
		LVITEM item;
		item.iItem = index;
		item.iSubItem = subindex;
		item.mask = LVIF_TEXT;
		item.pszText = (LPTSTR)str;
		::SendMessage(handle, LVM_SETITEM, 0, (LPARAM)&item);
	}
	void SetItem(unsigned int index, unsigned int subindex, LPCTSTR str, int image_index)
	{
		LVITEM item;
		item.iItem = index;
		item.iSubItem = subindex;
		item.mask = LVIF_TEXT | LVIF_IMAGE;
		item.pszText = (LPTSTR)str;
		item.iImage = image_index;
		::SendMessage(handle, LVM_INSERTITEM, 0, (LPARAM)&item);
	}

	inline void DeleteAllItems() { ::SendMessage(handle, LVM_DELETEALLITEMS, 0, 0); }
	inline void DeleteColumn(unsigned int index) { ::SendMessage(handle, LVM_DELETECOLUMN, index, 0); }
	inline void DeleteItem(unsigned int index) { ::SendMessage(handle, LVM_DELETEITEM, index, 0); }
	inline void EnsureVisible(unsigned int index, bool entire) { ::SendMessage(handle, LVM_ENSUREVISIBLE, index, entire); }
	inline int FindItemText(LVFINDINFO info, unsigned int begin_index) { return ::SendMessage(handle, LVM_FINDITEM, begin_index, (LPARAM)&info); }
	inline int FindItemText(LPCTSTR text, unsigned int begin_index, bool exact = false)
	{
		LVFINDINFO info;
		info.flags = exact ? LVFI_STRING : LVFI_PARTIAL;
		info.psz = text;
		return ::SendMessage(handle, LVM_FINDITEM, begin_index, (LPARAM)&info);
	}
	inline int FindItemText(LVITEM item, unsigned int begin_index)
	{
		LVFINDINFO info;
		info.flags = LVFI_PARAM;
		info.lParam = (LPARAM)&item;
		return ::SendMessage(handle, LVM_FINDITEM, begin_index, (LPARAM)&info);
	}
	inline COLORREF GetBackgroundColor() { return ::SendMessage(handle, LVM_GETBKCOLOR, 0, 0); }
	inline void GetColumnInfo(unsigned int index, const LVCOLUMN* info) { ::SendMessage(handle, LVM_GETCOLUMN, index, (LPARAM)info); }
	inline int GetColumnWidth(unsigned int index) { return ::SendMessage(handle, LVM_GETCOLUMNWIDTH, index, 0); }
	inline int GetItemCountPerPage() { return ::SendMessage(handle, LVM_GETCOUNTPERPAGE, 0, 0); }
	inline HIMAGELIST GetImageList(UINT flag) { return (HIMAGELIST)::SendMessage(handle, LVM_GETIMAGELIST, flag, 0); }
	inline int GetItemCount() { return ::SendMessage(handle, LVM_GETITEMCOUNT, 0, 0); }
	inline void GetItemPoint(unsigned int index, POINT* point) { ::SendMessage(handle, LVM_GETITEMINDEXRECT, index, (LPARAM)point); }
	inline void GetItemRect(unsigned int index, RECT* rect) { ::SendMessage(handle, LVM_GETITEMRECT, index, (LPARAM)rect); }
	inline void GetItemText(unsigned int index, unsigned int subindex, LPTSTR str, int size)
	{
		LVITEM item;
		item.iSubItem = subindex;
		::SendMessage(handle, LVM_GETITEMTEXT, index, (LPARAM)&item);
		str = item.pszText;
		size = item.cchTextMax;
	}
	inline int GetSelectedColumn() { return ::SendMessage(handle, LVM_GETSELECTEDCOLUMN, 0, 0); }
	inline int GetSelectedCount() { return ::SendMessage(handle, LVM_GETSELECTEDCOUNT, 0, 0); }
	inline COLORREF GetTextBackgroundColor() { return ::SendMessage(handle, LVM_GETTEXTBKCOLOR, 0, 0); }
	inline COLORREF GetTextColor() { return ::SendMessage(handle, LVM_GETTEXTCOLOR, 0, 0); }
	inline int GetCurrentTopIndex() { return ::SendMessage(handle, LVM_GETTOPINDEX, 0, 0); }
	inline void SetBackgroundColor() { ::SendMessage(handle, LVM_SETBKCOLOR, 0, CLR_NONE); }
	inline void SetBackgroundColor(COLORREF color) { ::SendMessage(handle, LVM_SETBKCOLOR, 0, color); }
	inline void SetColumnInfo(unsigned int index, LVCOLUMN& info) { ::SendMessage(handle, LVM_SETCOLUMN, index, (LPARAM)&info); }
	inline void SetColumnWidth(unsigned int index, unsigned int width) { ::SendMessage(handle, LVM_SETCOLUMN, index, width); }
	inline void SetImageList(HIMAGELIST imglist, int flag) { ::SendMessage(handle, LVM_SETIMAGELIST, flag, (LPARAM)imglist); }
	inline void SetImageList(EspImageList imglist, int flag) { ::SendMessage(handle, LVM_SETIMAGELIST, flag, (LPARAM)imglist.GetHandle()); }
	inline void SetItemCount(unsigned int count) { ::SendMessage(handle, LVM_SETITEMCOUNT, count, 0); }
	inline void SetItemText(unsigned int index, unsigned int subindex, LPCTSTR str)
	{
		LVITEM item;
		item.iSubItem = subindex;
		item.pszText = (LPTSTR)str;
		::SendMessage(handle, LVM_SETITEMTEXT, index, (LPARAM)&item);
	}
	inline void SetSelectedColumn(int index) { ::SendMessage(handle, LVM_SETSELECTEDCOLUMN, index, 0); }
	inline void SetTextBackgroundColor() { ::SendMessage(handle, LVM_SETTEXTBKCOLOR, 0, CLR_NONE); }
	inline void SetTextBackgroundColor(COLORREF color) { ::SendMessage(handle, LVM_SETTEXTBKCOLOR, 0, color); }
	inline void SetTextColor(COLORREF color) { ::SendMessage(handle, LVM_SETTEXTCOLOR, 0, color); }
};

//LVM_GETEDITCONTROL https://docs.microsoft.com/en-us/windows/win32/controls/lvm-geteditcontrol