#pragma once

#include"../Basic/EspWindow.h"

class EspProgressBar :public EspWindow
{
public:
	virtual bool Create(
		int x,
		int y,
		int width,
		int height,
		HWND parent_window,
		bool marquee = false
	)
	{
		handle = ::CreateWindowEx(
			WS_EX_CLIENTEDGE,
			PROGRESS_CLASS,
			NULL,
			WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | (marquee ? PBS_MARQUEE : (PBS_SMOOTH | PBS_SMOOTHREVERSE)),
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

	void SetRange(UINT min, UINT max) { ::SendMessage(handle, PBM_SETRANGE, 0, MAKELPARAM(min, max)); }
	void SetPos(UINT pos) { ::SendMessage(handle, PBM_SETPOS, pos, 0); }
	void SetState(UINT state) { ::SendMessage(handle, PBM_SETSTATE, state, 0); }
	void SetNormalState() { ::SendMessage(handle, PBM_SETSTATE, PBST_NORMAL, 0); }
	void SetPausedState() { ::SendMessage(handle, PBM_SETSTATE, PBST_PAUSED, 0); }
	void SetErrorState() { ::SendMessage(handle, PBM_SETSTATE, PBST_ERROR, 0); }
	void SetMarquee(bool open, DWORD time_per) { ::SendMessage(handle, PBM_SETMARQUEE, open, time_per); }
	int GetMaximumPos()const { PBRANGE range; ::SendMessage(handle, PBM_GETRANGE, true, (LPARAM)&range); return range.iHigh; }
	int GetMinimumPos()const { PBRANGE range; ::SendMessage(handle, PBM_GETRANGE, false, (LPARAM)&range); return range.iLow; }
	UINT GetPos()const { return  ::SendMessage(handle, PBM_GETPOS, 0, 0); }
	UINT GetState() { return ::SendMessage(handle, PBM_GETSTATE, 0, 0); }
};