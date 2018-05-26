#pragma once
#include "IModifier.h"
#include "..\zzxcf_tokenizer\zzxcf_tokenizer.h"
#include <iostream>
#include <fstream>
//https://docs.microsoft.com/en-us/cpp/standard-library/filesystem
#include <filesystem>
class BaseMain
{
public:
	static int Main(int argc, char**argv, IModifier&modifier)
	{
		if (2 != argc)
		{
			std::cout << "Error: count of arguments not equals 2.\nProvide a file path.";
			return -1;
		}
		std::string filePath(argv[1]);
		if (!std::experimental::filesystem::exists(filePath))
		{
			std::cout << "Error: specified file doesn't exist.\n";
			return -1;
		}
		std::string backupFile(filePath);
		backupFile += ".backup";
		if (!std::experimental::filesystem::exists(backupFile))
		{
			std::experimental::filesystem::copy_file(filePath, backupFile);
		}
		std::string originalFile(filePath);
		originalFile += ".orig";
		std::experimental::filesystem::copy_file(filePath, originalFile, std::experimental::filesystem::copy_options::overwrite_existing);
		std::ifstream fileStream(filePath);
		std::string fileContent;
		fileContent.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
		fileStream.close();
		TokenSequence tokenSequence;
		tokenize(fileContent, tokenSequence);
		modifier.Modify(tokenSequence);
		std::ofstream overwriteFileStream(filePath, std::ofstream::trunc);
		for each (std::string var in tokenSequence)
		{
			overwriteFileStream << var;
		}
		overwriteFileStream.close();
		return 0;
	}
};
