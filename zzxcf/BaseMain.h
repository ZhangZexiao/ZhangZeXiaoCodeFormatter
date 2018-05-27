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
			std::cout << "Error: count of arguments not equals 2.\nProvide a file path.\n";
			return -1;
		}
		std::string sourceFile(argv[1]);
		if (!std::experimental::filesystem::exists(sourceFile))
		{
			std::cout << "Error: specified file doesn't exist.\n";
			return -1;
		}
		std::string backupFile(sourceFile);
		backupFile += ".backup";
		if (!std::experimental::filesystem::exists(backupFile))
		{
			std::experimental::filesystem::copy_file(sourceFile, backupFile);
		}
		std::string originalFile(sourceFile);
		originalFile += ".orig";
		std::experimental::filesystem::copy_file(sourceFile, originalFile, std::experimental::filesystem::copy_options::overwrite_existing);
		std::ifstream iFileStream(sourceFile);
		std::string fileContent;
		fileContent.assign(std::istreambuf_iterator<char>(iFileStream), std::istreambuf_iterator<char>());
		iFileStream.close();
		TokenSequence tokenSequence;
		tokenize(fileContent, tokenSequence);
		modifier.Modify(tokenSequence);
		std::ofstream oFileStream(sourceFile, std::ofstream::trunc);
		for each (TokenSequence::value_type token in tokenSequence)
		{
			oFileStream << token;
		}
		oFileStream.close();
		return 0;
	}
};
