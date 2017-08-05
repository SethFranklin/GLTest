
#include <windows.h>

#include "Backend.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	Backend::Start();

	return 0;

}