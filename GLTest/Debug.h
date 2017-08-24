
#pragma once

#include <string>

namespace Debug
{

	void Enable();
	void Disable();

	void Log(std::string Output);
	void Log(char Output[]);
	void Log(float Output);
	void Log(int Output);

}