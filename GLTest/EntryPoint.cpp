
#ifdef _WIN32

#include <windows.h>

#define main() WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)

#elif __APPLE__

#elif __linux__

#endif

#include "Backend.h"

int main()
{

	Backend::Start();

	return 0;

}