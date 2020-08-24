#ifndef _CPRINT_H_
#define _CPRINT_H_

#include <windows.h>

namespace fontcolor {

const int Blue = 1;
const int Green = 2;
const int Red = 4;
const int White = 7;
const int Intensity = 8;

}

template <class... T>
void cprintf(const char* str, WORD color, T... x) {
	WORD colorOld;
	HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	SetConsoleTextAttribute(handle, color);
	printf(str, x...);
	SetConsoleTextAttribute(handle, colorOld);
}

#endif
