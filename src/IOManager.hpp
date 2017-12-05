#pragma once

#include <stdio.h>
#include <string>
#include <vector>

namespace Vigilant {

	/* \brief IOManager class
	 * IOManager class having utilities to read files
	 * TODO: is this good approach?
	 */
	class IOManager
	{
		public:
			static bool readFileToBuffer(std::string filePath, std::string& buffer);
	};

}
