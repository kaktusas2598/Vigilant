#pragma once

#include <stdio.h>
#include <string>
#include <vector>

namespace Vigilant {

	//struct DirEntry
	//{
		//std::string path;
		//bool isDirectory;
	//};

	class IOManager
	{
		public:
			static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
			static bool readFileToBuffer(std::string filePath, std::string& buffer);

			/// Gets all directory entries of specified directory path and stores them in rvEntries
			/// Retruns false if path is not a directory
			//static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);
			//static bool makeDirectory(const char* path);
	};


}
