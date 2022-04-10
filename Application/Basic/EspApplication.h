#pragma once

#include<tchar.h>
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>
#include<shellapi.h>
#include<ShellScalingApi.h>
#include<gdiplus.h>

#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"Shcore.lib")
#pragma comment(lib,"Gdiplus.lib")

class EspApplication;

extern EspApplication* EspGetApp();

class EspApplication
{
private:
	HINSTANCE instance_handle;
	HINSTANCE previous_instance_handle;
	LPTSTR command_line;
	int command_show;

	LPCTSTR app_name;

	HANDLE thread_handle;
	DWORD thread_id;
	HANDLE process_handle;
	DWORD process_id;

	UINT system_dpi;

public:
	EspApplication(LPCTSTR new_app_name)
		:app_name(new_app_name), thread_handle(::GetCurrentThread()), thread_id(::GetCurrentThreadId()),
		process_handle(::GetCurrentProcess()), process_id(::GetCurrentProcessId())
	{}
	~EspApplication()
	{

	}

	virtual void InitApp(HINSTANCE new_instance_handle, HINSTANCE new_pre_instance_handle, LPTSTR new_command_line, int new_command_show)
	{
		instance_handle = new_instance_handle;
		previous_instance_handle = new_pre_instance_handle;
		command_line = new_command_line;
		command_show = new_command_show;
		system_dpi = ::GetDpiForSystem();
	}

	virtual void Run()
	{
		MSG message;

		while (true)
		{
			if (!::GetMessage(&message, nullptr, 0, 0))
			{
				ExitInstance();
				return;
			}
			else
			{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
		}
	}

	virtual void InitInstance() {}
	virtual void ExitInstance() {}

	inline const TCHAR* GetAppName()const { return app_name; }
	inline const HINSTANCE& GetInstanceHandle()const { return instance_handle; }
	inline const HINSTANCE& GetPreviousInstanceHandle()const { return previous_instance_handle; }
	inline const TCHAR* GetCmdLine()const { return command_line; }
	inline const int& GetCmdShow()const { return command_show; }
	inline const HANDLE& GetThreadHandle()const { return thread_handle; }
	inline const DWORD& GetThreadId()const { return thread_id; }
	inline const UINT GetAppDpi()const { return system_dpi; }
};