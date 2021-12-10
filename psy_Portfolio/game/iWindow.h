#pragma once

#include "iGraphics.h"

#include "../resource.h"

#if _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#define xprintf printf
#else
#define xprintf (void*)
#endif // _DEBUG

#if 0

// imme나 gdiplus사용할 경우, WIN32_LEAN_AND_MEAN정의하지 않는다.
//#define WIN32_LEAN_AND_MEAN// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// 최소 1.1버전 이상을 사용.
GDIPVER = 0x0110
// strcpy_s 따위 안 쓴다.
_CRT_SECURE_NO_WARNINGS

#endif

extern Graphics * graphics;

void startApp(HDC hdc, VOID_METHOD methodLoad);
void endApp(VOID_METHOD methodFree);
void drawApp(FLOAT_METHOD methodDraw);
void resizeApp(int width, int height);
iPoint coordinate(int x, int y);

void initWndCtrlSystem();

typedef HBRUSH(*Method_Color_Update)(WPARAM wParam, LPARAM lParam);

HWND createWndStatic(const char* str, int x, int y, int width, int height, Method_Color_Update mColor);
HWND createWndButton(const char* str, int x, int y, int width, int height);
HWND createWndCheckBox(const char* str, int x, int y, int width, int height);
bool getWndCheckBox(HWND hwnd);
void setWndCheckBox(HWND hwnd, bool on);

HWND createWndComboBox(const char** str, int strNum, int x, int y, int width, int height);
void addWndComboBox(HWND hwnd, int index, const char* str);
void addWndComboBox(HWND hwnd, const char** str, int strNum);
void removeWndComboBox(HWND hwnd, int index);
int countWndComboBox(HWND hwnd);
int indexWndComboBox(HWND hwnd);
void setWndComboBox(HWND hwnd, int index);
char* getWndComboBox(HWND hwnd, int index);

HWND createWndListBox(const char** str, int strNum, int x, int y, int width, int height);
void addWndListBox(HWND hwnd, int index, const char* str);
void addWndListBox(HWND hwnd, const char** str, int strNum);
void removeWndListBox(HWND hwnd, int index);
int countWndListBox(HWND hwnd);
int indexWndListBox(HWND hwnd);
void setWndListBox(HWND hwnd, int index);
char* getWndListBox(HWND hwnd, int index);
void setWndListBoxData(HWND hwnd, int index, char* str);

HWND createWndRadio(const char* str, int x, int y, int width, int height);
bool getWndRadio(HWND hwnd);
void setWndRadio(HWND hwnd, bool on);

HWND createWndGroupBox(const char* str, int x, int y, int width, int height);

enum WndEditStyle {
	WndEditAll = 0,
	WndEditInt,
	WndEditFloat
};

HWND createWndEditBox(const char* str, int x, int y, int width, int height, WndEditStyle style);
HWND createWndEditBoxMultiline(const char* str, int x, int y, int width, int height);

// ===================================================
//	Common
// ===================================================
void enableWnd(HWND hwnd, bool enable);
void setWndText(HWND hwnd, const char* szFormat, ...);
char* getWndText(HWND hwnd);
int getWndInt(HWND hwnd);
float getWndFloat(HWND hwnd);

// ===================================================
//	FileDlg
// ===================================================
const char* openFileDlg(bool open, LPCWSTR filter, const wchar_t* _path);
const char* openFileDlg(bool open, const char* filter);

// ===================================================
//	ChooseColor
// ===================================================
typedef void (*CHOOSE_COLOR)(float r, float g, float b);
void showChooseColor(CHOOSE_COLOR method);


// ===================================================
//	Thread
// ===================================================
typedef void (*MethodThread)(void* parm);
HANDLE startThread(MethodThread method, void* parm = NULL);
void waitUntilThread(HANDLE* handle, int numHandle);

// ===================================================
//	iCriticalSection
// ===================================================
class iCriticalSection
{
private:
	iCriticalSection();
public:
	static iCriticalSection* share();

	virtual ~iCriticalSection();

	void start();
	void end();

public:
	CRITICAL_SECTION cs;
};


