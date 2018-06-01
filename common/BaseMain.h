#pragma once
#include "..\zzxcf\IModifier.h"
#include "..\zzxcf_tokenizer\zzxcf_tokenizer.h"
#include <iostream>
#include <fstream>
#include <chrono>
//https://docs.microsoft.com/en-us/cpp/standard-library/filesystem
#include <filesystem>
class BaseMain
{
public:
	static int Main(int argc, char**argv, IModifier *modifier)
	{
		if (2 != argc)
		{
			std::cout << "Error: count of arguments not equals 2." << std::endl;
			std::cout << "Provide a file path." << std::endl;
			return -1;
		}
		std::string sourceFile(argv[1]);
		if (!std::experimental::filesystem::exists(sourceFile))
		{
			std::cout << "Error: specified file doesn't exist." << std::endl;
			return -1;
		}
		if ((std::experimental::filesystem::status(sourceFile).permissions() & std::experimental::filesystem::perms::owner_write) == std::experimental::filesystem::perms::none)
		{
			std::cout << "Error: owner cannot writes " << sourceFile << std::endl;
			return -1;
		}
		std::string backupFile(sourceFile);
		backupFile += ".backup";
		if (!std::experimental::filesystem::exists(backupFile))
		{
			std::experimental::filesystem::copy_file(sourceFile, backupFile);
			std::cout << "Information: backup " << sourceFile << " to " << backupFile << std::endl;
		}
		std::string originalFile(sourceFile);
		originalFile += ".orig";
		if (std::experimental::filesystem::exists(originalFile) && (std::experimental::filesystem::status(originalFile).permissions() & std::experimental::filesystem::perms::owner_write) == std::experimental::filesystem::perms::none)
		{
			std::cout << "Warning: owner cannot writes " << originalFile << std::endl;			
		}
		else
		{
			std::experimental::filesystem::copy_file(sourceFile, originalFile, std::experimental::filesystem::copy_options::overwrite_existing);
			std::cout << "Information: copy " << sourceFile << " to " << originalFile << std::endl;
		}
		std::ifstream iFileStream(sourceFile);
		std::string fileContent;
		fileContent.assign(std::istreambuf_iterator<char>(iFileStream), std::istreambuf_iterator<char>());
		iFileStream.close();
		TokenSequence tokenSequence;
		tokenize(fileContent, tokenSequence);
		auto beginTime = std::chrono::steady_clock::now();
		modifier->Modify(tokenSequence);
		auto endTime = std::chrono::steady_clock::now();
		auto modifyDuration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - beginTime);
		std::cout << "Modification took " << modifyDuration.count() << "seconds." << std::endl;
		delete modifier;
		std::ofstream oFileStream(sourceFile, std::ofstream::trunc);
		for (auto &token : tokenSequence)
		{
			oFileStream << token;
		}
		oFileStream.close();
		std::ifstream sourceFileStream(sourceFile, std::ifstream::binary | std::ifstream::ate);
		std::ifstream originalFileStream(originalFile, std::ifstream::binary | std::ifstream::ate);
		if (sourceFileStream.tellg() == originalFileStream.tellg())
		{
			if (std::equal(std::istreambuf_iterator<char>(sourceFileStream), std::istreambuf_iterator<char>(), std::istreambuf_iterator<char>(originalFileStream)))
			{
				std::cout << sourceFile << " content not been changed." << std::endl;
			}
		}
		originalFileStream.close();
		sourceFileStream.close();
		return 0;
	}
};
