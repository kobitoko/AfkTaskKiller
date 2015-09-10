// References: 
// Keyboard and low level hooks in winapi:
// Nick Dandoulakis' answer http://stackoverflow.com/questions/1437158/c-win32-keyboard-events
//
// Killing a process using system
// James Hugard's answer http://stackoverflow.com/questions/1916574/how-to-effectively-kill-a-process-in-c-win32
//
// Microsoft's MSDN https://msdn.microsoft.com/en-us/library/windows/desktop/bg126469%28v=vs.85%29.aspx

#include "afkTaskKill.h"

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("This program needs 2 arguments, the executable name of the app to kill, and the afk time.\nYou have %d arguments", argc - 1);
		return EXIT_FAILURE;
	}

	appName = new std::string(argv[1]);
	afkTime = atof(argv[2]);

	if(verbose)
		printf("afkTime is %f\nprogram to kill is %s\n", afkTime, appName->c_str());

	hKeybdHook = SetWindowsHookEx(WH_KEYBOARD_LL, &keybdProc, GetModuleHandle(NULL), 0);
	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, &mouseProc, GetModuleHandle(NULL), 0);

	oldT = clock();
	// Set timer to a 1/2 a second interval. ID should be 0 since hWnd is NULL and it's not intended to replace an existing timer.
	timerID = SetTimer(NULL, 0, 500, &endTimerProc);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	clean();
	return EXIT_SUCCESS;
}

VOID CALLBACK endTimerProc(HWND hwnd, UINT umsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent == timerID)
	{
		timeDiff = static_cast<double>(clock() - oldT);
		if (timeDiff > afkTime)
		{
			std::string command = std::string("taskkill /F /T /IM \"" + *appName + "\"");

			if (verbose)
				printf("AFK! NOW I SHALL KILL THE APP\nPerforming %s\n\n", command.c_str());

			system(command.c_str());

			clean();
			exit(EXIT_SUCCESS);
		}
	}
}

LRESULT CALLBACK keybdProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (WM_KEYDOWN == wParam || WM_KEYUP == wParam)
	{
		if (verbose)
			printf("Key was pressed or released. Resetting afk. Counter was %fms\n", timeDiff);

		oldT = clock();
	}
	// Do not swallow the keys. They all shall pass!
	return CallNextHookEx(hKeybdHook, code, wParam, lParam);
}

LRESULT CALLBACK mouseProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (WM_LBUTTONDOWN == wParam || WM_LBUTTONUP == wParam || WM_MOUSEMOVE == wParam
		|| WM_MOUSEWHEEL == wParam || WM_RBUTTONDOWN == wParam || WM_RBUTTONUP)
	{
		if (verbose)
			printf("Mouse or wheel was moved or button was pressed or released.\nResetting afk. Counter was %fms\n", timeDiff);

		oldT = clock();
	}
	// Do not swallow input. All shall pass!
	return CallNextHookEx(hKeybdHook, code, wParam, lParam);
}

void clean()
{
	// cleanup
	KillTimer(NULL, timerID);
	UnhookWindowsHookEx(hKeybdHook);
	UnhookWindowsHookEx(hMouseHook);
	delete appName;
}