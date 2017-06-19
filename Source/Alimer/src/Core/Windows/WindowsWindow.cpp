/*
** Alimer - Copyright (C) 2016-2017 Amer Koleci.
**
** This file is subject to the terms and conditions defined in
** file 'LICENSE.md', which is part of this source code package.
*/

#include "WindowsWindow.h"
#include "Engine/Application.h"
#include <Dbt.h>

namespace Alimer
{
	namespace priv
	{
		static const float DipsPerInch = 96.0f;
		uint32_t          windowCount = 0; // Windows owned by Engine
		static const LPCWSTR  _ALIMER__WNDCLASSNAME = L"AlimerWindow";

		static BOOL(WINAPI* registerTouchWindow)(HWND, ULONG) = nullptr;
		static BOOL(WINAPI* getTouchInputInfo)(HTOUCHINPUT, UINT, PTOUCHINPUT, int) = nullptr;
		static BOOL(WINAPI* closeTouchInputHandle)(HTOUCHINPUT) = nullptr;
		static LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static bool functionsInitialized = false;

		static void CheckFunctionsInit()
		{
			if (!functionsInitialized)
			{
				HMODULE userDll = GetModuleHandleW(L"user32.dll");
				registerTouchWindow = (BOOL(WINAPI*)(HWND, ULONG))(void*)GetProcAddress(userDll, "RegisterTouchWindow");
				getTouchInputInfo = (BOOL(WINAPI*)(HTOUCHINPUT, UINT, PTOUCHINPUT, int))(void*)GetProcAddress(userDll, "GetTouchInputInfo");
				closeTouchInputHandle = (BOOL(WINAPI*)(HTOUCHINPUT))(void*)GetProcAddress(userDll, "CloseTouchInputHandle");

				functionsInitialized = true;
			}
		}

		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			WindowsWindow* window = (WindowsWindow*)GetPropW(hwnd, L"ALIMER");
			if (!window)
			{
				switch (msg)
				{
				case WM_DEVICECHANGE:
				{
					if (wParam == DBT_DEVNODES_CHANGED)
					{
					}
					else if (wParam == DBT_DEVICEARRIVAL)
					{
					}
					else if (wParam == DBT_DEVICEREMOVECOMPLETE)
					{
					}
					break;
				}
				}

				return DefWindowProcW(hwnd, msg, wParam, lParam);
			}

			if (window->GetWindowHandle() != hwnd)
			{
				// Ignore messages that are not for our window.
				return DefWindowProcW(hwnd, msg, wParam, lParam);
			}

			// Skip emulated mouse events that are caused by touch
			bool emulatedMouse = (GetMessageExtraInfo() & 0xffffff00) == 0xff515700;
			Application* app = Application::GetInstance();

			switch (msg)
			{
			case WM_CREATE:
				::UpdateWindow(hwnd);
				break;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
				break;

			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
				if (!emulatedMouse)
				{
					//HandleMouseButtonEvent(msg, wParam, lParam);
				}

				if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONUP)
					return TRUE;

				return 0;

			case WM_MOUSEMOVE:
				if (!emulatedMouse)
				{

				}

				return TRUE;

			case WM_MOUSEWHEEL:
			case WM_MOUSEHWHEEL:
				return TRUE;

			case WM_TOUCH:
				return TRUE;

			case WM_SETCURSOR:
				if (LOWORD(lParam) == HTCLIENT)
				{
					// TODO:
				}

				break;

			case WM_SETFOCUS:
				break;

			case WM_KILLFOCUS:
				break;

			case WM_SIZE:
				break;

			case WM_PAINT:
				break;

			case WM_ERASEBKGND:
				return TRUE;

			case WM_SYSCOMMAND:
			{
				switch (wParam & 0xfff0)
				{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				{
					if (app && !app->IsScreenSaverEnabled())
					{
						// Disable screensaver.
						return 0;
					}

					break;
				}
				// Disable accessing menu using alt key
				case SC_KEYMENU:
					return 0;
				}

				break;
			}

			case WM_CLOSE:
				// TODO: Sent event
				if (windowCount == 1)
					PostQuitMessage(0);

				return 0;

			case WM_DPICHANGED:
				break;
			}

			return DefWindowProcW(hwnd, msg, wParam, lParam);
		}

		WindowsWindow::WindowsWindow(uint32_t width, uint32_t height, const String& title, bool resizable, bool fullscreen)
		{
			CheckFunctionsInit();

			// Get the instance of this application.
			auto hInstance = GetModuleHandleW(nullptr);

			if (windowCount == 0)
			{
				WNDCLASSEXW wc;
				memset(&wc, 0, sizeof(wc));
				wc.cbSize = sizeof(wc);
				wc.style = CS_HREDRAW | CS_VREDRAW;
				wc.lpfnWndProc = WndProc;
				wc.hInstance = hInstance;
				wc.hCursor = LoadCursor(NULL, IDC_ARROW);
				wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
				wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
				wc.lpszClassName = _ALIMER__WNDCLASSNAME;
				wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

				if (!RegisterClassExW(&wc))
				{
					ALIMER_ERROR("Could not register window class!");
				}
			}

			uint32_t screenWidth = GetSystemMetrics(SM_CXSCREEN);
			uint32_t screenHeight = GetSystemMetrics(SM_CYSCREEN);

			// TODO: Manage fullscreen
			DWORD windowWindowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_BORDER | WS_DLGFRAME | WS_THICKFRAME | WS_GROUP | WS_TABSTOP;

			if (resizable)
			{
				windowWindowedStyle |= WS_SIZEBOX | WS_MAXIMIZEBOX;
			}

			DWORD windowFullscreenStyle = WS_CLIPSIBLINGS | WS_GROUP | WS_TABSTOP;

			DWORD windowStyle = 0;
			DWORD dwExStyle = WS_EX_APPWINDOW;

			if (fullscreen)
			{
				windowStyle = windowFullscreenStyle;
			}
			else
			{
				windowStyle = windowWindowedStyle;
				dwExStyle |= WS_EX_WINDOWEDGE;
			}

			LONG x = 0;
			LONG y = 0;
			if (fullscreen)
			{
				width = screenWidth;
				height = screenHeight;
			}
			else
			{
				x = (LONG)((screenWidth - width) / 2);
				y = (LONG)((screenHeight - height) / 2);
			}

			RECT rect = { 0, 0, (LONG)width, (LONG)height };
			AdjustWindowRectEx(&rect, windowStyle, FALSE, dwExStyle);

			// Create a window.
			std::wstring titleString = std::wstring(title.begin(), title.end());

			HWND hwnd = CreateWindowExW(
				WS_EX_APPWINDOW,
				_ALIMER__WNDCLASSNAME,
				titleString.c_str(),
				windowStyle,
				x,
				y,
				rect.right - rect.left,
				rect.bottom - rect.top,
				nullptr,
				nullptr,
				hInstance,
				nullptr);

			if (hwnd == nullptr)
			{
				ALIMER_LOGERROR("Windows: Cannot create window");
				return;
			}

			_hwnd = hwnd;
			windowCount++;

			// Enable touch input if available
			if (registerTouchWindow)
			{
				if (!registerTouchWindow(hwnd, TWF_FINETOUCH | TWF_WANTPALM))
				{
					ALIMER_LOGWARNING("Failed to enable touch for window");
				}
			}

			_monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);

			// TODO: Resolve content scale from DPI
			/*if (__win32Data.shcoreDLL &&
				__win32Data.GetDpiForMonitor)
			{
				UINT dpiX = 0;
				UINT dpiY = 0;

				if (SUCCEEDED(__win32Data.GetDpiForMonitor(_monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)))
				{
					_contentScale = static_cast<float>(dpiX) / DipsPerInch;
				}
			}*/

			if (fullscreen)
			{
				//SwitchFullscreen(fullscreen);
			}

			_hdc = ::GetDC(hwnd);

			//_location = Vector2((float)windowRect.left, (float)windowRect.top);
			SetPropW(hwnd, L"ALIMER", this);

			// Show the window.
			ShowWindow(hwnd, SW_SHOW);
		}

		WindowsWindow::~WindowsWindow()
		{
			if (_hwnd)
			{
				if (_hdc)
				{
					ReleaseDC(_hwnd, _hdc);
					_hdc = nullptr;
				}

				RemovePropW(_hwnd, L"ALIMER");
				DestroyWindow(_hwnd);
				_hwnd = nullptr;

				// Decrement the window count
				windowCount--;
			}

			// Unregister window class if we were the last window
			if (windowCount == 0)
			{
				UnregisterClassW(_ALIMER__WNDCLASSNAME, GetModuleHandleW(nullptr));
			}
		}

		WindowHandle WindowsWindow::GetWindowHandle() const
		{
			return _hwnd;
		}
	}
}