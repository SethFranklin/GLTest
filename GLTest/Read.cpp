
#include <string>
#include <fstream>

#include "Read.h"

std::string Read::File(std::string Directory)
{

	std::ifstream Stream;
	std::string String;

	Stream.open(Directory);

	char CharBuffer;

	while (Stream.get(CharBuffer))
	{

		String += CharBuffer;

	}

	Stream.close();

	return String;

}