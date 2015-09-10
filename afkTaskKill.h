#pragma once

#include <Windows.h>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <tchar.h>

HHOOK hKeybdHook;
HHOOK hMouseHook;
UINT_PTR timerID;
std::string* appName;
clock_t oldT;
double afkTime;
double timeDiff;
static bool verbose = false;

VOID CALLBACK endTimerProc(HWND hwnd, UINT umsg, UINT_PTR idEvent, DWORD dwTime);
LRESULT CALLBACK keybdProc(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK mouseProc(int code, WPARAM wParam, LPARAM lParam);
void clean();
