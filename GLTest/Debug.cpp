
#include <fstream>
#include <string>

#include "Debug.h"

enum StateEnum {ON, OFF};
StateEnum State = OFF;

std::ofstream Stream;
const char LogFile[] = "Debug.log";

void Debug::Enable()
{

	if (State == OFF)
	{

		Stream.open(LogFile);

		State = ON;

	}

}

void Debug::Disable()
{

	if (State == ON)
	{

		Stream.close();

		State = OFF;

	}

}

void Debug::Log(std::string Output)
{

	if (State == ON)
	{

		Stream << Output << std::endl;

	}

}

void Debug::Log(float Output)
{

	if (State == ON)
	{

		Stream << Output << std::endl;

	}

}

void Debug::Log(char Output[])
{

	if (State == ON)
	{

		Stream << Output << std::endl;

	}

}

void Debug::Log(int Output)
{

	if (State == ON)
	{

		Stream << Output << std::endl;

	}

}