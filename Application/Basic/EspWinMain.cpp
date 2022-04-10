#include<crtdbg.h>

#include"../EspTimeManagerApplication.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _UNICODE
#define EspWinMain wWinMain
#else
#define EspWinMain WinMain
#endif

EspApplication* EspGetApp()
{
	return &app;
}

int __stdcall EspWinMain(
	_In_ HINSTANCE instance_handle,//Current Instance Handle
	_In_opt_ HINSTANCE pre_instance_handle,//Last identical app instance handle, or NULL
	_In_ LPWSTR command_line,
	_In_ int command_show)
{
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ULONG_PTR gdi_token;
	Gdiplus::GdiplusStartupInput gdi_input;
	Gdiplus::GdiplusStartupOutput gdi_output;
	Gdiplus::GdiplusStartup(&gdi_token, &gdi_input, &gdi_output);

	DPI_AWARENESS_CONTEXT dac = ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

	app.InitApp(instance_handle, pre_instance_handle, command_line, command_show);
	app.InitInstance();
	app.Run();

	Gdiplus::GdiplusShutdown(gdi_token);

	::SetThreadDpiAwarenessContext(dac);
}