#pragma once

#include <string>
#include <fstream>

namespace sparky
{
	class FileUtils
	{
	public:
		static std::string read_file(const char* filepath)
		{
			FILE* file = fopen(filepath, "rt");              // r -- read-only  t-- textfile as it should recognize line endings which is important for shaders.
			fseek(file, 0, SEEK_END);                        // seek till end
			unsigned long length = ftell(file);              // no of bytes/character read
			char* data = new char[length + 1];
			memset(data, 0, length + 1);
			fseek(file, 0, SEEK_SET);                        // resetting seek position to beginning
			fread(data, 1, length, file);
			fclose(file);

			std::string result(data);
			delete[] data;
			return result;
		}
	};
	
}